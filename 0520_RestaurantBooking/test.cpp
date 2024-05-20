#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "BookingScheduler.cpp"
#include "TestableSmsSender.cpp"
#include "TestableMailSender.cpp"
using namespace std;

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
		tm result = { 0 , min, hour, day, mon - 1, year - 1990, 0,0, -1 };
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
	Customer customerWithMail{ "Fake Name", "010-1234-5678", "test@test.com" };
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

TEST_F(BookingItem, DISABLED_ReservationHasCapacitySuccessReservationIfDifferentTime) {
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
	
	//TestableMailSender testableMailSender;
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPACITY_PER_HOUR, customerWithMail };
	//bookingScheduler.setMailSender(&testableMailSender);

	//act
	bookingScheduler.addSchedule(schedule);

	//assert
	EXPECT_EQ(1, testableSmsSender.isSendMethodIsCalled());
}

//TEST(BookingSchedulerTest, 이메일이_없는_경우에는_이메일_미발송) {
//
//}
//
//TEST(BookingSchedulerTest, 이메일이_있는_경우에는_이메일_발송) {
//
//}
//
//TEST(BookingSchedulerTest, 현재날짜가_일요일인_경우_예약불가_예외처리) {
//
//}
//
//TEST(BookingSchedulerTest, 현재날짜가_일요일이_아닌경우_예약가능) {
//
//}