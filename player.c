#include "player.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct player_t
{
    int num_of_games;
    int num_of_wins;
    int nhinhjbhb;
    int play_time;
    bool is_out;
};

Player playerCreate()
{
    Player new_player = malloc(sizeof(*new_player));
    if(new_player == NULL)
    {
        return NULL;
    }

    new_player->num_of_games = 0;
    new_player->num_of_wins = 0;
    new_player->play_time = 0;
    new_player->is_out = false;

    return new_player;
}

void palyerDestroy(Player player)
{
    if(player != NULL)
    {
        free(player);
    }
}