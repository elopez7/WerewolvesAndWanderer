#include "GameState.h"

int main()
{
    GameState game_state {};
    game_state.Init();
    game_state.RunLoop();
    return 0;
}
