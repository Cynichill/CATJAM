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

    //Create values for each item, each item gives different stats

     _givenStats = {0.0f, 4.0f, 10.0f, -5.0f, 0.0f, 1.0f, 0.0f, 0.0f };
    _items["CannedCatFood"] = std::make_pair("res/sprites/cannedCatFood.png", _givenStats);

    _itemType["CannedCatFood"] = "Food";

    _givenStats = { 0.0f, 5.0f, 7.0f, -2.0f, 1.0f, 0.0f, 1.0f, 0.0f };
    _items["Tuna"] = std::make_pair("res/sprites/Tuna.png", _givenStats);

    _itemType["Tuna"] = "Food";

    _givenStats = { 0.0f, 4.0f, 7.0f, -2.0f, 0.0f, 0.0f, 2.0f, 0.0f };
    _items["Salmon"] = std::make_pair("res/sprites/Salmon.png", _givenStats);

    _itemType["Salmon"] = "Food";

    _givenStats = { 0.0f, 8.0f, 2.0f, -10.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    _items["Catnip"] = std::make_pair("res/sprites/Catnip.png", _givenStats);

    _itemType["Catnip"] = "Food";

    _givenStats = { 0.0f, 3.0f, 8.0f, -4.0f, 0.0f, 1.0f, 1.0f, 0.0f };
    _items["DryCatMix"] = std::make_pair("res/sprites/DryCatMix.png", _givenStats);

    _itemType["DryCatMix"] = "Food";

    _givenStats = { 0.0f, 3.0f, 9.0f, -8.0f, 1.0f, 1.0f, 0.0f, 0.0f };
    _items["WetCatMix"] = std::make_pair("res/sprites/WetCatMix.png", _givenStats);

    _itemType["WetCatMix"] = "Food";

    _givenStats = { 0.0f, 1.0f, 1.0f, 100.0f, 1.0f, 1.0f, 1.0f, 0.0f };
    _items["CatBath"] = std::make_pair("res/sprites/catBath.png", _givenStats);

    _itemType["CatBath"] = "Toy";

};

std::string ItemComponent::GetSpriteLocation() {  return _items[_key].first; }

std::string ItemComponent::GetType() { return _itemType[_key]; }

std::string ItemComponent::GetKey() { return _key; }

std::vector<float> ItemComponent::GetStats() { return _items[_key].second; }