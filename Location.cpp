#include "Location.h"
#include <format>
#include <iostream>

const std::map<int, MonsterInfo> Location::monster_data = {
    { -1, { "FEROCIOUS WEREWOLF", 5 } },
    { -2, { "FANATICAL FLESHGORGER", 10 } },
    { -3, { "MALOVENTY MALDEMER", 15 } },
    { -4, { "DEVASTATING ICE_DRAGON", 20 } }
};

Location::Location()
    : name_ {}
    , description_ {}
    , exits_ {}
    , visible_objects_ {}
    , room_content_value_ {}
    , is_valid_ { false }
{
}

void Location::Initialize(
    const std::string& name,
    const std::string& description,
    const std::vector<std::string>& visible_objects,
    int content_value)
{
    name_ = name;
    description_ = description;
    visible_objects_ = visible_objects;
    room_content_value_ = content_value;
    is_valid_ = true;
}

std::string Location::GetLocationName() const
{
    return name_;
}

std::string Location::GetLocationDescription() const
{
    return description_;
}

std::vector<std::string> Location::GetVisibleObjects() const
{
    return visible_objects_;
}

void Location::AddExit(Direction direction, Location& destination)
{
    exits_[direction] = destination;
}

bool Location::IsValid()
{
    return is_valid_;
}

std::string Location::GetExit(Direction direction)
{
    auto it { exits_.find(direction) };
    if (it != exits_.end()) {
        return it->second.name_;
    }
    return "";
}

void Location::Describe()
{
    std::cout << std::format("\x1b[33m{}\x1b[0m", description_) << '\n';

    if (!visible_objects_.empty()) {
        std::cout << "You see:\n";
        std::cout << "----------\n";
        for (const auto& obj : visible_objects_) {
            std::cout << obj << '\n';
        }
        std::cout << "----------\n";
    }

    if (!exits_.empty()) {
        std::cout << "Exits:\n";
        std::cout << "==========\x1b[34m\n";
        for (const auto& pair : exits_) {
            std::cout << "To the " << DirectionToString(pair.first) << '\n';
        }
        std::cout << "\x1b[0m==========\n";
    }

    if (room_content_value_ > 9) {
        std::cout << std::format("\x1b[32mTHERE IS A TREASURE HERE WORTH ${}\x1b[0m\n", room_content_value_);
    } else if (room_content_value_ < 1) {
        auto it { monster_data.find(room_content_value_) };
        if (it != monster_data.end()) {
            std::cout << "\x1b[31mDANGER...THERE IS A MONSTER HERE...\n";
            std::cout << std::format("IT IS A {}\n", it->second.name);
            std::cout << std::format("THE DANGER LEVEL IS {}!\x1b[0m\n", it->second.danger_level);
        }
    }
}

void Location::SetRoomContentValue(int new_value)
{
    room_content_value_ = 0;
}

int Location::GetRoomContentValue() const
{
    return room_content_value_;
}

Location::~Location()
{
    exits_.clear();
    visible_objects_.clear();
}

std::string Location::DirectionToString(Direction direction)
{
    switch (direction) {
    case Direction::North:
        return "North";
    case Direction::South:
        return "South";
    case Direction::East:
        return "East";
    case Direction::West:
        return "West";
    case Direction::Down:
        return "Bottom";
    case Direction::Up:
        return "Top";
    }
    return "Unknown";
}
