#include "SkillTree.h"

SkillTree::SkillTree()
{
	BuildTree();
}

void SkillTree::BuildTree()
{
	nodes.clear();

	nodes.push_back({ SkillType::Time, "Time I", "Increase Time by 2s.", 2.f, 0,  5, 2, 1.25f, true, {300.f,100.f}, {1,13,24,33} });

	nodes.push_back({ SkillType::Damage, "Damage I", "Increase damage by 1.", 1.f, 0, 5, 3, 1.25f, false, {200.f,200.f}, {2,6} });

	nodes.push_back({ SkillType::Damage, "Damage II", "Increase damage by 2.", 2.f, 0, 5, 7, 1.2f, false, {50.f,200.f}, {3,5} });

	nodes.push_back({ SkillType::Damage, "Damage III", "Increase damage by 4.", 4.f, 0, 5, 13, 1.15f, false, {-100.f,200.f}, {4} });

	nodes.push_back({ SkillType::Damage, "Damage IV", "Increase damage by 6.", 6.f, 0, 10, 22, 1.1f, false, {-250.f,200.f}, {} });

	nodes.push_back({ SkillType::UnlockShotgun, "Shotgun", "Can find shotgun during run.", 1.f, 0, 1, 12, 1.f, false, {-100.f,100.f}, {} });

	nodes.push_back({ SkillType::CritChance, "CritChance I", "Increase CritChance by 5%.", 0.05f, 0, 4, 7, 1.25f, false, {200.f,350.f}, {7,9} });

	nodes.push_back({ SkillType::CritChance, "CritChance II", "Increase CritChance by 5%.", 0.05f, 0, 4, 12, 1.2f, false, {50.f,350.f},{8} });

	nodes.push_back({ SkillType::CritChance, "CritChance III", "Increase CritChance by 7%.", 0.07f, 0, 5, 12, 1.15f, false, {50.f,500.f},{} });

	nodes.push_back({ SkillType::CritDamage, "CritDamage I", "Increase CritDamage by 20%.", 0.2f, 0, 5, 7, 1.25f, false, {200.f,500.f}, {10} });

	nodes.push_back({ SkillType::CritDamage, "CritDamage II", "Increase CritDamage by 25%.", 0.25f, 0, 5, 16, 1.2f, false, {200.f,650.f}, {11} });

	nodes.push_back({ SkillType::CritDamage, "CritDamage III", "Increase CritDamage by 30%.", 0.30f, 0, 5, 38, 1.15f, false, {50.f,650.f}, {12} });

	nodes.push_back({ SkillType::CritDamage, "CritDamage IV", "Increase CritDamage by 35%.", 0.35f, 0, 5, 76, 1.1f, false, {-100.f,650.f}, {} });

	nodes.push_back({ SkillType::AttackSpeed, "Attack Speed I", "Increase attack speed by 5%.", 0.05f, 0, 5, 3, 1.25f, false, {400.f,200.f}, {14,16} });

	nodes.push_back({ SkillType::AttackSpeed, "Attack Speed II", "Increase attack speed by 10%.", 0.1f, 0, 4, 7, 1.2f, false,  {550.f,200.f}, {15,19,20} });

	nodes.push_back({ SkillType::AttackSpeed, "Attack Speed III", "Increase attack speed by 20%.", 0.2f, 0, 5, 11, 1.15f, false, {700.f,200.f}, {21} });

	nodes.push_back({ SkillType::ReloadSpeed, "ReloadSpeed I", "Increase reload speed by 5%.", 0.05f, 0, 3, 7, 1.25f, false, {400.f,350.f}, {17} });

	nodes.push_back({ SkillType::ReloadSpeed, "ReloadSpeed II", "Increase reload speed by 5%.", 0.05f, 0, 3, 10, 1.2f, false, {400.f,500.f},{18} });

	nodes.push_back({ SkillType::ReloadSpeed, "ReloadSpeed III", "Increase reload speed by 5%.", 0.05f, 0, 3, 13, 1.15f, false, {400.f,650.f},{} });

	nodes.push_back({ SkillType::WeaponSlot, "Weapon Bag", "Unlocks a second weapon slot,\n" "allow you to switch weapons during a run.", 0.f, 0, 1, 10, 1.f, false, {550.f, 350.f},{} });

	nodes.push_back({ SkillType::UnlockSubmachinegun, "Submachine Gun", "Can find submachine gun during run.", 0.2f, 0, 1, 13, 1.f, false, {700,100.f}, {} });

	nodes.push_back({ SkillType::Capacity, "Capacity I", "Increase ammunition capacity by 2.", 2.f, 0, 5, 12, 1.25f, false, {700.f,350.f}, {22} });

	nodes.push_back({ SkillType::Capacity, "Capacity II", "Increase ammunition capacity by 2.", 2.f, 0, 5, 29, 1.2f, false, {700.f,500.f}, {23} });

	nodes.push_back({ SkillType::Capacity, "Capacity III", "Increase ammunition capacity by 2.", 2.f, 0, 5, 59, 1.15f, false, {550.f,500.f}, {} });

	nodes.push_back({ SkillType::Health, "Health I", "Increase health by 10.", 10.f, 0, 5, 2, 1.25f, false, {200.f,0.f}, {25,28} });

	nodes.push_back({ SkillType::Health, "Health II", "Increase health by 15.", 15.f, 0, 5, 5, 1.2f, false, {50.f,0.f}, {26,31} });

	nodes.push_back({ SkillType::Health, "Health III", "Increase health by 20.", 20.f, 0, 5, 9, 1.15f, false, {-100.f,0.f}, {27} });

	nodes.push_back({ SkillType::Health, "Health IV", "Increase health by 25.", 25.f, 0, 5, 15, 1.1f, false, {-250.f,0.f}, {} });

	nodes.push_back({ SkillType::LifeSteal, "LifeSteal I", "Increase LifeSteal by 1%.", 0.01f, 0, 3, 5, 1.25f, false, {200.f,-150.f}, {29} });

	nodes.push_back({ SkillType::LifeSteal, "LifeSteal II", "Increase LifeSteal by 1%.", 0.01f, 0, 3, 10, 1.20f, false, {200.f,-300.f}, {30} });

	nodes.push_back({ SkillType::LifeSteal, "LifeSteal III", "Increase LifeSteal by 1%.", 0.01f, 0, 3, 13, 1.15f, false, {200.f,-450.f}, {} });

	nodes.push_back({ SkillType::Regen, "Regen I", "Increase Regen by 0.5 per 5s.", 0.5f, 0, 5, 10, 1.25f, false, {50.f,-150.f}, {32} });

	nodes.push_back({ SkillType::Regen, "Regen II", "Increase Regen by 1 per 5s.", 1.f, 0, 5, 23, 1.2f, false, {50.f,-300.f}, {} });

	nodes.push_back({ SkillType::Time, "Time II", "Increase time by 4s.", 4.f, 0, 5, 5, 1.2f, false, {400.f,0.f}, {34,37} });

	nodes.push_back({ SkillType::Time, "Time III", "Increase time by 5s.", 5.f, 0, 6, 9, 1.15f, false, {550.f,0.f}, {35,40} });

	nodes.push_back({ SkillType::Time, "Time IV", "Increase time by 8s.", 5.f, 0, 5, 17, 1.1f, false, {700.f,0.f}, {36} });

	nodes.push_back({ SkillType::Time, "Time V", "Increase time by 10s.", 5.f, 0, 10, 24, 1.05f, false, {850.f,0.f}, {} });

	nodes.push_back({ SkillType::Speed, "Speed I", "Increase speed by 2%.", 1.02f, 0, 5, 5, 1.25f, false, {400.f,-150.f}, {38} });;

	nodes.push_back({ SkillType::Speed, "Speed II", "Increase speed by 4%.", 1.04f, 0, 5, 12, 1.20f, false, {400.f,-300.f}, {39} });

	nodes.push_back({ SkillType::Speed, "Speed III", "Increase speed by 8%.", 1.08f, 0, 5, 24, 1.15f, false, {400.f,-450.f}, {} });

	nodes.push_back({ SkillType::Difficulty, "Difficulty I", "Increase Difficulty to 2\n""2x rewards, but enemies are stronger.", 1.f, 0, 1, 10, 1.f, false, {550.f,-150.f}, {41} });

	nodes.push_back({ SkillType::Difficulty, "Difficulty II", "Increase Difficulty to 3\n""3x rewards, but enemies are stronger.", 1.f, 0, 1, 50, 1.f, false, {550.f,-300.f}, {42} });

	nodes.push_back({ SkillType::Difficulty, "Difficulty III", "Increase Difficulty to 4\n""4x rewards, but enemies are stronger.", 1.f, 0, 1, 100, 1.f, false, {700.f,-300.f}, {} });
}

