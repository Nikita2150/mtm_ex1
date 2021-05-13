#include "player.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct player_t
{
    int num_of_games;
    int num_of_wins;
    int num_of_draws;
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
    new_player->num_of_draws = 0;
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

Player playerCopy(Player player)
{
    if(player == NULL)
    {
        return NULL;
    }
    Player new_player = malloc(sizeof(*new_player));
    if(new_player == NULL)
    {
        return NULL;
    }
    
    new_player->is_out = player->is_out;
    new_player->num_of_draws = player->num_of_draws;
    new_player->num_of_games = player->num_of_games;
    new_player->num_of_wins = player->num_of_wins;
    new_player->play_time = player->play_time;

    return new_player;    
}


void playerAddGame(Player player)
{
    player->num_of_games++;
}

void playerAddWin(Player player)
{
    player->num_of_wins++;
}
void playerAddDraw(Player player)
{
    player->num_of_draws++;
}

void playerAddTime(Player player, int time)
{
    player->play_time += time;
}

bool playerGetIsOut(Player player)
{
    return player->is_out;
}