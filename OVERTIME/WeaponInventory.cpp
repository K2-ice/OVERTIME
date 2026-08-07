#include "pch.h"
#include "WeaponInventory.h"

#include <cstdlib>

WeaponInventory::WeaponInventory()
    : pistol(WeaponStats{ "Pistolet", 3.f, 10.f, 1, 0.f, 12, 1.2f, -1 })
    , mitraillette(WeaponStats{ "Mitraillette", 10.f, 6.f, 1, 0.f, 30, 1.8f, 90 })
    , shotgun(WeaponStats{ "Shotgun", 1.f, 8.f, 5, 30.f, 6, 1.5f, 24 })
{
    reset(false);
}

void WeaponInventory::reset(bool hasSecondWeaponSlot)
{
    weaponSlots.clear();
    weaponSlots.push_back(&pistol);

    if (hasSecondWeaponSlot)
    {
        weaponSlots.push_back(nullptr);
    }

    currentWeaponIndex = 0;
}

EngineL::Weapon* WeaponInventory::getCurrentWeapon() const
{
    return weaponSlots[currentWeaponIndex];
}

bool WeaponInventory::isInInventory(EngineL::Weapon* weapon) const
{
    for (EngineL::Weapon* slot : weaponSlots)
    {
        if (slot == weapon)
            return true;
    }

    return false;
}

void WeaponInventory::equipPickup(EngineL::Weapon* weapon)
{
    for (size_t i = 0; i < weaponSlots.size(); i++)
    {
        if (weaponSlots[i] == nullptr)
        {
            weaponSlots[i] = weapon;
            currentWeaponIndex = static_cast<int>(i);
            return;
        }
    }

    weaponSlots[currentWeaponIndex] = weapon;
}

void WeaponInventory::handleSwitch(EngineL::InputManager& input, bool hasSecondWeaponSlot)
{
    if (!hasSecondWeaponSlot)
        return;

    if (input.isKeyJustPressed(sf::Keyboard::Key::Num1) && weaponSlots[0] != nullptr)
        currentWeaponIndex = 0;

    if (weaponSlots.size() > 1 && input.isKeyJustPressed(sf::Keyboard::Key::Num2) && weaponSlots[1] != nullptr)
        currentWeaponIndex = 1;
}

EngineL::Weapon* WeaponInventory::getWeaponById(const std::string& id)
{
    if (id == "shotgun")
        return &shotgun;

    if (id == "mitraillette")
        return &mitraillette;

    if (id == "pistol")
        return &pistol;

    return nullptr;
}

bool WeaponInventory::rollChance(float chance) const
{
    float roll = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    return roll < chance;
}

EngineL::WeaponPickup* WeaponInventory::tryDropWeapon(float x, float y)
{
    std::vector<std::string> availableIds;

    if (!isInInventory(&shotgun))
        availableIds.push_back("shotgun");

    if (!isInInventory(&mitraillette))
        availableIds.push_back("mitraillette");

    if (availableIds.empty())
        return nullptr;

    if (!rollChance(weaponDropChance))
        return nullptr;

    int index = rand() % static_cast<int>(availableIds.size());

    return new EngineL::WeaponPickup(x, y, availableIds[index]);
}