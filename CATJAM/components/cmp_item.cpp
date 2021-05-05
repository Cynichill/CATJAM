#include "cmp_item.h"
#include <system_renderer.h>
#include <system_resources.h>

void ItemComponent::update(double dt) {}

void ItemComponent::render() {}

//Constructor
ItemComponent::ItemComponent(Entity* p, std::string key)
    : Component(p), _key(key) {}


//Getters and Setters

void ItemComponent::baseItems() {

     _givenStats = {5.0f, 10.0f, 20.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.5f };
    _items["CannedCatFood"] = std::make_pair("res/sprites/cannedCatFood.png", _givenStats);

    _givenStats = { 8.0f, 20.0f, 10.0f, -2.0f, 1.0f, 0.0f, 1.0f, 0.5f };
    _items["Tuna"] = std::make_pair("res/sprites/Tuna.png", _givenStats);

    _givenStats = { 8.0f, 15.0f, 10.0f, -2.0f, 0.0f, 0.0f, 2.0f, 0.5f };
    _items["Salmon"] = std::make_pair("res/sprites/Salmon.png", _givenStats);

    _givenStats = { 0.0f, 30.0f, 2.0f, -10.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    _items["Catnip"] = std::make_pair("res/sprites/Catnip.png", _givenStats);

    _givenStats = { 5.0f, 5.0f, 20.0f, -4.0f, 0.0f, 1.0f, 1.0f, 0.5f };
    _items["DryCatMix"] = std::make_pair("res/sprites/DryCatMix.png", _givenStats);

    _givenStats = { 5.0f, 5.0f, 20.0f, -8.0f, 1.0f, 1.0f, 0.0f, 0.5f };
    _items["WetCatMix"] = std::make_pair("res/sprites/WetCatMix.png", _givenStats);

    _givenStats = { 2.0f, 2.0f, 8.0f, -10.0f, 1.0f, 1.0f, 1.0f, 0.5f };
    _items["MysteryMeat"] = std::make_pair("res/sprites/MysteryMeat.png", _givenStats);

};

std::string ItemComponent::GetSpriteLocation(std::string key) {  return _items[key].first; }

std::string ItemComponent::GetKey() { return _key; }

std::vector<float> ItemComponent::GetStats(std::string key) { return _items[key].second; }