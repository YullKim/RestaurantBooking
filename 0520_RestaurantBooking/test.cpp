#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "BookingScheduler.cpp"
#include "TestableBookingScheduler.cpp"
#include "TestableSmsSender.cpp"
#include "TestableMailSender.cpp"
#include "SundayBookingScheduler.cpp"
#include "MondayBookingScheduler.cpp"
using namespace std;

//class MockBookingScheduler : public TestableBookingScheduler {
//public:
//	//MOCK_METHOD(int, abc, (int), ());
//	//MOCK_METHOD(int, getSum, (int a, int b), (override));
//	//MOCK_METHOD(int, getValue, (), ());
//	MOCK_METHOD(int, getValue, (), ());
//};

class BookingItem : public testing::Test {
protected:
	void SetUp() override {
		NOT_ON_THE_HOUR = getTime(2021, 3, 26, 9, 5);
		ON_THE_HOUR = getTime(2021, 3, 26, 9, 0);

		bookingScheduler.setSmsSender(&testableSmsSender);
		bookingScheduler.setMailSender(&testableMailSender);
	}
public:
	tm getTime(int year, int mon, int day, int hour, int min) {
		tm result = { 0 , min, hour, day, mon - 1, year - 1900, 0,0, -1 };
		mktime(&result);
		return result;
	}

	tm plusHour(tm base, int hour) {
		base.tm_hour += hour;
		mktime(&base);
		return base;
	}


	tm NOT_ON_THE_HOUR;
	tm ON_THE_HOUR;
	Customer CUSTOMER{ "Fake name","010-1234-5678" };
	Customer CUSTOMER_WITH_MAIL{ "Fake Name", "010-1234-5678", "test@test.com" };
	const int UNDER_CAPACITY = 1;
	const int CAPACITY_PER_HOUR = 3;

	BookingScheduler bookingScheduler{ CAPACITY_PER_HOUR };
	TestableSmsSender testableSmsSender;
	TestableMailSender testableMailSender;
};


TEST_F(BookingItem, ReservationAvailableOnlyOnTimeNotAvailableIfNotOnTime) {
	//arrage
	Schedule* schedule = new Schedule{ NOT_ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER };
	
	//act
	EXPECT_THROW(bookingScheduler.addSchedule(schedule), std::runtime_error);

	//assert
	//expected runtime exception
}

TEST_F(BookingItem, ReservationAvailableOnlyOnTimeAvailableIfOnTime) {
	//arrage
	Schedule* schedule = new Schedule{ ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER };

	//act
	bookingScheduler.addSchedule(schedule);

	//assert
	EXPECT_EQ(true, bookingScheduler.hasSchedule(schedule));
}

TEST_F(BookingItem, ReservationHasCapacityOccurExceptionIfOverCapacityAtTheSameTime) {
	//arrage
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPACITY_PER_HOUR, CUSTOMER };
	bookingScheduler.addSchedule(schedule);

	//act
	try {
		Schedule* newschedule = new Schedule{ ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER };
		bookingScheduler.addSchedule(newschedule);
		FAIL();
	}
	catch (std::runtime_error& e) {
		//assert
		EXPECT_EQ(string{ e.what() }, string{"Number of people is over restaurant capacity per hour"});
	}
}

TEST_F(BookingItem, ReservationHasCapacitySuccessReservationIfDifferentTime) {
	//arrage
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPACITY_PER_HOUR, CUSTOMER };
	bookingScheduler.addSchedule(schedule);

	//act
	//tm differentHour = ON_THE_HOUR;
	//differentHour.tm_hour += 1;
	//cout << differentHour.tm_hour << endl;

	tm differentHour = plusHour(ON_THE_HOUR, 1);
	mktime(&differentHour);
	Schedule* newSchedule = new Schedule{ differentHour, UNDER_CAPACITY, CUSTOMER };
	bookingScheduler.addSchedule(newSchedule);

	//assert
	EXPECT_EQ(true, bookingScheduler.hasSchedule(schedule));
	EXPECT_EQ(true, bookingScheduler.hasSchedule(newSchedule));
}


TEST_F(BookingItem, ReservationSendSMSIfSuccessReservation) {
	//arrage
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPACITY_PER_HOUR, CUSTOMER };
	bookingScheduler.setSmsSender(&testableSmsSender);

	//act
	bookingScheduler.addSchedule(schedule);

	//assert
	EXPECT_EQ(true, testableSmsSender.isSendMethodIsCalled());
}


TEST_F(BookingItem, ReservationNotSendMailIfDoNotHaveCustomerMail) {
	//arrage
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPACITY_PER_HOUR, CUSTOMER };

	//act
	bookingScheduler.addSchedule(schedule);

	//assert
	EXPECT_EQ(0, testableMailSender.getCountSendMailMethodIsCalled());
}

TEST_F(BookingItem, ReservationSendMailIfHaveCustomerMail) {
	//arrage
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPACITY_PER_HOUR, CUSTOMER_WITH_MAIL };

	//act
	bookingScheduler.addSchedule(schedule);

	//assert
	EXPECT_EQ(1, testableMailSender.getCountSendMailMethodIsCalled());
}


TEST_F(BookingItem, ReservationNotAvailableOccurExceptionIfTodayIsSunday) {
	//arrage
	BookingScheduler* bookingScheduler = new SundayBookingScheduler{ CAPACITY_PER_HOUR };

	try {
		//act
		Schedule* schedule = new Schedule{ ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER_WITH_MAIL };
		bookingScheduler->addSchedule(schedule);
		FAIL();
	}
	catch (std::runtime_error& e) {
		//assert
		EXPECT_EQ(string{ e.what() }, string{ "Booking system is not available on sunday" });
	}
}


TEST_F(BookingItem, ReservationAvailableIfTodayIsNotSumday) {
	//arrage
	BookingScheduler* bookingScheduler = new MondayBookingScheduler{ CAPACITY_PER_HOUR };

	Schedule* schedule = new Schedule{ ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER_WITH_MAIL };
	bookingScheduler->addSchedule(schedule);

	EXPECT_EQ(true, bookingScheduler->hasSchedule(schedule));
	
}

//TEST(BookingSchedulerTest, 현재날짜가_일요일인_경우_예약불가_예외처리) {
//
//}
////
//TEST(BookingSchedulerTest, 현재날짜가_일요일이_아닌경우_예약가능) {
//
//}