void SkillTree::Buy(int index, Player& player, WeaponInventory& weaponInventory)
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
		stats.speed *= node.amount;
		break;

	case SkillType::AttackSpeed:
		stats.attackSpeed += node.amount;
		break;

	case SkillType::Time:
		player.increaseMaxTime(node.amount);
		break;

	case SkillType::CritChance:
		stats.critChance += node.amount;
		break;

	case SkillType::CritDamage:
		stats.critDamage += node.amount;
		break;

	case SkillType::LifeSteal:
		stats.lifesteal += node.amount;
		break;

	case SkillType::ReloadSpeed:
		stats.reloadSpeed -= node.amount;
		break;

	case SkillType::Regen:
		stats.regen += node.amount;
		break;

	case SkillType::Capacity:
		weaponInventory.increaseCapacityAll(node.amount, 3 * node.amount);
		break;

	case SkillType::Difficulty:
		stats.difficulty += static_cast<int>(node.amount);
		break;

	case SkillType::WeaponSlot:
		player.unlockSecondWeaponSlot();
		break;

	case SkillType::UnlockShotgun:
		player.GetStats().hasShotgun = true;
		break;

	case SkillType::UnlockSubmachinegun:
		player.GetStats().hasSubmachinegun = true;
		break;

	default:
		break;
	}
}

std::vector<SkillNode>& SkillTree::GetNodes()
{
	return nodes;
}