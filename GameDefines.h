#ifndef GAME_DEFINES
#define GAME_DEFINES
#include <string>
#include <utility>
#include <vector>

enum class Direction {
    North,
    South,
    East,
    West,
    Up,
    Down,
    Invalid
};

static std::vector<std::string> player_commands {
    "'N' (North)",
    "'S' (South)",
    "'E' (East)",
    "'W' (West)",
    "'U' (Up)",
    "'D' (Down)",
    "'I' (Inventory)",
    "'F' (Fight)",
    "'R' (Run)",
    "'P' (Pickup)",
    "'C' (Eat)",
    "'Q' (Quit)",
    "'T' (Tally)"
};

// Below ground rooms
static const std::pair<std::string, std::string> guard_room { "Guardroom", " You are in the prison guardroom, in the basement of the castle.\nThe stairwell ends in this room.\nThere is one other exit, a small hole in the east wall.\nThe air is damp and unpleasant; a chill wind rushes into the room from gaps in the stone at the top of the walls." };
static const std::pair<std::string, std::string> dungeon { "Dungeon", "You are in the dank, dark dungeon.\nThere is a single exit, a small hole in the wall towards the west.\nA hollow, dry chuckle is heard from the guard room." };

// The Ground Floor where the player begins the adventure
static const std::pair<std::string, std::string> entrance { "Entrance", "You are at the entrance to a forbidding-looking stone castle." };
static const std::pair<std::string, std::string> hallway { "Hallway", "You are in the hallway.\nFrom the dust on the ground, you can tell no one has walked here for a long, long time.\nThere is a door to the south, and through windows to the north, you can see a secret herb garden." };
static const std::pair<std::string, std::string> kitchen { "Kitchen", "This is the castle's kitchen.\nThrough windows in the north wall, you can see a secret herb garden.\nIt has been many years since meals were prepared for the monarch and the court in this kitchen.\nA rat scurries across the floor." };
static const std::pair<std::string, std::string> audience_chamber { "Audience Chamber", "This is the audience chamber.\nThe faded tapestries on the wall only hint at the splendor this room once had.\nThere is a window to the west, and by looking to the right through it, you can see the entrance to the castle" };
static const std::pair<std::string, std::string> private_meeting { "Private Meeting", "This is the Monarch's Private Meeting Room.\nThe echo of ancient plotting and wrangling hangs heavy in the musty air.\nThere is a single exit to the south." };
static const std::pair<std::string, std::string> store_room { "Store Room", "You are in the store room, amidst spices, vegetables, and vast sacks of flour and other provisions.\nThe air is thick with spice and curry fumes." };
static const std::pair<std::string, std::string> great_hall { "Great Hall", "You are in the great hall, an L-shaped room.\nThere are two doors in this room.\nThe wood panels are warped and faded.\nAs you stand there, you hear a mouse scamper along the floor behind you...\nbut you see nothing when you whirl around." };
static const std::pair<std::string, std::string> inner_hallway { "Inner Hallway", "This inner hallway contains a door to the north, one to the west, and a circular stairwell passes through the room.\nThe room is small and unfriendly.\nYou can see an ornamental lake through the windows to the south." };
static const std::pair<std::string, std::string> rear_vestibule { "Rear Vestibule", "You are in the rear vestibule.\nThere are windows to the south from which you can see the ornamental lake.\nThere is an exit to the east and one to the north." };

// The Upper Floor
static const std::pair<std::string, std::string> master_bedroom { "Master Bedroom", "You are in the master bedroom on the upper level of the castle.\nLooking down from the window to the west, you can see the entrance to the castle, while the secret herb garden is visible below the north window.\nThere are doors to the east and to the south." };
static const std::pair<std::string, std::string> dressing_room { "Dressing Chamber", "This tiny room on the upper level is the dressing chamber.\nThere is a window to the north, with a view of the herb garden down below. A door leaves to the south.\nYou catch a glimpse of yourself in the mirror hanging on the wall and are shocked at your disheveled appearance." };
static const std::pair<std::string, std::string> chambersmaid_bedroom { "Chambermaid's Bedroom", "Ooooh... you are in the chambermaids' bedroom.\nFaint perfume still hangs in the air.\nThere is an exit to the west and a door to the south." };
static const std::pair<std::string, std::string> treasury { "Treasury", "This room was used as the castle treasury in by-gone years.\nA spider scampers down the wall.\nThere are no windows, just exits." };
static const std::pair<std::string, std::string> upper_hallway { "Upper Hallway", "This is the L-shaped upper hallway.\nA moth flits across near the ceiling.\nTo the north is a door, and there is a stairwell in the hall as well.\nYou can see the lake through the south windows." };
static const std::pair<std::string, std::string> castle_exit { "Castle Exit", "This is the small room outside the castle.\nYou can see the lake through the southern windows." };

#endif // !GAME_DEFINES
