#include "GameState.h"
#include "GameDefines.h"
#include <format>
#include <iostream>
#include <random>
#include <string>

GameState::GameState()
    : player_ {}
    , game_world_ {}
    , playerLocation_ {}
    , is_running_ {}
{
}

GameState::~GameState() { }

bool GameState::Init()
{
    CreateCastle();

    playerLocation_ = game_world_[entrance.first];
    std::cout << "What is your name, explorer? ";
    std::string player_name {};
    std::getline(std::cin, player_name);
    player_.InitCharacter(player_name, playerLocation_);
    DisplayPlayerStatus();
    is_running_ = true;
    return true;
}

void GameState::RunLoop()
{
    while (is_running_) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void GameState::ProcessInput()
{
    std::cout << "-----AVAILABLE COMMANDS-----\x1b[35m\n";
    for (auto& c : player_commands) {
        std::cout << c << '\n';
    }
    std::cout << "\x1b[0mWhat do you want to do?\n";
    std::string command_line {};
    std::getline(std::cin, command_line);
    std::cout << "\x1b[0m\033[2J\033[1;1H";
    if (command_line.empty()) {
        std::cout << "\x1b[31mI don't understand that command.\x1b[0m\n";
        return;
    }

    char action = std::toupper(command_line.at(0));
    HandleActions(action);
}

void GameState::HandleActions(char input_action)
{
    switch (input_action) {
    case 'N': // North
    case 'S': // South
    case 'E': // East
    case 'W': // West
    case 'U': // Up
    case 'D': // Down
        HandleMovement(input_action);
        break;
    case 'I': // Inventory / Provisions
        ProcessInventory();
        break;
    case 'F': // Fight
    case 'R': // Run
        HandleCombatAction(input_action);
        break;
    case 'P': // Pick up Treasure
        PickUpTreasure();
        break;
    case 'C': // Eat Food
        EatFood();
        break;
    case 'Q': // Quit
        QuitGame();
        break;
    case 'T': // Tally Score
        DisplayScore();
        break;
    default:
        std::cout << "\x1b[31mI don't understand that command.\x1b[0m\n";
        break;
    }
}

void GameState::HandleMovement(char input_action)
{
    Direction direction { CharToDirection(input_action) };

    if (direction == Direction::Invalid) {
        std::cout << "\x1b[31mI don't understand that direction\x1b[0m\n";
        return;
    }

    std::string new_location { player_.GetCurrentLocation().GetExit(direction) };

    if (new_location.empty()) {
        std::cout << "\x1b[31mNo exit that way. You cannot move through solid stone.\x1b[0m\n";
        return;
    }

    player_.SetLocation(game_world_[new_location]);
    player_.DecreaseStrength(5);
    if (player_.GetPlayerStrength() <= 0) {
        std::cout << "\x1b[31mYOU HAVE DIED!........\x1b[0m\n";
        QuitGame();
    }
}

void GameState::DisplayPlayerStatus()
{
    player_.DisplayStrength();
    player_.GetCurrentLocation().Describe();
}

Direction GameState::CharToDirection(char input_action)
{
    switch (std::toupper(input_action)) {
    case 'N':
        return Direction::North;
    case 'S':
        return Direction::South;
    case 'E':
        return Direction::East;
    case 'W':
        return Direction::West;
    case 'U':
        return Direction::Up;
    case 'D':
        return Direction::Down;
    default:
        return Direction::Invalid;
    }
}

void GameState::ProcessInventory()
{
    int selection { -1 };

    std::vector<PurchaseableItems> available_items {
        { 1, "Flaming Torch", 15, false },
        { 2, "Axe", 10, false },
        { 3, "Sword", 20, false },
        { 4, "Food", 2, true },
        { 5, "Magic Amulet", 30, false },
        { 6, "Suit of Armor", 50, false }
    };

    while (selection != 0) {
        player_.DisplayStats();

        std::cout << "<-----ITEMS AVAILABLE FOR PURCHASE>-----\x1b[35m\n";
        for (const auto& item : available_items) {
            std::cout << std::format("{}. {} (${})\n", item.id, item.name, item.price);
        }
        std::cout << "'0' Return to adventure\n";
        std::cout << "\x1b[0m----------------------------------------\n";

        std::cout << "\x1b[33mEnter a number to select an option\x1b[0m\n";
        std::cin >> selection;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (selection == 0) {
            std::cout << "Returning to adventure.\n";
            break;
        }

        bool item_found { false };
        for (const auto& item : available_items) {
            if (item.id == selection) {
                item_found = true;

                if (item.is_food) {
                    int food_quantity {};
                    std::cout << std::format("\x1b[34mHow many units of food do you want to buy? (Each unit costs ${}\x1b[0m): ", item.price);
                    std::cin >> food_quantity;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    if (food_quantity > 0) {
                        int total_food_cost { food_quantity * item.price };
                        if (player_.GetWealth() >= total_food_cost) {
                            player_.DecreaseWealth(total_food_cost);
                            player_.IncreaseFood(food_quantity);
                            std::cout << std::format("\x1b[32mAdded {} units of food. You now have {} units.\x1b[0m\n", food_quantity, player_.GetFood());
                            std::cout << std::format("Remaining wealth: $\x1b[32m{}.\x1b[0m\n", player_.GetWealth());
                        } else {
                            std::cout << "Insuficient funds.\n";
                            break;
                        }
                    } else {
                        std::cout << "No food purchased.\n";
                    }
                } else {
                    if (player_.GetWealth() >= item.price) {
                        std::cout << std::format("\x1b[32mYou purchased {} for ${}\x1b[0m\n", item.name, item.price);
                        if (item.id == 1) {
                            if (player_.HasLight()) {
                                std::cout << "\x1b[31mYou already have this item.\x1b[0m\n";
                                continue;
                            }
                            player_.LightTourch(true);
                            player_.DecreaseWealth(item.price);
                        }
                        if (item.id == 2) {
                            if (player_.HasAxe()) {
                                std::cout << "\x1b[31mYou already have this item.\x1b[0m\n";
                                continue;
                            }
                            player_.EquipAxe(true);
                            player_.DecreaseWealth(item.price);
                        }
                        if (item.id == 3) {
                            if (player_.HasSword()) {
                                std::cout << "\x1b[31mYou already have this item.\x1b[0m\n";
                                continue;
                            }
                            player_.EquipSword(true);
                            player_.DecreaseWealth(item.price);
                        }
                        if (item.id == 5) {
                            if (player_.HasAmulet()) {
                                std::cout << "\x1b[31mYou already have this item.\x1b[0m\n";
                                continue;
                            }
                            player_.EquipAmulet(true);
                            player_.DecreaseWealth(item.price);
                        }
                        if (item.id == 6) {
                            if (player_.HasSuit()) {
                                std::cout << "\x1b[31mYou already have this item.\x1b[0m\n";
                                continue;
                            }
                            player_.EquipSuit(true);
                            player_.DecreaseWealth(item.price);
                        }
                        std::cout << "Remaining wealth: \x1b[32m$" << player_.GetWealth() << ".\x1b[0m\n";
                    }
                }
            }
        }
        if (!item_found && selection != 0) {
            std::cout << "\x1b[31mInvalid selection. Please enter a valid item number or 0 to exit.\x1b[0m\n";
        }
    }
}

void GameState::HandleCombatAction(char input_action)
{
    int monster_value { player_.GetCurrentLocation().GetRoomContentValue() };

    if (monster_value >= 0) {
        std::cout << "\x1b[31mThere is nothing to fight here.\x1b[0m\n";
        return;
    }

    static std::random_device rd {};
    static std::mt19937 g { rd() };
    std::uniform_real_distribution<float> rand_dist(0.0f, 1.0f);

    if (input_action == 'R') {
        if (rand_dist(g) > 0.7f) {
            std::cout << "\x1b[32mYOU HAVE SUCCESSFULLY FLED!.\x1b[0m\n";
            Location& current_location_ref { game_world_[player_.GetCurrentLocation().GetLocationName()] };
            current_location_ref.Initialize(current_location_ref.GetLocationName(), current_location_ref.GetLocationDescription(), current_location_ref.GetVisibleObjects(), 0);
            player_.SetLocation(current_location_ref);
            return;
        } else {
            std::cout << "\x1b[31mYOU MUST STAND AND FIGHT!\x1b[0m\n";
            input_action = 'F';
        }
    }

    auto it_monster { Location::monster_data.find(monster_value) };
    if (it_monster == Location::monster_data.end()) {
        std::cout << "\x1b[31mError: Monster data not found for this room\x1b[0m\n";
        return;
    }

    std::string monster_name { it_monster->second.name };
    int monster_danger_level { it_monster->second.danger_level };

    if (input_action == 'F') {
        if (player_.HasSuit()) {
            std::cout << "\x1b[34mYOUR ARMOR INCREASES YOUR CHANCES OF SUCCESS!\x1b[0m\n";
            monster_danger_level = static_cast<int>(monster_danger_level * 0.75f);
        }

        // Weapon Adjustments
        if (player_.HasAxe() && player_.HasSword()) {
            std::cout << "\x1b[33mWHICH WEAPON?\n";
            std::cout << "1 - AXE\n";
            std::cout << "2 - SWORD\x1b[0m\n";
            int weapon_choice {};
            std::cin >> weapon_choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (weapon_choice == 1) {
                monster_danger_level = static_cast<int>(monster_danger_level * 0.8f);
            } else if (weapon_choice == 2) {
                monster_danger_level = static_cast<int>(monster_danger_level * 0.75f);
            } else {
                std::cout << "\x1b[31mInvalid weapon choice. Fighting with bare hands.\x1b[0m\n";
                monster_danger_level = static_cast<int>(monster_danger_level * 1.2f);
            }
        } else if (player_.HasAxe()) {
            std::cout << "\x1b[34mYOU HAVE ONLY AN AXE TO FIGHT WITH\x1b[0m\n";
            monster_danger_level = static_cast<int>(monster_danger_level * 0.8f);
        } else if (player_.HasSword()) {
            std::cout << "\x1b[34mYOU MUST FIGHT WITH YOUR SWORD\x1b[0m\n";
            monster_danger_level = static_cast<int>(monster_danger_level * 0.75f);
        } else {
            std::cout << "\x1b[31mYOU HAVE NO WEAPONS! YOU MUST FIGHT WITH YOUR BARE HANDS\x1b[0m\n";
            monster_danger_level = static_cast<int>(monster_danger_level * 1.2f);
        }

        std::cout << "\x1b[33mPRESS ANY KEY TO FIGHT!\x1b[0m\n";
        std::cin.get();

        // Battle loop
        bool battle_continues { true };
        while (battle_continues && player_.GetPlayerStrength() > 0 && monster_danger_level > 0) {
            std::cout << "\n------------------------------------\n";
            if (rand_dist(g) > 0.5f) {
                // Monster attacks (50% chance)
                std::cout << std::format("{} ATTACKS!\n", monster_name);
                if (rand_dist(g) > 0.5f) {
                    // Monster wounds player
                    std::cout << "\x1b[31mTHE MONSTER WOUNDS YOU!\x1b[0m\n";
                    player_.DecreaseStrength(5);
                }
            } else {
                // Player attacks
                std::cout << "YOU ATTACK!\n";
                if (rand_dist(g) > 0.5f) {
                    // Player wounds monster
                    std::cout << "\x1b[32mYOU MANAGE TO WOUND IT!\x1b[0m\n";
                    monster_danger_level = static_cast<int>(monster_danger_level * (5.0f / 6.0f));
                }

                // Item Loss Chances
                if (player_.HasLight() && rand_dist(g) > 0.5f) {
                    std::cout << "\x1b[31mYOUR LIGHT TORCH WAS KNOCKED FROM YOUR HAND!\x1b[0m\n";
                    player_.LightTourch(false);
                }
                if (player_.HasAxe() && rand_dist(g) > 0.5f) {
                    std::cout << "\x1b[31mYOU DROP YOUR AXE IN THE HEAT OF BATTLE!\x1b[0m\n";
                    player_.EquipAxe(false);
                    monster_danger_level = static_cast<int>(monster_danger_level * 1.25f);
                }
                if (player_.HasSword()) {
                    std::cout << "\x1b[31mYOUR SWORD IS KNOCKED FROM YOUR HAND!\x1b[0m\n";
                    player_.EquipSword(false);
                    monster_danger_level = static_cast<int>(monster_danger_level * (4.0f / 3.0f));
                }
            }

            if (rand_dist(g) > 0.95f)
                std::cout << "\x1b[31mAaaaargh!!! RIP! TEAR! RIP!\x1b[0m\n";
            if (rand_dist(g) > 0.9f)
                std::cout << "\x1b[31mYOU WANT TO RUN BUT YOU STAND YOUR GROUND...\x1b[0m\n";
            if (rand_dist(g) > 0.9f)
                std::cout << "\x1b[31m*&%%$#$% $%# !! @ #$$# #$@! #$ $#$\x1b[0m\n";
            if (rand_dist(g) > 0.7f)
                std::cout << "\x1b[33mWILL THIS BE A BATTLE TO THE DEATH?\x1b[0m\n";
            if (rand_dist(g) > 0.7f)
                std::cout << "\x1b[33mHIS EYES FLASH FEARFULLY.\x1b[0m\n";
            if (rand_dist(g) > 0.7f)
                std::cout << "\x1b[33mBLOOD DRIPS FROM HIS CLAWS.\x1b[0m\n";
            if (rand_dist(g) > 0.7f)
                std::cout << "\x1b[33mYOU SMELL THE SULPHUR ON HIS BREATH.\x1b[0m\n";
            if (rand_dist(g) > 0.7f)
                std::cout << "\x1b[33mHE STRIKES WILDLY, MADLY...........\x1b[0m\n";
            if (rand_dist(g) > 0.7f)
                std::cout << "\x1b[33mYOU HAVE NEVER FOUGHT AN OPPONENT LIKE THIS!!\x1b[0m\n";

            if (player_.GetPlayerStrength() <= 0) {
                battle_continues = false;
                std::cout << "\x1b[31mYOU HAVE DIED!........\x1b[0m\n";
                QuitGame();
            } else if (monster_danger_level <= 0 || (rand_dist(g) * 16.0f) > monster_danger_level) {
                std::cout << "\x1b[32mAND YOU MANAGED TO KILL THE " << monster_name << "!\x1b[0m\n";
                player_.IncreaseKillCount();
                battle_continues = false;
            } else if (rand_dist(g) < 0.35f) {
                // Battle Continues to Next Round
            } else {
                // Battle end, but player is defeated (monster did not die and win condition was not met)
                std::cout << "\x1b[31mTHE " << monster_name << " DEFEATED YOU!\x1b[0m\n";
                player_.DecreaseStrength(player_.GetPlayerStrength() / 2);
                battle_continues = false;
            }
        }

        Location& current_location_ref { game_world_[player_.GetCurrentLocation().GetLocationName()] };
        current_location_ref.Initialize(current_location_ref.GetLocationName(), current_location_ref.GetLocationDescription(), current_location_ref.GetVisibleObjects(), 0);
        player_.SetLocation(current_location_ref);
    }
}

void GameState::PickUpTreasure()
{
    int treasure_value { player_.GetCurrentLocation().GetRoomContentValue() };

    if (treasure_value < 10) {
        std::cout << "\x1b[31mThere is no treasure to pick up.\x1b[0m\n";
        return;
    }

    if (!player_.HasLight()) {
        std::cout << "\x1b[31mYou cannot see where it is.\x1b[0m\n";
        return;
    }

    player_.IncreaseWealth(treasure_value);
    std::cout << std::format("\x1b[32mYOU HAVE PICKED UP TREASURE WORTH ${}\x1b[0m\n", treasure_value);
    std::cout << std::format("Your total wealth: \x1b[32m${}\x1b[0m\n", player_.GetWealth());

    Location& current_location_ref = game_world_[player_.GetCurrentLocation().GetLocationName()];
    current_location_ref.SetRoomContentValue(0);
    player_.SetLocation(current_location_ref);
}

void GameState::EatFood()
{
    int current_food { player_.GetFood() };

    if (current_food < 1) {
        std::cout << "\x1b[31mYOU HAVE NO FOOD\x1b[0m\n";
        return;
    }

    std::cout << std::format("YOU HAVE \x1b[36m{}\x1b[0m UNITS OF FOOD\n", current_food);

    int amount_to_eat { -1 };

    while (true) {
        std::cout << "How many do you want to eat? ";
        std::cin >> amount_to_eat;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (std::cin.fail() || amount_to_eat <= 0 || amount_to_eat > current_food) {
            std::cout << "\x1b[31mInvalid amount. Please enter a positive number not exceeding your current food supply.\x1b[0m\n";
            std::cin.clear();
        } else {
            break;
        }
    }

    player_.IncreaseFood(-amount_to_eat); // Use negative amount to DECREASE food
    player_.DecreaseStrength(-5 * amount_to_eat); // Use negative amount to INCREASE strength

    std::cout << std::format("\x1b[32mYOU ATE {} UNITS OF FOOD. Your strength increased by {}.\x1b[0m\n", amount_to_eat, 5 * amount_to_eat);
    player_.DisplayStrength(); // Show updated strength
    std::cout << std::format("You now have \x1b[36m{}\x1b[0m UNITS OF FOOD remaining.\n", player_.GetFood());

    std::cout << "\n----------------------------------------\n";
}

void GameState::QuitGame()
{
    is_running_ = false;
    std::cout << std::format("\x1b[33mExiting Werewolves and Wanderer. Goodbye, {}\x1b[0m\n", player_.GetName());
}

void GameState::DisplayScore()
{
    int score = (3 * player_.GetTally()) + // Player's turns/tally
        (5 * player_.GetPlayerStrength()) + // Player's current strength
        (2 * player_.GetWealth()) + // Player's current wealth
        player_.GetFood() + // Player's remaining food
        (30 * player_.GetKillCount()); // Number of monsters killed

    std::cout << "\n----------------------------------------\n";
    std::cout << std::format("\x1b[32mYOUR SCORE IS: {}\x1b[0m\n", score);
    std::cout << "----------------------------------------\n";
}

void GameState::UpdateGame()
{
    player_.IncreaseTally();
    if (player_.GetCurrentLocation().GetLocationName() == castle_exit.first) {
        std::cout << "\n----------------------------------------\n";
        std::cout << std::format("\x1b[32mYOU HAVE DONE {}! YOU ESCAPED FROM THE CASTLE!\x1b[0m\n", player_.GetName());
        std::cout << "----------------------------------------\n";
    }
}

void GameState::GenerateOutput()
{
    DisplayPlayerStatus();
}

void GameState::ShutDown()
{
}

void GameState::CreateCastle()
{
    CreateRooms();
    AssignExits();
}

void GameState::CreateRooms()
{
    // Below Ground
    game_world_[guard_room.first] = Location();
    game_world_[guard_room.first].Initialize(guard_room.first, guard_room.second, std::vector<std::string> { "", "" }, GenerateContentValue());

    game_world_[dungeon.first] = Location();
    game_world_[dungeon.first].Initialize(
        dungeon.first,
        dungeon.second,
        std::vector<std::string> { "A wooden table", "A tattered map" }, GenerateContentValue());

    // Ground Floor
    game_world_[entrance.first] = Location();
    game_world_[hallway.first] = Location();
    game_world_[kitchen.first] = Location();
    game_world_[audience_chamber.first] = Location();
    game_world_[private_meeting.first] = Location();
    game_world_[store_room.first] = Location();
    game_world_[great_hall.first] = Location();
    game_world_[inner_hallway.first] = Location();
    game_world_[rear_vestibule.first] = Location();

    game_world_[entrance.first].Initialize(entrance.first, entrance.second, {}, 0);
    game_world_[hallway.first].Initialize(hallway.first, hallway.second, {}, GenerateContentValue());
    game_world_[kitchen.first].Initialize(kitchen.first, kitchen.second, {}, GenerateContentValue());
    game_world_[audience_chamber.first].Initialize(audience_chamber.first, audience_chamber.second, {}, GenerateContentValue());
    game_world_[private_meeting.first].Initialize(private_meeting.first, private_meeting.second, {}, 75);
    game_world_[store_room.first].Initialize(store_room.first, store_room.second, {}, GenerateContentValue());
    game_world_[great_hall.first].Initialize(great_hall.first, great_hall.second, {}, GenerateContentValue());
    game_world_[inner_hallway.first].Initialize(inner_hallway.first, inner_hallway.second, {}, GenerateContentValue());
    game_world_[rear_vestibule.first].Initialize(rear_vestibule.first, rear_vestibule.second, {}, 0);

    // Upper Floor
    game_world_[master_bedroom.first] = Location();
    game_world_[dressing_room.first] = Location();
    game_world_[chambersmaid_bedroom.first] = Location();
    game_world_[treasury.first] = Location();
    game_world_[upper_hallway.first] = Location();
    game_world_[castle_exit.first] = Location();

    game_world_[master_bedroom.first].Initialize(master_bedroom.first, master_bedroom.second, {}, GenerateContentValue());
    game_world_[dressing_room.first].Initialize(dressing_room.first, dressing_room.second, {}, GenerateContentValue());
    game_world_[chambersmaid_bedroom.first].Initialize(chambersmaid_bedroom.first, chambersmaid_bedroom.second, {}, GenerateContentValue());
    game_world_[treasury.first].Initialize(treasury.first, treasury.second, {}, 150);
    game_world_[upper_hallway.first].Initialize(upper_hallway.first, upper_hallway.second, {}, GenerateContentValue());
    game_world_[castle_exit.first].Initialize(castle_exit.first, castle_exit.second, {}, GenerateContentValue());
}

void GameState::AssignExits()
{
    // Add Exits
    game_world_[guard_room.first].AddExit(Direction::West, game_world_[dungeon.first]);
    game_world_[guard_room.first].AddExit(Direction::Up,
        game_world_[inner_hallway.first]);
    game_world_[dungeon.first].AddExit(Direction::East, game_world_[guard_room.first]);

    game_world_[entrance.first].AddExit(Direction::East, game_world_[hallway.first]);

    game_world_[hallway.first].AddExit(Direction::West, game_world_[entrance.first]);
    game_world_[hallway.first].AddExit(Direction::South,
        game_world_[audience_chamber.first]);

    game_world_[audience_chamber.first].AddExit(Direction::North,
        game_world_[hallway.first]);
    game_world_[audience_chamber.first].AddExit(Direction::East,
        game_world_[great_hall.first]);
    game_world_[audience_chamber.first].AddExit(Direction::South,
        game_world_[great_hall.first]);

    game_world_[great_hall.first].AddExit(Direction::West,
        game_world_[audience_chamber.first]);
    game_world_[great_hall.first].AddExit(Direction::North,
        game_world_[audience_chamber.first]);
    game_world_[great_hall.first].AddExit(Direction::East,
        game_world_[inner_hallway.first]);

    game_world_[inner_hallway.first].AddExit(Direction::West,
        game_world_[great_hall.first]);
    game_world_[inner_hallway.first].AddExit(Direction::North,
        game_world_[private_meeting.first]);
    game_world_[inner_hallway.first].AddExit(Direction::Up,
        game_world_[upper_hallway.first]);
    game_world_[inner_hallway.first].AddExit(Direction::Down,
        game_world_[guard_room.first]);

    game_world_[private_meeting.first].AddExit(Direction::South,
        game_world_[inner_hallway.first]);

    game_world_[kitchen.first].AddExit(Direction::South,
        game_world_[store_room.first]);

    game_world_[store_room.first].AddExit(Direction::North,
        game_world_[kitchen.first]);
    game_world_[store_room.first].AddExit(Direction::South,
        game_world_[rear_vestibule.first]);

    game_world_[rear_vestibule.first].AddExit(Direction::North,
        game_world_[store_room.first]);
    game_world_[rear_vestibule.first].AddExit(Direction::Up,
        game_world_[castle_exit.first]);

    game_world_[master_bedroom.first].AddExit(Direction::South,
        game_world_[upper_hallway.first]);
    game_world_[master_bedroom.first].AddExit(Direction::East,
        game_world_[chambersmaid_bedroom.first]);

    game_world_[chambersmaid_bedroom.first].AddExit(Direction::West,
        game_world_[master_bedroom.first]);
    game_world_[chambersmaid_bedroom.first].AddExit(Direction::North,
        game_world_[dressing_room.first]);
    game_world_[chambersmaid_bedroom.first].AddExit(Direction::South,
        game_world_[treasury.first]);

    game_world_[dressing_room.first].AddExit(Direction::South,
        game_world_[chambersmaid_bedroom.first]);

    game_world_[upper_hallway.first].AddExit(Direction::North,
        game_world_[master_bedroom.first]);
    game_world_[upper_hallway.first].AddExit(Direction::Down,
        game_world_[inner_hallway.first]);

    game_world_[treasury.first].AddExit(Direction::North,
        game_world_[chambersmaid_bedroom.first]);
    game_world_[treasury.first].AddExit(Direction::East, game_world_[castle_exit.first]);

    game_world_[castle_exit.first].AddExit(Direction::West, game_world_[treasury.first]);
    game_world_[castle_exit.first].AddExit(Direction::Down, game_world_[kitchen.first]);
}

int GameState::GenerateContentValue()
{
    std::random_device rd {};
    std::mt19937 g { rd() };
    std::uniform_int_distribution<> room_distribution(0, 1);
    std::uniform_int_distribution<> money_distribution(10, 109);
    std::uniform_int_distribution<> monster_type_distribution(1, 4);

    int selected_content { room_distribution(g) };
    if (selected_content == 0) {
        return money_distribution(g);
    } else {
        return -monster_type_distribution(g);
    }
}
