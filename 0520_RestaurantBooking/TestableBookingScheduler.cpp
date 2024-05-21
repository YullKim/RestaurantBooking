#include "BookingScheduler.cpp"

class TestableBookingScheduler : public BookingScheduler {
public:
    TestableBookingScheduler(int capacityPerHour, tm dateTime) :
        BookingScheduler{ capacityPerHour }, 
        dataTime{dateTime} {
    
    }
    time_t getNow() override {
        return mktime(&dataTime);
    }

private:
    tm dataTime;
};