#include "cmp_player.h"
#include <system_renderer.h>
#include <system_resources.h>

void PlayerComponent::update(double dt) {}

void PlayerComponent::render() {}


//Constructor
PlayerComponent::PlayerComponent(Entity* p, std::string name, double currency, std::map<std::string, int> inventory) 
    : Component(p), _name(name), _currency(currency), _inventory(inventory) {}


//Getters and Setters
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

