#pragma once

#include <vector>

#include "SkillNode.h"
#include "Player.h"

using EngineL::Player;

class SkillTree
{
public:

	SkillTree();

	void BuildTree();

	void Buy(int index,
		Player& player);

	std::vector<SkillNode>& GetNodes();

private:

	std::vector<SkillNode> nodes;
};