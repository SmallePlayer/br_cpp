#include <chrono>
#include <thread>
#include "br_time.h"

void delay_seconds(int sec)
{
    std::this_thread::sleep_for(std::chrono::seconds(sec));
}

void delay_microseconds(int usec)
{
    std::this_thread::sleep_for(std::chrono::microseconds(usec));
}

void delay_milliseconds(int sec)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(sec));
}
