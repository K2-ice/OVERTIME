#pragma once
#include "Entity.h"
#include <string>

namespace EngineL
{
	class WeaponPickup : public Entity
	{
	public:
		WeaponPickup(float x, float y, const std::string& weaponId);

		void update(float deltaTime) override;

		const std::string& getWeaponId() const;

	private:
		std::string weaponId;
	};
}