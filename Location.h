#ifndef LOCATION_H
#define LOCATION_H

#include "GameDefines.h"
#include <map>
#include <string>
#include <vector>

struct MonsterInfo {
    std::string name {};
    int danger_level {};
};

class Location {
public:
    Location();
    void Initialize(
        const std::string& name,
        const std::string& description,
        const std::vector<std::string>& visible_objects,
        int content_value);
    std::string GetLocationName() const;
    std::string GetLocationDescription() const;
    std::vector<std::string> GetVisibleObjects() const;
    void AddExit(Direction direction, Location& destination);
    bool IsValid();
    std::string GetExit(Direction direction);
    void Describe();
    void SetRoomContentValue(int new_value);
    int GetRoomContentValue() const;
    ~Location();

    static const std::map<int, MonsterInfo> monster_data;

private:
    std::string DirectionToString(Direction direction);
    std::string name_;
    std::string description_;
    std::map<Direction, Location> exits_;
    std::vector<std::string> visible_objects_;
    int room_content_value_;
    bool is_valid_;
};

#endif // !LOCATION_H
