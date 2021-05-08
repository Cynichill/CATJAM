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
    _items["Fish"] = std::make_pair("res/sprites/Fish.png", _givenStats);

    _itemType["Fish"] = "Food";

    _givenStats = { 0.0f, 4.0f, 7.0f, -2.0f, 0.0f, 0.0f, 2.0f, 0.0f };
    _items["Catnip"] = std::make_pair("res/sprites/Catnip.png", _givenStats);

    _itemType["Catnip"] = "Food";

    _givenStats = { 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f };
    _items["YarnBall"] = std::make_pair("res/sprites/YarnBall.png", _givenStats);

    _itemType["YarnBall"] = "Toy";

    _givenStats = { 0.0f, 12.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f };
    _items["FishPlush"] = std::make_pair("res/sprites/FishPlush.png", _givenStats);

    _itemType["FishPlush"] = "Toy";

    _givenStats = { 0.0f, 6.0f, 1.0f, 100.0f, 1.0f, 1.0f, 1.0f, 0.0f };
    _items["CatBath"] = std::make_pair("res/sprites/catBath.png", _givenStats);

    _itemType["CatBath"] = "Toy";

};

std::string ItemComponent::GetSpriteLocation() {  return _items[_key].first; }

std::string ItemComponent::GetType() { return _itemType[_key]; }

std::string ItemComponent::GetKey() { return _key; }

std::vector<float> ItemComponent::GetStats() { return _items[_key].second; }