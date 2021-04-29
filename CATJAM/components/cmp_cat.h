#pragma once

#include <SFML/Graphics/Text.hpp>
#include <ecm.h>

class CatComponent : public Component {
public:
	CatComponent() = delete;

	explicit CatComponent(Entity* p, std::string type, std::string name, std::string sex, std::string faveFood, int age);

	void update(double dt) override;

	void render() override;

	~CatComponent() override = default;

	void SetType(const std::string& str);

	std::string& getType();

	void SetName(const std::string& str);

	std::string& getName();

	void SetSex(const std::string& str);

	std::string& getSex();

	void SetFaveFood(const std::string& str);

	std::string& getFaveFood();

	void SetAge(const int);

	int& getAge();

	void SetHealth(const float);

	float& getHealth();

	void SetMood(const float);

	float& getMood();

	void SetHunger(const float);

	float& getHunger();

	void SetCleanliness(const float);

	float& getCleanliness();

	void SetAgility(const float);

	float& getAgility();

	void SetPower(const float);

	float& getPower();

	void SetStamina(const float);

	float& getStamina();

	void SetBond(const float);

	float& getBond();

protected:

	float _stats[8] = { 100.0f, 50.0f, 100.0f, 100.0f, 1.0f, 1.0f, 1.0f, 0.0f };

	std::string _type;
	std::string _name;
	std::string _sex;
	std::string _faveFood;
	int _age;
};
