#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "BookingScheduler.cpp"
using namespace std;

class BookingItem : public testing::Test {
protected:
	void SetUp() override {
		NOT_ON_THE_HOUR = getTime(2021, 3, 26, 9, 5);
		ON_THE_HOUR = getTime(2021, 3, 26, 9, 0);
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
	const int UNDER_CAPACITY = 1;
	const int CAPACITY_PER_HOUR = 3;

	BookingScheduler bookingSceduler{ CAPACITY_PER_HOUR };
};



TEST_F(BookingItem, ReservationAvailableOnlyOnTimeNotAvailableIfNotOnTime) {
	//arrage
	Schedule* schedule = new Schedule{ NOT_ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER };
	
	//act
	EXPECT_THROW(bookingSceduler.addSchedule(schedule), std::runtime_error);

	//assert
	//expected runtime exception
}

TEST_F(BookingItem, ReservationAvailableOnlyOnTimeAvailableIfOnTime) {
	//arrage
	Schedule* schedule = new Schedule{ ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER };

	//act
	bookingSceduler.addSchedule(schedule);

	//assert
	EXPECT_EQ(true, bookingSceduler.hasSchedule(schedule));
}

TEST_F(BookingItem, ReservationHasCapacityOccurExceptionIfOverCapacityAtTheSameTime) {
	//arrage
	Schedule* schedule = new Schedule{ ON_THE_HOUR, CAPACITY_PER_HOUR, CUSTOMER };
	bookingSceduler.addSchedule(schedule);

	//act
	try {
		Schedule* newschedule = new Schedule{ ON_THE_HOUR, UNDER_CAPACITY, CUSTOMER };
		bookingSceduler.addSchedule(newschedule);
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
	bookingSceduler.addSchedule(schedule);

	//act
	//tm differentHour = ON_THE_HOUR;
	//differentHour.tm_hour += 1;
	//cout << differentHour.tm_hour << endl;

	tm differentHour = plusHour(ON_THE_HOUR, 1);
	mktime(&differentHour);
	Schedule* newSchedule = new Schedule{ differentHour, UNDER_CAPACITY, CUSTOMER };
	bookingSceduler.addSchedule(newSchedule);

	//assert
	EXPECT_EQ(true, bookingSceduler.hasSchedule(schedule));
	EXPECT_EQ(true, bookingSceduler.hasSchedule(newSchedule));
}


//TEST(BookingSchedulerTest, 시간대별_인원제한이_있다_같은_시간대가_다르면_Capacity_차있어도_스케쥴_추가_성공) {
//
//}
//
//TEST(BookingSchedulerTest, 예약완료시_SMS는_무조건_발송) {
//
//}
//
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