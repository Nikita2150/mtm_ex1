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

void playerUpdateRemovedGame(Player player, Winner winner, bool first, int play_time)
{
    if(player != NULL)
    {
        player->num_of_games--;
        player->play_time -= play_time;
        if (winner == DRAW) {
            player->num_of_draws--;
        } else if (winner == FIRST_PLAYER && first)
        {
            player->num_of_wins--;
        } else if (winner == SECOND_PLAYER && !first)
        {
            player->num_of_wins--;
        }
    }
}
static int playerGetNumOfLosses(Player player)
{
    assert(player!=NULL);
    return player->num_of_games - player->num_of_draws - player->num_of_wins;
}

double playerGetScore(Player player)
{
    assert(player != NULL);
    if(player->num_of_games == 0)
    {
        return 0;
    }
    return ((player->num_of_wins * WINS_SCORE_MULTIPLIER + player->num_of_draws * DRAW_SCORE_MULTIPLIER +
             (playerGetNumOfLosses(player) * LOSSES_SCORE_MULTIPLIER) * 1.0));
}




double playerGetLevel(Player player)
{
    assert(player!=NULL);
    int x = WINS_LEVEL_MULTIPLIER * player->num_of_wins +
    		LOSSES_LEVEL_MULTIPLIER * playerGetNumOfLosses(player) +
    		DRAWS_LEVEL_MULTIPLIER * player->num_of_draws;
    int n = player->num_of_games;
    if(n == 0)
    {
        return 0;
    }
    return (x*1.0)/n;
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

void updateNewGame(Player first_player_data, Player second_player_data, Winner winner, int play_time)
{
    if(first_player_data != NULL)
    {
        first_player_data->num_of_games++;
        first_player_data->play_time += play_time;
    }
    if(second_player_data != NULL)
    {
        second_player_data->num_of_games++;
        second_player_data->play_time += play_time;
    }

    if(winner == FIRST_PLAYER && first_player_data != NULL)
    {
        first_player_data->num_of_wins++;
    }
    else if(winner == SECOND_PLAYER && second_player_data != NULL)
    {
        second_player_data->num_of_wins++;
    }
    if(winner == DRAW)
    {
        if(first_player_data != NULL)
        {
            first_player_data->num_of_draws++;
        }
        if(second_player_data != NULL)
        {
            second_player_data->num_of_draws++;
        }
    }
}

void playerUpdateWhenRemoving(Map players, Winner winner, int current_player_id, int first_player, 
                                    int second_player)
{
    if(first_player == current_player_id)
	{
        if(winner != SECOND_PLAYER)
        {
            Player second_player_data = (Player)mapGet(players, &second_player);
            if(second_player_data == NULL)
            {
                return;
            }
            if(winner == DRAW)
            {
                second_player_data->num_of_draws--;
            }
            second_player_data->num_of_wins++;

        }
    }
    else if(second_player == current_player_id)
    {
        if(winner != FIRST_PLAYER)
        {
            Player first_player_data = (Player)mapGet(players, &first_player);
            if(first_player_data == NULL)
            {
                return;
            }
            if(winner == DRAW)
            {
                first_player_data->num_of_draws--;
            }
            first_player_data->num_of_wins++;
        }
    }
}

Player initializePlayer(Map players, int* player_id)
{
    Player player;
    if(!mapContains(players, player_id))
    {
        player = playerCreate();
    }
    else
    {
        player = playerCopy(mapGet(players, player_id));
    }
    return player; //if NULL we'll sent NULL.
}

PlayerResult initializeBothPlayers(Map players, int first_player_id, int
							second_player_id, Player* first_player_data, Player* second_player_data)
{
    if(first_player_id > 0)
    {
        *first_player_data = initializePlayer(players, &first_player_id);
        if((*first_player_data) == NULL)
        {
            return PLAYER_OUT_OF_MEMORY;
        }
    }
    else
    {
        *first_player_data = NULL;
    }
    if(second_player_id > 0)
    {
        *second_player_data = initializePlayer(players, &second_player_id);
        if((*second_player_data) == NULL)
        {
            playerDestroy(*first_player_data);
            return PLAYER_OUT_OF_MEMORY;
        }
    }
    else
    {
        *second_player_data = NULL;
    }

    return PLAYER_SUCCESS;
}

double playerAverageTime(Player player)
{
    if(player == NULL || player->num_of_games == 0)
    {
        return 0;
    }
    return (player->play_time * 1.0) / player->num_of_games;
}

bool playerHasGames(Player player)
{
    if(player == NULL)
    {
        return false;
    }
    if(player->num_of_games > 0)
    {
        return true;
    }
    return false;
}