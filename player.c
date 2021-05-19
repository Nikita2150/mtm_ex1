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
    if(player != NULL)
    {
        player->num_of_games++;
    }
}

void playerAddWin(Player player)
{
    if(player != NULL)
    {
        player->num_of_wins++;
    }
}
void playerAddDraw(Player player)
{
    if(player != NULL)
    {
        player->num_of_draws++;
    } 
}

void playerAddTime(Player player, int time)
{
    if(player != NULL)
    {
        player->play_time += time;
    }
}

bool playerGetIsOut(Player player)
{
    if(player != NULL)
    {
        return player->is_out;
    }
}

int playerNumOfGames(Player player)
{
    if(player != NULL)
    {
        return player->num_of_games;
    }
}

void updateRemovedGame(Player player, Winner winner, bool first, int play_time)
{
    if(player == NULL)
    {
        return;
    }
    player->num_of_games--;
    player->play_time -= play_time;
    if(winner == DRAW)
    {
        player->num_of_draws--;
    }
    else if(winner == FIRST_PLAYER && first)
    {
        player->num_of_wins--;
    }
    else if(winner == SECOND_PLAYER && !first)
    {
        player->num_of_wins--;
    }
}

void playerSubtractDraw(Player player)
{
    if(player != NULL)
    {
        player->num_of_draws--;
    }
}

int playerPlayTime(Player player)
{
    if(player == NULL)
    {
        return -1;
    }
    return player->num_of_games;
}