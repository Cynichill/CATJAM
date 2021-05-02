#pragma once

#include <ecm.h>
#include <chrono>

class ClockComponent : public Component {
public:
	ClockComponent() = delete;

	explicit ClockComponent(Entity* p);

	void update(double dt) override;

	void render() override;

	void SetTime(std::string str, int hr, int minute, std::string ampm);

	int getHour();
	
	int getMinutes();

	std::string& getTime();

	~ClockComponent() override = default;

protected:

	int _hour = 0;
	int _minute = 0;
	bool _ampm = false;
	bool _tPassed = false;
	bool _resetTime = true;
	std::string _time = "00:00 AM";
	std::string _ap;

	std::chrono::time_point<std::chrono::system_clock> _start;

};
