#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "BookingScheduler.cpp"
using namespace std;

class BookingItem : public testing::Test {
protected:
	void SetUp() override {
		notOnTheHour = getTime(2021, 3, 26, 9, 5);
		onTheHour = getTime(2021, 3, 26, 9, 0);
	}
public:
	tm getTime(int year, int mon, int day, int hour, int min) {
		tm result = { 0 , min, hour, day, mon - 1, year - 1990, 0,0, -1 };
		mktime(&result);
		return result;
	}

	tm NOT_ON_THE_HOUR;
	tm ON_THE_HOUR;
	Customer CUSTOMER{ "Fake name","010-1234-5678" };
	const int UNDER_CAPACITY = 1;
	const int CAPACITY_PER_HOUR = 3;
};



TEST_F(BookingItem, ReservationAvailableOnlyOnTimeNotAvailableIfNotOnTime) {
	//arrage
	tm notOnTheHour = getTime(2021, 3, 26, 9, 5);
	Customer customer{ "Fake name","010-1234-5678" };
	Schedule* schedule = new Schedule{ notOnTheHour, 1, customer };
	BookingScheduler bookingSceduler{ 3 };

	//act
	EXPECT_THROW(bookingSceduler.addSchedule(schedule), std::runtime_error);

	//assert
	//expected runtime exception
}

TEST_F(BookingItem, ReservationAvailableOnlyOnTimeAvailableIfOnTime) {
	//arrage
	tm onTheHour = getTime(2021, 3, 26, 9, 0);

	Customer customer{ "Fake name","010-1234-5678" };
	Schedule* schedule = new Schedule{ onTheHour, 1, customer };
	BookingScheduler bookingSceduler{ 3 };

	//act
	bookingSceduler.addSchedule(schedule);

	//assert
	EXPECT_EQ(true, bookingSceduler.hasSchedule(schedule));
}



//TEST(BookingSchedulerTest, 예약은_정시에만_가능하다_정시가_아닌경우_예약불가) {
//
//}

//TEST(BookingSchedulerTest, 예약은_정시에만_가능하다_정시인_경우_예약가능) {
//
//}
//
//TEST(BookingSchedulerTest, 시간대별_인원제한이_있다_같은_시간대에_Capacity_초과할_경우_예외발생) {
//
//}
//
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