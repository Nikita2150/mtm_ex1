#include <stdio.h>
#include "game.h"
#include <stdlib.h>
#include "player.h"
#include "chessSystem.h"



struct game_t
{
	bool player_deleted_after_end[NUM_OF_PLAYERS];
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

    game->player_deleted_after_end[FIRST_PLAYER] = false;
    game->player_deleted_after_end[SECOND_PLAYER] = false;

    return game;
}

void gameDestroy(Game game)
{
    if(game != NULL)
    {
        free(game);
    }
}

Game gameCopy(Game game)
{
    if(game == NULL)
    {
        return NULL;
    }
    Game new_game = malloc(sizeof(*new_game));
    if(new_game == NULL)
    {
        return NULL;
    }
    new_game->first_player = game->first_player;
    new_game->play_time = game->play_time;
    new_game->second_player = game->second_player;
    new_game->winner = game->winner;

    new_game->player_deleted_after_end[FIRST_PLAYER] = game->player_deleted_after_end[FIRST_PLAYER];
    new_game->player_deleted_after_end[SECOND_PLAYER] = game->player_deleted_after_end[SECOND_PLAYER];

    return new_game;
}

void gameAreNewPlayers(Game current_game, int first_player, int second_player, bool* new_first_player, 
                        bool* new_second_player)
{
    if(first_player == current_game->first_player || first_player == current_game->second_player)
    {
        *new_first_player = false;
    }
    if(second_player == current_game->first_player || second_player == current_game->second_player)
    {
        *new_second_player = false;
    }
}

void gameUpdateWhenRemovingPlayer(Map players, Game current_game, int current_player_id)
{
    Winner winner = current_game->winner;
    int first_player = current_game->first_player, second_player = current_game->second_player;
    playerUpdateWhenRemoving(players, winner, current_player_id, first_player, second_player);
    if(first_player == current_player_id)
	{
        current_game->first_player = DELETED_PLAYER;
		if(winner != SECOND_PLAYER)
		{
            current_game->winner = SECOND_PLAYER;
		}
	}
	else if(second_player == current_player_id)
    {
        current_game->second_player = DELETED_PLAYER;
        if(winner != FIRST_PLAYER)
        {
            current_game->winner = FIRST_PLAYER;
        }
    }
}

GameResult gameAddPlayersToMap(Game game, Map tournament_players)
{
    Player first_player_data, second_player_data;
    int first_player = game->first_player, second_player = game->second_player;

    if(initializeBothPlayers(tournament_players, first_player, second_player, &first_player_data,
                                &second_player_data) == PLAYER_OUT_OF_MEMORY)
    {
        return GAME_OUT_OF_MEMORY;
    }

    updateNewGame(first_player_data, second_player_data, game->winner, game->play_time);
    MapResult insert_player_one_result = MAP_SUCCESS, insert_player_two_result = MAP_SUCCESS;
    if(first_player_data != NULL)
    {
        insert_player_one_result = mapPut(tournament_players, &first_player, first_player_data);
        if(insert_player_one_result == MAP_OUT_OF_MEMORY)
        {
            playerDestroy(first_player_data);
            playerDestroy(second_player_data);
            return GAME_OUT_OF_MEMORY;
        }
        /* ELSE WE GOT MAP_SUCCESS BECAUSE OF HOW WE CALL THE FUNCTION */
    }
    playerDestroy(first_player_data); //We don't need it anymore
	assert(insert_player_one_result == MAP_SUCCESS);
    if(second_player_data != NULL)
    {
        insert_player_two_result = mapPut(tournament_players, &second_player, second_player_data);
        if(insert_player_two_result == MAP_OUT_OF_MEMORY)
        {
            playerDestroy(second_player_data);
            return GAME_OUT_OF_MEMORY;
        }
    }
    assert(insert_player_two_result == MAP_SUCCESS);
	playerDestroy(second_player_data);
    return GAME_SUCCESS;
}

void gameTournamentRemoveUpdatePlayers(Game game, Map players)
{
    if(game != NULL)
    {
	    int first_player = game->first_player, second_player = game->second_player;
    	if(!game->player_deleted_after_end[FIRST_PLAYER] && first_player > 0)
	    {

		    Player first_player_data = mapGet(players, &first_player);
		    playerUpdateRemovedGame(first_player_data, game->winner, true, game->play_time);
	    }
    	if(!game->player_deleted_after_end[SECOND_PLAYER] && second_player > 0)
	    {
		    Player second_player_data = mapGet(players, &second_player);
		    playerUpdateRemovedGame(second_player_data, game->winner, false, game->play_time);
	    }
    }
}

bool gameContains(Game game, int first_player, int second_player)
{
    if(game == NULL)
    {
        return false;
    }
    return (game->first_player == first_player && game->second_player == second_player);
}

int gameGetPlayTime(Game game)
{
    if(game == NULL)
    {
        return 0;
    }
    return game->play_time;
}

bool gamePlayerIsPlaying(Game game, int first_player)
{
    if(game == NULL)
    {
        return false;
    }
    if(game->first_player == first_player || game->second_player == first_player)
    {
        return true;
    }
    return false;
}

void gameUpdateRemovedPlayersAfterEnd(Game game, int player_id)
{
	if(game == NULL)
	{
		return;
	}
	if(game->first_player == player_id)
	{
		game->player_deleted_after_end[FIRST_PLAYER] = true;
	}
	else if(game->second_player == player_id)
	{
		game->player_deleted_after_end[SECOND_PLAYER] = true;
	}
}