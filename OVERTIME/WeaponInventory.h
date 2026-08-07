#pragma once

#include <vector>
#include <string>

#include "Weapon.h"
#include "WeaponStats.h"
#include "WeaponPickup.h"
#include "InputManager.h"

class WeaponInventory {

public:

    WeaponInventory();

    void reset(bool hasSecondWeaponSlot);
    void handleSwitch(EngineL::InputManager& input, bool hasSecondWeaponSlot);

    EngineL::Weapon* getCurrentWeapon() const;
    EngineL::Weapon* getWeaponById(const std::string& id);

    bool isInInventory(EngineL::Weapon* weapon) const;
    void equipPickup(EngineL::Weapon* weapon);

    EngineL::WeaponPickup* tryDropWeapon(float x, float y);

private:

    bool rollChance(float chance) const;

    EngineL::Weapon pistol;
    EngineL::Weapon mitraillette;
    EngineL::Weapon shotgun;

    std::vector<EngineL::Weapon*> weaponSlots;
    int currentWeaponIndex = 0;

    float weaponDropChance = 0.2f;
};