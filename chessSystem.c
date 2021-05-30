#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <string.h>
#include <assert.h>

#include "chessSystem.h"
#include "map.h"
#include "tournament.h"
#include "player.h"
#include "node.h"

#define ID_AND_LEVEL 2

#define FIRST_CAPITAL_LETTER 'A'
#define LAST_CAPITAL_LETTER 'Z'
#define FIRST_SMALL_LETTER 'a'
#define LAST_SMALL_LETTER 'z'
#define SPACE_CHAR ' '


struct chess_system_t
{
    Map tournaments; // Key - tournament_id, Data - tournament
    Map players;
};

// FUNCTIONS FOR MAPS

static MapDataElement tournamentDataCopy(MapDataElement tournament)
{
	return (MapDataElement) tournamentCopy((Tournament) tournament);
}
static MapKeyElement copyInt(MapKeyElement num)
{
    if(num == NULL)
    {
        return NULL;
    }

    int* copy = malloc(sizeof(*copy));
    if(copy == NULL)
    {
        return NULL;
    }
    *copy = *(int *)num;
    return (MapKeyElement) copy;
}
static void tournamentDataFree(MapDataElement tournament)
{
    if(tournament != NULL)
    {
        tournamentDestroy((Tournament) tournament);
    }
}

static void freeInt(MapKeyElement num)
{
    free(num);
}

static int compareInts(MapKeyElement num1, MapKeyElement num2)
{

    return (*(int*)num1 - *(int*)num2);
}

static MapDataElement playerDataCopy(MapDataElement player)
{
	return (MapDataElement) playerCopy((Player) player);
}

static void playerDataFree(MapDataElement player)
{
    if(player != NULL)
    {
        playerDestroy((Player) player);
    }
}
////////////////////////////////////////////////////////////////

//Helping functions:

static ChessResult convertMapResult(ChessSystem chess, MapResult map_result)
{
    switch(map_result)
    {
        case MAP_NULL_ARGUMENT:
            return CHESS_NULL_ARGUMENT;
        case MAP_OUT_OF_MEMORY:
            return CHESS_OUT_OF_MEMORY;
        case MAP_SUCCESS:
            return CHESS_SUCCESS;
        default: //Won't happen, but the compiler wants it
            return CHESS_SUCCESS;
    }
    return CHESS_SUCCESS;
}

static void shiftArrayRight(double** array, int index, int last_index)
{
    for(int i = last_index; i > index; i--)
    {
        array[i][0] = array[i-1][0];
        array[i][1] = array[i-1][1];
    }
}

static bool putInArray(double** array, int player_id, double level, int size, int current_size)
{
    if(array == NULL)
    {
        return false;
    }
    if(player_id < 0)
    {
        return false;
    }
    if(size == 0)
    {
        return true;
    }
    if(current_size >= size)
    {
        return false;
    }
    if(current_size == 0)
    {
        array[current_size][0] = player_id;
        array[current_size][1] = level;
        return true;
    }
    for(int i = 0; i < current_size; i++)
    {
        if(level > array[i][1])
        {
            shiftArrayRight(array, i, current_size);
            array[i][0] = player_id;
            array[i][1] = level;
            return true;
        }
    }
    array[current_size][0] = player_id;
    array[current_size][1] = level;
    return true;

}
static bool checkValidLocation(const char* location)
{
    assert(location != NULL);
    int length = strlen(location);
    if(length <= 0)
    {
        return false;
    }
    if(location[0] < FIRST_CAPITAL_LETTER || location[0] > LAST_CAPITAL_LETTER)
    {
        return false;
    }

    for(int i = 1; i < length; i++)
    {
        if((location[i] < FIRST_SMALL_LETTER || location[i] > LAST_SMALL_LETTER)
           && location[i] != SPACE_CHAR)
        {
            return false;
        }
    }
    return true;
}

