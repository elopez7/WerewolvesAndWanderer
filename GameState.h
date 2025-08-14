#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "Character.h"
#include "Location.h"
#include <memory>
#include <string>
#include <unordered_map>

struct PurchaseableItems {
    int id;
    std::string name;
    int price;
    bool is_food;
};

class GameState {
public:
    GameState();
    ~GameState();
    bool Init();
    void RunLoop();
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();
    void ShutDown();

private:
    void CreateCastle();
    void CreateRooms();
    void AssignExits();
    int GenerateContentValue();
    void HandleActions(char input_actions);
    void HandleMovement(char input_action);
    void DisplayPlayerStatus();
    Direction CharToDirection(char input_action);
    void ProcessInventory();
    void HandleCombatAction(char input_action);
    void PickUpTreasure();
    void EatFood();
    void QuitGame();
    void DisplayScore();

    Character player_;
    std::unordered_map<std::string, Location> game_world_;
    Location playerLocation_;
    bool is_running_;
};

#endif // !GAME_STATE_H
