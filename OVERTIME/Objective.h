#pragma once
#include <string>

enum class ObjectiveType {
	SurviveTime,
	KillEnemies,
	CollectSouls
};

struct Objective {
	std::string description;
	ObjectiveType type;
	float target;
	bool completed = false;
};