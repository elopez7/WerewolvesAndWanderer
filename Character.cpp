#include "Character.h"
#include <format>
#include <iostream>

Character::Character()
    : name_ {}
    , strength_ { 95 }
    , wealth_ { 75 }
    , food_ {}
    , tally_ {}
    , kill_count {}
    , has_sword_ {}
    , has_amulet_ {}
    , has_axe_ {}
    , has_suit_ {}
    , has_light_ {}
    , current_location_ {}
{
}

void Character::InitCharacter(const std::string& name,
    const Location& current_location)
{
    name_ = name;
    current_location_ = current_location;
}

Character::~Character() { }

void Character::SetLocation(Location new_location)
{
    current_location_ = new_location;
}

Location Character::GetCurrentLocation() const
{
    return current_location_;
}

int Character::GetPlayerStrength() const
{
    return strength_;
}

void Character::DecreaseStrength(int amount)
{
    strength_ -= amount;
    if (strength_ < 0) {
        strength_ = 0;
    }
}

void Character::DisplayStrength() const
{
    std::cout << std::format("\x1b[36mCurrent strength: {}\x1b[0m\n", strength_);
}

void Character::DisplayStats() const
{
    std::cout << "\n----------------------------------------\n";
    std::cout << std::format(
        "You have \x1b[36m${}\x1b[0m\n"
        "Your provisions sack holds \x1b[36m{}\x1b[0m units of food\n",
        wealth_, food_);
    std::cout << "\n----------------------------------------\n";

    if (has_suit_) {
        std::cout << "You are wearing armor.\n";
    }

    bool carrying_something { false };
    std::string carried_items { "You are carrying " };

    if (has_axe_) {
        carried_items += "an axe ";
        carrying_something = true;
    }
    if (has_sword_) {
        if (has_axe_)
            carried_items += "and ";
        carried_items += "an sword ";
        carrying_something = true;
    }
    if (has_amulet_) {
        if (has_axe_ || has_sword_)
            carried_items += "and ";
        carried_items += "an amulet ";
        carrying_something = true;
    }

    if (carrying_something) {
        std::cout << carried_items << '\n';
    }
}

int Character::GetWealth() const
{
    return wealth_;
}

void Character::DecreaseWealth(int amount)
{
    wealth_ -= amount;
    if (wealth_ < 0) {
        wealth_ = 0;
    }
}

void Character::IncreaseWealth(int amount)
{
    wealth_ += amount;
}

int Character::GetFood() const
{
    return food_;
}

void Character::IncreaseFood(int amount)
{
    food_ += amount;
}

void Character::EatFood()
{
    food_--;
    strength_++;
}

void Character::LightTourch(bool has_light)
{
    has_light_ = has_light;
}

bool Character::HasLight() const
{
    return has_light_;
}

void Character::EquipSword(bool has_sword)
{
    has_sword_ = has_sword;
}

bool Character::HasSword() const
{
    return has_sword_;
}

void Character::EquipAmulet(bool has_amulet)
{
    has_amulet_ = has_amulet;
}

bool Character::HasAmulet() const
{
    return has_amulet_;
}

void Character::EquipAxe(bool has_axe)
{
    has_axe_ = has_axe;
}

bool Character::HasAxe() const
{
    return has_axe_;
}

void Character::EquipSuit(bool has_suit)
{
    has_suit_ = has_suit;
}

bool Character::HasSuit() const
{
    return has_suit_;
}

int Character::GetKillCount() const
{
    return kill_count;
}

void Character::IncreaseKillCount()
{
    kill_count++;
}

void Character::IncreaseTally()
{
    tally_++;
}

int Character::GetTally() const
{
    return tally_;
}

std::string Character::GetName() const
{
    return name_;
}
