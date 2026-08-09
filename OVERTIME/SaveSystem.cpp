#include "pch.h"
#include "SaveSystem.h"

#include <fstream>

namespace
{
	const char* kSaveFilePath = "save.txt";
}

void SaveSystem::save(int souls) {
	std::ofstream file(kSaveFilePath);

	if (file.is_open())
	{
		file << souls;
	}
}

int SaveSystem::load() {

	std::ifstream file(kSaveFilePath);

	if (!file.is_open())
		return 0;

	int souls = 0;
	file >> souls;

	if (file.fail())
		return 0;

	return souls;
}