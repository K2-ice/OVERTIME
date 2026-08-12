#pragma once
#include <vector>
#include "Objective.h"

class ObjectiveManager {
public:
	ObjectiveManager();

	void update(float runTime, int kills, int souls);
	void reset();

	const std::vector<Objective>& getObjectives() const;

private:
	std::vector<Objective> objectives;
};