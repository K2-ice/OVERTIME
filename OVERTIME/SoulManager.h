#pragma once

#include <vector>
#include "Soul.h"

namespace EngineL
{
	class SoulManager	{

	public:

		std::vector<Soul*>& getSouls();

	private:

		std::vector<Soul*> souls;
	};
}