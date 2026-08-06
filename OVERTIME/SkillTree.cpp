#include "SkillTree.h"

SkillTree::SkillTree()
{
	BuildTree();
}

void SkillTree::BuildTree()
{
	nodes.clear();

	nodes.push_back({
		SkillType::Damage,
		"Damage I",
		"Increase damage by 5.",
		5.f,
		0,
		3,
		10,
		1.25f,
		true,
		{300.f,100.f},
		{1,2}
		});

	nodes.push_back({
		SkillType::Damage,
		"Damage I",
		"Increase damage by 10.",
		10.f,
		0,
		3,
		20,
		1.25f,
		false,
		{200.f,250.f},
		{}
		});

	nodes.push_back({
		SkillType::Damage,
		"Attack Speed I",
		"Increase attack speed by 10%.",
		10.f,
		0,
		3,
		10,
		1.25f,
		false,
		{400.f,250.f},
		{3}
		});
	nodes.push_back({
		SkillType::Damage,
		"Attack Speed II",
		"Increase attack speed by 10%.",
		10.f,
		0,
		5,
		30,
		1.25f,
		false,
		{600.f,250.f},
		{}
		});

	nodes.push_back({
		SkillType::WeaponSlot,
		"Sac d'armes",
		"Debloque un second emplacement d'arme, permettant de changer d'arme au combat.",
		0.f,
		0,
		1,
		15,
		1.f,
		true,
		{500.f, 450.f},
		{}
		});
}

void SkillTree::Buy(int index, Player& player)
{
	SkillNode& node = nodes[index];

	if (!node.unlocked)
		return;

	if (node.level >= node.maxLevel)
		return;

	if (!player.spendSouls(node.cost))
		return;

	node.level++;

	node.cost = static_cast<int>(
		std::round(node.cost * node.priceMultiplier));

	if (node.level == 1)
	{
		for (int child : node.children)
			nodes[child].unlocked = true;
	}

	Stats& stats = player.GetStats();

	switch (node.type)
	{
	case SkillType::Damage:
		stats.damage += node.amount;
		break;

	case SkillType::Health:
		stats.maxHealth += node.amount;
		stats.health += node.amount;
		break;

	case SkillType::Speed:
		stats.speed += node.amount;
		break;

	case SkillType::AttackSpeed:
		stats.attackSpeed *= node.amount;
		break;

	case SkillType::WeaponSlot:
		player.unlockSecondWeaponSlot();
		break;

	default:
		break;
	}
}

std::vector<SkillNode>& SkillTree::GetNodes()
{
	return nodes;
}