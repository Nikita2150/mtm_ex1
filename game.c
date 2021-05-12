#include <stdio.h>
#include "game.h"
#include <stdlib.h>

struct game_t{

    int first_player;
    int second_player;

    Winner winner;

    int play_time;
};

Game gameCreate(int first_player, int second_player, Winner winner, int play_time)
{
    Game game = malloc(sizeof(*game));
    if(game == NULL)
    {
        return NULL;
    }
    game->first_player = first_player;
    game->second_player = second_player;

    game->winner = winner;

    game->play_time = play_time;

    return game;
}

void gameDestroy(Game game)
{
    if(game != NULL)
    {
        free(game);
    }
}