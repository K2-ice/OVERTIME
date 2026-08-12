#include "pch.h"
#include "ObjectiveManager.h"

ObjectiveManager::ObjectiveManager()
{
	reset();
}

void ObjectiveManager::reset()
{
	objectives.clear();

	objectives.push_back({ "Survivre 30 secondes", ObjectiveType::SurviveTime, 30.f, false });
	objectives.push_back({ "Survivre 1 minute", ObjectiveType::SurviveTime, 60.f, false });
	objectives.push_back({ "Tuer 10 ennemis", ObjectiveType::KillEnemies, 10.f, false });
	objectives.push_back({ "Tuer 25 ennemis", ObjectiveType::KillEnemies, 25.f, false });
	objectives.push_back({ "Recolter 50 souls", ObjectiveType::CollectSouls, 50.f, false });
	objectives.push_back({ "Recolter 150 souls", ObjectiveType::CollectSouls, 150.f, false });
}

void ObjectiveManager::update(float runTime, int kills, int souls){
	for (int i = 0; i < objectives.size(); i++)	{
		Objective& objective = objectives[i];

		if (objective.completed)
			continue;

		if (objective.type == ObjectiveType::SurviveTime && runTime >= objective.target)
		{
			objective.completed = true;
		}
		else if (objective.type == ObjectiveType::KillEnemies && kills >= objective.target)
		{
			objective.completed = true;
		}
		else if (objective.type == ObjectiveType::CollectSouls && souls >= objective.target)
		{
			objective.completed = true;
		}
	}
}

const std::vector<Objective>& ObjectiveManager::getObjectives() const
{
	return objectives;
}