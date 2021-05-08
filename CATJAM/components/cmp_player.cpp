#include "cmp_player.h"
#include <system_renderer.h>
#include <system_resources.h>

void PlayerComponent::update(double dt) {}

void PlayerComponent::render() {}


//Constructor
PlayerComponent::PlayerComponent(Entity* p, std::string name, double currency) 
    : Component(p), _name(name), _currency(currency) {}


//Getters and Setters

void PlayerComponent::baseInventory() {

    //Create default inventory stats
    _inventory["CannedCatFood"] = 0;
    _inventory["Fish"] = 0;
    _inventory["Catnip"] = 0;
    _inventory["YarnBall"] = 0;
    _inventory["FishPlush"] = 0;
    _inventory["CatBath"] = 0;

}
void PlayerComponent::SetName(const std::string& str) {
    _name = str;
}

std::string& PlayerComponent::getName() { return _name; }

void PlayerComponent::SetCurrency(const double cur) {
    _currency = cur;
}

double& PlayerComponent::getCurrency() { return _currency; }

void PlayerComponent::SetInventory(const std::map<std::string, int> inv) {
    _inventory = inv;
}

std::map<std::string, int>& PlayerComponent::getInventory() { return _inventory; }

void PlayerComponent::changeItem(const std::string& key, const int& value) {
    _inventory[key] = value;
}

int PlayerComponent::getItem(const std::string& key) { return _inventory[key]; }

