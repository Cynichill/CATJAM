#pragma once
#include <ecm.h>
#include <SFML/Graphics/Text.hpp>
#include <string>

class ItemComponent : public Component {
public:
	ItemComponent() = delete;

	explicit ItemComponent(Entity* p, std::string key);

	void update(double dt) override;

	void render() override;

	void ItemComponent::baseItems();

	std::string ItemComponent::GetSpriteLocation();

	std::vector<float> ItemComponent::GetStats();

	std::string ItemComponent::GetType();

	
	std::string ItemComponent::GetKey();

	~ItemComponent() override = default;

protected:

	std::string _spriteLocation;
	std::vector<float> _givenStats = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
	std::map<std::string, std::pair<std::string, std::vector<float>>> _items;
	std::map<std::string, std::string> _itemType;
	std::string _key;
};
