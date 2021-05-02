#pragma once
#include <ecm.h>
#include <string>
#include <SFML/Graphics/Text.hpp>

class PlayerComponent : public Component {
public:
	PlayerComponent() = delete;

	explicit PlayerComponent(Entity* p, std::string name, double currency);

	void update(double dt) override;

	void render() override;

	~PlayerComponent() override = default;

	void SetName(const std::string& str);

	std::string& getName();

	void SetCurrency(const double cur);

	double& getCurrency();

	void SetInventory(const std::map<std::string, int> inv);

	std::map<std::string, int>& getInventory();

	void changeItem(const std::string& key, const int& value);

	int getItem(const std::string& key);

	void baseInventory();
	
protected:

	std::string _name;
	double _currency;
	std::map<std::string, int> _inventory;

};