static Map playersMapForTournament(ChessSystem chess, int tournament_id)
{
	Map tournament_players = mapCreate(playerDataCopy, copyInt, playerDataFree, freeInt, compareInts);
	if(tournament_players == NULL)
	{
		return NULL;
	}
    Tournament this_tournament = mapGet(chess->tournaments, &tournament_id);
    assert(this_tournament != NULL);

	if(tournamentGetPlayersMap(this_tournament, tournament_players) != TOURNAMENT_SUCCESS)
    {
        mapDestroy(tournament_players);
        return NULL;
    }
    return tournament_players;
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////


ChessSystem chessCreate()
{
    ChessSystem new_chess_system = malloc(sizeof(*new_chess_system));
    if(new_chess_system == NULL)
    {
        return NULL;
    }

    new_chess_system->tournaments = mapCreate(tournamentDataCopy, copyInt, tournamentDataFree, freeInt, compareInts);
    if(new_chess_system->tournaments == NULL)
    {
    	chessDestroy(new_chess_system);
    	return NULL;
    }
    new_chess_system->players = mapCreate(playerDataCopy, copyInt, playerDataFree, freeInt, compareInts);
    if(new_chess_system->players == NULL)
    {
    	chessDestroy(new_chess_system);
    	return NULL;
    }

    return new_chess_system;
}

void chessDestroy(ChessSystem chess)
{
    if(chess != NULL)
    {

        mapDestroy(chess->tournaments);
        mapDestroy(chess->players);
        free(chess);
    }
}



ChessResult chessAddTournament(ChessSystem chess, int tournament_id, int max_games_per_player,
							                                        const char* tournament_location)
{
    if(chess == NULL || tournament_location == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    if(tournament_id <= 0)
    {
        return CHESS_INVALID_ID;
    }
    if(mapContains(chess->tournaments, &tournament_id))
    {
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }
    if(!checkValidLocation(tournament_location))
    {
        return CHESS_INVALID_LOCATION;
    }
    if(max_games_per_player <= 0)
    {
        return CHESS_INVALID_MAX_GAMES;
    }


    Tournament new_tournament = tournamentCreate(max_games_per_player, tournament_location);
    if(new_tournament == NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }

    ChessResult insert_tournament_result = convertMapResult(chess, mapPut(chess->tournaments,
																		  &tournament_id, new_tournament));
    tournamentDestroy(new_tournament); //because we copied the tournament
    assert(insert_tournament_result != CHESS_NULL_ARGUMENT);

    return insert_tournament_result;

}



ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player, int
														second_player, Winner winner, int play_time)
{
    if(chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(tournament_id <= 0 || first_player <= 0 || second_player <= 0 || first_player ==second_player)
    {
        return CHESS_INVALID_ID;
    }
    if(!mapContains(chess->tournaments, &tournament_id))
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    Tournament this_tournament = mapGet(chess->tournaments, &tournament_id);
    if(tournamentHasEnded(this_tournament))
    {
        return CHESS_TOURNAMENT_ENDED;
    }

    if(tournamentContains(this_tournament, first_player, second_player)
       || tournamentContains(this_tournament, second_player, first_player))
    {
        return CHESS_GAME_ALREADY_EXISTS;
    }
    if(play_time <= 0)
    {
        return CHESS_INVALID_PLAY_TIME;
    }
	if(tournamentPlayersExceededGames(this_tournament, first_player, second_player))
    {
        return CHESS_EXCEEDED_GAMES;
    }

	Game this_game = gameCreate(first_player, second_player, winner, play_time);
	if(this_game == NULL)
	{
		return CHESS_OUT_OF_MEMORY;
	}
	if(gameAddPlayersToMap(this_game, chess->players) == GAME_OUT_OF_MEMORY)
	{
		gameDestroy(this_game);
		return CHESS_OUT_OF_MEMORY;
	}
	/// So if we removed these players we could add them to the array again.
	bool new_first_player = true, new_second_player = true;
	tournamentAreNewPlayers(this_tournament, first_player, second_player, &new_first_player, &new_second_player);

	ChessResult add_game_result = convertMapResult(chess, tournamentAddGame(this_tournament, this_game,
																	   new_first_player, new_second_player));
    if(add_game_result != CHESS_SUCCESS)
    {
    	assert(add_game_result == CHESS_OUT_OF_MEMORY);
    	gameDestroy(this_game);
    }
    return add_game_result;
	/// adding the players to the set inside tournamentAddGame
}

ChessResult chessRemoveTournament(ChessSystem chess, int tournament_id)
{
    if(chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(tournament_id < 0)
    {
        return CHESS_INVALID_ID;
    }
    if(!mapContains(chess->tournaments, &tournament_id))
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }

    Tournament this_tournament = mapGet(chess->tournaments, &tournament_id);
    tournamentRemoveUpdatePlayers(this_tournament, chess->players);
    mapRemove(chess->tournaments, &tournament_id);

    return CHESS_SUCCESS;
}


ChessResult chessRemovePlayer(ChessSystem chess, int player_id)
{
    if(chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(player_id <= 0)
    {
        return CHESS_INVALID_ID;
    }
    if(!mapContains(chess->players, &player_id))
    {
        return CHESS_PLAYER_NOT_EXIST;
    }
    int* tournament_iterator_id = mapGetFirst(chess->tournaments);
    while(tournament_iterator_id != NULL)
    {
        Tournament tournament_iterator = mapGet(chess->tournaments, tournament_iterator_id);
        assert(tournament_iterator != NULL);
        tournamentUpdateWhenRemovingPlayer(chess->players, tournament_iterator, player_id);

        free(tournament_iterator_id);
        tournament_iterator_id = mapGetNext(chess->tournaments);
    }
    mapRemove(chess->players, &player_id);

    return CHESS_SUCCESS;
}

ChessResult chessEndTournament (ChessSystem chess, int tournament_id)
{
    if(chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(tournament_id < 0)
    {
        return CHESS_INVALID_ID;
    }
    if(!mapContains(chess->tournaments, &tournament_id))
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }


    Tournament this_tournament = mapGet(chess->tournaments, &tournament_id);
    if(tournamentHasEnded(this_tournament))
    {
        return CHESS_TOURNAMENT_ENDED;
    }
    assert(this_tournament != NULL);

    if(tournamentGetNumOfGames(this_tournament) < 1)
    {
        return CHESS_NO_GAMES;
    }

    Map tournament_players = playersMapForTournament(chess, tournament_id);
	if(tournament_players == NULL)
	{
		return CHESS_OUT_OF_MEMORY;
	}

    //Need to get the scores of the players and calculate who won:
    tournamentCalculateWinner(this_tournament, tournament_players);
    mapDestroy(tournament_players);
    return CHESS_SUCCESS;
}

double chessCalculateAveragePlayTime(ChessSystem chess, int player_id, ChessResult* chess_result)
{
    if(chess == NULL)
    {
        *chess_result = CHESS_NULL_ARGUMENT;
        return -1;
    }
    if(player_id < 0)
    {
        *chess_result = CHESS_INVALID_ID;
        return -1;
    }

    if(!mapContains(chess->players, &player_id))
    {
        *chess_result = CHESS_PLAYER_NOT_EXIST;
        return -1;
    }

    *chess_result = CHESS_SUCCESS;

    Player player_data = mapGet(chess->players, &player_id);
    assert(player_data != NULL);
    return playerAverageTime(player_data);
}

///
void freePlayerIdAndLevelArray(double** array, int size)
{
    for(int j = 0; j < size; j++)
    {
        free(array[j]);
    }
    free(array);
}
///

ChessResult chessSavePlayersLevels (ChessSystem chess, FILE* file)
{
    if(chess == NULL || file == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    int num_of_players = mapGetSize(chess->players);
    double** player_id_and_level = malloc(sizeof(*player_id_and_level) * num_of_players);
    if(player_id_and_level == NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }
    for(int i = 0; i < num_of_players; i++)
    {
        player_id_and_level[i] = malloc(sizeof(**player_id_and_level) * ID_AND_LEVEL);
        if(player_id_and_level[i] == NULL)
        {
            freePlayerIdAndLevelArray(player_id_and_level, i);
            return CHESS_OUT_OF_MEMORY;
        }
    }
    int* player_iterator_id = mapGetFirst(chess->players);
    int counter = 0;
    while(player_iterator_id != NULL)
    {
        Player current_player = mapGet(chess->players, player_iterator_id);
        if(playerHasGames(current_player))
        {
            if(!putInArray(player_id_and_level, *player_iterator_id, playerGetLevel(current_player),
						                                                num_of_players, counter++))
            {
                freePlayerIdAndLevelArray(player_id_and_level, num_of_players);
				free(player_iterator_id);
                return CHESS_SAVE_FAILURE;
            }
        }
        free(player_iterator_id);
        player_iterator_id = mapGetNext(chess->players);
    }
    for(int i = 0; i < counter; i++)
    {
        if(fprintf(file, "%d %.2f\n", (int)player_id_and_level[i][0], player_id_and_level[i][1]) == EOF)
        {
            freePlayerIdAndLevelArray(player_id_and_level, num_of_players);
            return CHESS_SAVE_FAILURE;
        }
    }
    freePlayerIdAndLevelArray(player_id_and_level, num_of_players);
    return CHESS_SUCCESS;
}

ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file)
{
    if(chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }

    bool all_in_progress = true;
    int* tournament_iterator_id = mapGetFirst(chess->tournaments);
    while(tournament_iterator_id != NULL)
    {
	    if(tournamentHasEnded(mapGet(chess->tournaments, tournament_iterator_id)))
	    {
		    all_in_progress= false;
		    free(tournament_iterator_id);
		    break;
	    }
	    free(tournament_iterator_id);
	    tournament_iterator_id = mapGetNext(chess->tournaments);
    }

    if(all_in_progress)
    {
        return CHESS_NO_TOURNAMENTS_ENDED;
    }
    FILE* stream  = fopen(path_file, "w");
    if(stream == NULL)
    {
        return CHESS_SAVE_FAILURE;
    }
    tournament_iterator_id = mapGetFirst(chess->tournaments);
    while(tournament_iterator_id != NULL)
    {
	    Tournament tournament_iterator = mapGet(chess->tournaments, tournament_iterator_id);
	    if(tournamentHasEnded(tournament_iterator))
	    {
		    if(!tournamentPrintStatistics(tournament_iterator, stream))
		    {
			    free(tournament_iterator_id);
			    fclose(stream);
			    return CHESS_SAVE_FAILURE;
		    }
	    }
	    free(tournament_iterator_id);
	    tournament_iterator_id = mapGetNext(chess->tournaments);

    }
    fclose(stream);
    return CHESS_SUCCESS;
}

