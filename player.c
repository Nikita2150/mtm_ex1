#include "player.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "map.h"

#define WINS_LEVEL_MULTIPLIER 6
#define LOSSES_LEVEL_MULTIPLIER -10
#define DRAWS_LEVEL_MULTIPLIER 2

#define WINS_SCORE_MULTIPLIER 2
#define LOSSES_SCORE_MULTIPLIER 0
#define DRAW_SCORE_MULTIPLIER 1



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

void playerDestroy(Player player)
{
    if(player != NULL)
    {
        free(player);
    }
}

void playerFree(MapDataElement player)
{
    if(player != NULL)
    {
        playerDestroy((Player) player);
    }
}

MapDataElement playerCopy(MapDataElement player)
{
    if(player == NULL)
    {
        return NULL;
    }
    Player this_player = (Player) player;
    Player new_player = malloc(sizeof(*new_player));
    if(new_player == NULL)
    {
        return NULL;
    }
    
    new_player->num_of_draws = this_player->num_of_draws;
    new_player->num_of_games = this_player->num_of_games;
    new_player->num_of_wins = this_player->num_of_wins;
    new_player->play_time = this_player->play_time;

    return (MapDataElement) new_player;    
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

int playerGetScore(Player player)
{
    assert(player != NULL);
    if(player->num_of_games == 0)
    {
        return 0;
    }
    return ((player->num_of_wins * WINS_SCORE_MULTIPLIER + player->num_of_draws * DRAW_SCORE_MULTIPLIER +
             (playerGetNumOfLosses(player) * LOSSES_SCORE_MULTIPLIER)) / player->num_of_games);
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

int playerGetLevel(Player player)
{
    assert(player!=NULL);
    int x = WINS_LEVEL_MULTIPLIER*playerGetNumOfWins(player) + LOSSES_LEVEL_MULTIPLIER*playerGetNumOfLosses(player) + DRAWS_LEVEL_MULTIPLIER*playerGetNumOfDraws(player);
    int n = playerGetNumOfGames(player);
    if(n == 0)
    {
        return 0;
    }
    return x/n;
}

bool sameWinner(Player current_winner, int current_winner_id, Player current_player, int current_player_id)
{
    assert(current_winner != NULL && current_player != NULL);
    if(playerGetNumOfLosses(current_winner) == playerGetNumOfLosses(current_player))
    {
        if(current_winner->num_of_wins == current_player->num_of_wins)
        {
            return current_winner_id < current_player_id;
        }
        return (current_winner->num_of_wins > current_player->num_of_wins) ? true : false;
    }
    return (playerGetNumOfLosses(current_winner) < playerGetNumOfLosses(current_player)) ? true : false;
}