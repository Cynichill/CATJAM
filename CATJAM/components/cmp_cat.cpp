#include "cmp_cat.h"
#include <system_renderer.h>
#include <system_resources.h>

void CatComponent::update(double dt) {}

void CatComponent::render() {}


//Constructor
CatComponent::CatComponent(Entity* p, std::string type, std::string name, std::string sex, std::string faveFood, int age)
    : Component(p), _type(type), _name(name), _sex(sex), _faveFood(faveFood), _age(age) {}


//Getters and Setters
void CatComponent::SetType(const std::string& str) {
    _type = str;
}

std::string& CatComponent::getType() { return _type; }

void CatComponent::SetName(const std::string& str) {
    _name = str;
}

std::string& CatComponent::getName() { return _name; }

void CatComponent::SetSex(const std::string& str) {
    _sex = str;
}

std::string& CatComponent::getSex() { return _sex; }

void CatComponent::SetFaveFood(const std::string& str) {
    _faveFood = str;
}

std::string& CatComponent::getFaveFood() { return _faveFood; }

void CatComponent::SetAge(const int i) {
    _age = i;
}

int& CatComponent::getAge() { return _age; }


//Getters and Setters for stats
void CatComponent::SetHealth(const float flo) {
    if (flo <= 100.0f && flo > -1.0f)
    {
        _stats[0] = flo;
    }
    else if (flo < 0.0f)
    {
        _stats[0] = 0.0f;
    }
    else if (flo > 100.0f)
    {
        _stats[0] = 100.0f;
    }
}

float& CatComponent::getHealth() { return _stats[0]; }

void CatComponent::SetMood(const float flo) {
    if (flo <= 100.0f && flo > -1.0f)
    {
        _stats[1] = flo;
    }
    else if (flo < 0.0f)
    {
        _stats[1] = 0.0f;
    }
    else if (flo > 100.0f)
    {
        _stats[1] = 100.0f;
    }
}

float& CatComponent::getMood() { return _stats[1]; }

void CatComponent::SetHunger(const float flo) {
    if (flo <= 100.0f && flo > -1.0f)
    {
        _stats[2] = flo;
    }
    else if (flo < 0.0f)
    {
        _stats[2] = 0.0f;
    }
    else if (flo > 100.0f)
    {
        _stats[2] = 100.0f;
    }
}

float& CatComponent::getHunger() { return _stats[2]; }

void CatComponent::SetCleanliness(const float flo) {
    if (flo <= 100.0f && flo > -1.0f)
    {
        _stats[3] = flo;
    }
    else if (flo < 0.0f)
    {
        _stats[3] = 0.0f;
    }
    else if (flo > 100.0f)
    {
        _stats[3] = 100.0f;
    }
}

float& CatComponent::getCleanliness() { return _stats[3]; };


void CatComponent::SetAgility(const float flo) {
    if (flo <= 100.0f && flo > -1.0f)
    {
        _stats[4] = flo;
    }
    else if (flo < 0.0f)
    {
        _stats[4] = 0.0f;
    }
    else if (flo > 100.0f)
    {
        _stats[4] = 100.0f;
    }
}

float& CatComponent::getAgility() { return _stats[4]; }

void CatComponent::SetPower(const float flo) {
    if (flo <= 100.0f && flo > -1.0f)
    {
        _stats[5] = flo;
    }
    else if (flo < 0.0f)
    {
        _stats[5] = 0.0f;
    }
    else if (flo > 100.0f)
    {
        _stats[5] = 100.0f;
    }
}

float& CatComponent::getPower() { return _stats[5]; }

void CatComponent::SetStamina(const float flo) {
    if (flo <= 100.0f && flo > -1.0f)
    {
        _stats[6] = flo;
    }
    else if (flo < 0.0f)
    {
        _stats[6] = 0.0f;
    }
    else if (flo > 100.0f)
    {
        _stats[6] = 100.0f;
    }
}

float& CatComponent::getStamina() { return _stats[6]; }

void CatComponent::SetBond(const float flo) {
    if (flo <= 100.0f && flo > -1.0f)
    {
        _stats[7] = flo;
    }
    else if (flo < 0.0f)
    {
        _stats[7] = 0.0f;
    }
    else if (flo > 100.0f)
    {
        _stats[7] = 100.0f;
    }
}

float& CatComponent::getBond() { return _stats[7]; }

void CatComponent::gainStats(std::vector<float> vec) {

   
    for (int i = 0; i < 8; i++) {

        if (_stats[i] + vec[i] <= 100.0f && _stats[i] + vec[i] > -1.0f)
        {
            _stats[i] += vec[i];
        }
        else if (_stats[i] + vec[i] < 0.0f)
        {
            _stats[i] = 0.0f;
        }
        else if (_stats[i] + vec[i] > 100.0f)
        {
            _stats[i] = 100.0f;
        }

    }
}
