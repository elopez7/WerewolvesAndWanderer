#ifndef CHARACTER_H
#define CHARACTER_H

#include "Location.h"
#include <string>
class Character {
public:
    Character();
    void InitCharacter(const std::string& name, const Location& current_location);
    ~Character();
    void SetLocation(Location new_location);
    Location GetCurrentLocation() const;
    int GetPlayerStrength() const;
    void DecreaseStrength(int amount);
    void DisplayStrength() const;
    void DisplayStats() const;

    int GetWealth() const;
    void DecreaseWealth(int amount);
    void IncreaseWealth(int amount);

    int GetFood() const;
    void IncreaseFood(int amount);
    void EatFood();

    void LightTourch(bool has_light);
    bool HasLight() const;

    void EquipSword(bool has_sword);
    bool HasSword() const;

    void EquipAmulet(bool has_amulet);
    bool HasAmulet() const;

    void EquipAxe(bool has_axe);
    bool HasAxe() const;

    void EquipSuit(bool has_suit);
    bool HasSuit() const;

    int GetKillCount() const;
    void IncreaseKillCount();

    void IncreaseTally();
    int GetTally() const;

    std::string GetName() const;

private:
    std::string name_;
    int strength_;
    int wealth_;
    int food_;
    int tally_;
    int kill_count;
    bool has_sword_;
    bool has_amulet_;
    bool has_axe_;
    bool has_suit_;
    bool has_light_;
    Location current_location_;
};

#endif // !CHARACTER_H
