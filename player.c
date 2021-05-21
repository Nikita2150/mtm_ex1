#include "player.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#define WIN_MULTIPLIER 6
#define LOSSES_MULTIPLIER -10
#define DRAW_MULTIPLIER 2



struct player_t
{
    int num_of_games;
    int num_of_wins;
    int num_of_draws;
    int play_time;
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
    
    new_player->num_of_draws = player->num_of_draws;
    new_player->num_of_games = player->num_of_games;
    new_player->num_of_wins = player->num_of_wins;
    new_player->play_time = player->play_time;

    return new_player;    
}


void playerAddGame(Player player)
{
    assert(player!=NULL);
    player->num_of_games++;
}

void playerAddWin(Player player)
{
    assert(player!=NULL);
    player->num_of_wins++;
}

void playerAddDraw(Player player)
{
    assert(player!=NULL);
    player->num_of_draws++;
    
}

void playerAddTime(Player player, int time)
{
    assert(player!=NULL);
    player->play_time += time;
    
}

int playerGetNumOfGames(Player player)
{
    assert(player!=NULL);
    return player->num_of_games;
    
}

void playerUpdateRemovedGame(Player player, Winner winner, bool first, int play_time)
{
    assert(player!=NULL);
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
    assert(player!=NULL);
    player->num_of_draws--;
    
}

int playerGetPlayTime(Player player)
{
    assert(player!=NULL);
    return player->num_of_games;
}

int playerGetNumOfWins(Player player)
{
    assert(player!=NULL);
    return player->num_of_wins;
}

int playerGetNumOfDraws(Player player)
{
    assert(player!=NULL);
    return player->num_of_draws;
}

int playerGetNumOfLosses(Player player)
{
    assert(player!=NULL);
    return player->num_of_games - player->num_of_draws - player->num_of_wins;
}

int playerGetLevel(PLayer player)
{
    assert(player!=NULL);
    int x = WIN_MULTIPLIER*playerGetNumOfWins(player) + LOSSES_MULTIPLIER*playerGetNumOfLosses(player) + DRAWS_MULTIPLIER*playerGetNumOfDRaws(player);
    int n = playerGetNumOfGames(player);
    return x/n;
}