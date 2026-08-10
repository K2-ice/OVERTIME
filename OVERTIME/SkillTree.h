#pragma once

#include <vector>

#include "SkillNode.h"
#include "Player.h"
#include "WeaponInventory.h"

using EngineL::Player;

class SkillTree {

public:

	SkillTree();

	void BuildTree();

	void Buy(int index,
		Player& player,
		WeaponInventory& weaponInventory);

	std::vector<SkillNode>& GetNodes();

private:

	std::vector<SkillNode> nodes;
};