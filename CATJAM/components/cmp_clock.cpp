#include "cmp_clock.h"
#include <chrono>
#include <system_renderer.h>
#include <system_resources.h>

void ClockComponent::update(double dt) {


    //If 30 seconds have passed, add 10 minutes to in-universe clock

    //Get current system time at this point
    if (_resetTime) 
    {
        _start = std::chrono::system_clock::now();
        _resetTime = false;
    }

    //Get current system time every frame
    std::chrono::time_point<std::chrono::system_clock> dur = std::chrono::system_clock::now();

    //Find difference between dur and _start
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(dur - _start).count();

    //If 30 seconds have passed since we got _start
    if (seconds >= 30)
    {
        _tPassed = true;
        _resetTime = true;
    }

    //If _tPassed is true, update time
    if (_tPassed) 
    {
        _tPassed = false;
        //Add 10 minutes
        _minute = _minute + 1;
        //If minute = 6, change hour
        if (_minute == 6)
        {
            if (_hour == 11)
            {
                _hour = _hour + 1;
                _minute = 0;
                _ampm = !_ampm;
            }
            else if (_hour == 12)
            {
                _minute = 0;
                _hour = 1;
            }
            else
            {
                _hour = _hour + 1;
                _minute = 0;
            }
        }

        //Sets AM/PM based on ampm
        if (_ampm) 
        {
            _ap = "PM";
        }
        else
        {
            _ap = "AM";
        }

        //Print output of clock to _time in 12 hour clock format
        if (_hour < 10)
        {
            _time = "0" + std::to_string(_hour) + ":" + std::to_string(_minute) + "0" + " " + _ap;
        }
        else
        {
            _time = std::to_string(_hour) + ":" + std::to_string(_minute) + "0" + " " + _ap;
        }
    }
}

void ClockComponent::render() {}


//Constructor
ClockComponent::ClockComponent(Entity* p)
    : Component(p) {}


//Getters and Setters

void ClockComponent::SetTime(std::string str, int hr, int minute, std::string ampm) {

    _time = str;

    _hour = hr;

    _minute = minute;

    if (ampm == "AM")
    {
        _ampm = false;
    }
    else if (ampm == "PM")
    {
        _ampm = true;
    }
}



int ClockComponent::getHour() { return _hour;}

int ClockComponent::getMinutes() { return _minute; }

std::string& ClockComponent::getTime() { return _time; }


