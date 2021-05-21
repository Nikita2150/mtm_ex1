#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include <assert.h>

#include "chessSystem.h"
#include "map.h"
#include "tournament.h"
#include "player.h"
#include "node.h"


struct chess_system_t
{
    Map tournaments; // Key - tournament_id, Data - tournament
    Map players;
};

// FUNCTIONS FOR MAPS

//tournamentCopy from "tournament.h"
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
    return copy;
}
//freeDataElement is tournamentDestroy
static void freeInt(MapKeyElement num)
{
    free(num);
}

static int compareInts(MapKeyElement num1, MapKeyElement num2)
{
    return (*(int*)num1 - *(int*)num2);
}
//copyPlayer from player.h (DATA)
//freePlayer from player.h (DATA)

//copyInt from this file (KEY)
//freeInt from this file (KEY)
//compareInt also from this file (KEY)
////////////////////////////////////////////////////////////////




static Player initializePlayer(ChessSystem chess, Map players, int* player_id, bool* is_malloced)
{
    Player player;
    if(!mapContains(players, player_id))
    {
        player = playerCreate();
        if(player == NULL)
        {
            return NULL;
        }
        *is_malloced = true;
    }
    else
    {
        player = mapGet(players, player_id);
    }
    return player;
}

static ChessResult convertMapResult(ChessSystem chess, MapResult map_result)
{
    switch(map_result)
    {
        case MAP_NULL_ARGUMENT:
            return CHESS_NULL_ARGUMENT;
        case MAP_OUT_OF_MEMORY:
            chessDestroy(chess);
            return CHESS_OUT_OF_MEMORY;
        case MAP_SUCCESS:
            return CHESS_SUCCESS;
    }
    return CHESS_SUCCESS;
}

static void updateNewGame(Player first_player_data, Player second_player_data, Winner winner, int play_time)
{
    playerAddGame(first_player_data);
    playerAddGame(second_player_data);
    playerAddTime(first_player_data, play_time);
    playerAddTime(second_player_data, play_time);
    
    if(winner == FIRST_PLAYER)
    {
        playerAddWin(first_player_data);
    }
     else if(winner == SECOND_PLAYER)
     {
         playerAddWin(second_player_data);
     }
    if(winner == DRAW)
    {
        playerAddDraw(first_player_data);
        playerAddDraw(second_player_data);
    }
}

static ChessResult initializeBothPlayers(ChessSystem chess, Map players, int first_player_id, int second_player_id, Player* first_player_data, Player* second_player_data)
{
    //What if one of the players got removed???
    assert(first_player_data != NULL && second_player_data != NULL);
    bool is_malloced_first = false, is_malloced_second = false;
    *first_player_data = initializePlayer(chess, players, &first_player_id, &is_malloced_first);
    if((*first_player_data) == NULL)
    {
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }
    *second_player_data = initializePlayer(chess, players, &second_player_id, &is_malloced_second);
    if((*second_player_data) == NULL)
    {
        if(is_malloced_first)
        {
            playerDestroy(*first_player_data);
        }
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }
    return CHESS_SUCCESS;
}

static void freeNeededPlayers(Map players, int first_player, int second_player, Player first_player_data, Player second_player_data)
{
    if(!mapContains(players, &first_player))
    {
        playerDestroy(first_player_data);
    }
    if(!mapContains(players, &second_player))
    {
        playerDestroy(second_player_data);
    }
}

static void shiftArrayRight(int** array, int index, int last_index)
{
    for(int i = last_index; i > index; i--)
    {
        array[i][0] = array[i-1][0];
        array[i][1] = array[i-1][1];
    }
}

static bool putInArray(int** array, int player_id, int level, int size, int last_added)
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
    if(last_added >= size - 1)
    {
        return false;
    }
    if(last_added == 0)
    {
        array[last_added][0] = player_id;
        array[last_added][1] = level; 
    }
    for(int i = 0; i < last_added; i++)
    {
        if(level > array[i][1])
        {
            shiftArrayRight(array, i, last_added+1);
            array[i][0] = player_id;
            array[i][1] = level;
            return true;
        }
    }
    array[last_added + 1][0] = player_id;
    array[last_added + 1][0] = level;
    return true;

}





//////////////////////
ChessSystem chessCreate()
{
    ChessSystem new_chess_system = malloc(sizeof(*new_chess_system));
    if(new_chess_system == NULL)
    {
        return NULL;
    }

    new_chess_system->tournaments = NULL;
    new_chess_system->players = NULL;

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

ChessResult chessAddTournament(ChessSystem chess, int tournament_id, int max_games_per_player, const char* tournament_location)
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
    if(tournament_location == "")//Is this the only empty string?
    {
        return CHESS_INVALID_LOCATION;
    } //should create a function that checks the location!!!
    if(max_games_per_player <= 0)
    {
        return CHESS_INVALID_MAX_GAMES;
    }
    
    

    if(chess->tournaments == NULL)
    {
        chess->tournaments = mapCreate(tournamentCopy, copyInt, tournamentDestroy, freeInt, compareInts);
    }

    
    

    Tournament new_tournament = tournamentCreate(max_games_per_player, tournament_location);
    if(new_tournament == NULL)
    {
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }
    
    ChessResult insert_tournament_result = convertMapResult(chess, mapPut(chess->tournaments, &tournament_id, new_tournament));
    tournamentDestroy(new_tournament); //because we copied the tournament

    return insert_tournament_result;
        
}


ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player, int second_player, Winner winner, int play_time)
{
    if(chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(tournament_id <= 0 || first_player <= 0 || second_player <= 0 || first_player == second_player)
    {
        return CHESS_INVALID_ID;
    }

    int* tournament_id_key = &tournament_id;  

    if(!mapContains(chess->tournaments, tournament_id_key))
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    Tournament this_tournament = mapGet(chess->tournaments, tournament_id_key);
    if(tournamentHasEnded(this_tournament))
    {
        return CHESS_TOURNAMENT_ENDED;
    }

    
    Player first_player_data, second_player_data;

    
    if(initializeBothPlayers(chess, chess->players, first_player, second_player, &first_player_data, &second_player_data) == CHESS_OUT_OF_MEMORY)
    {
        return CHESS_OUT_OF_MEMORY;
    }

    if(tournamentContains(mapGet(chess->tournaments, this_tournament), first_player, second_player)
        || tournamentContains(mapGet(chess->tournaments, this_tournament), second_player, first_player))
    {
        freeNeededPlayers(chess->players, first_player, second_player, first_player_data, second_player_data);
        return CHESS_GAME_ALREADY_EXISTS;
    }

    if(play_time <= 0)
    {
        freeNeededPlayers(chess->players, first_player, second_player, first_player_data, second_player_data);
        return CHESS_INVALID_PLAY_TIME;
    }

    if(playerGetNumOfGames(first_player_data) >= tournamentGetMaxGamesPerPlayer(this_tournament)
    || playerGetNumOfGames(second_player_data) >= tournamentGetMaxGamesPerPlayer(this_tournament))
    {
        freeNeededPlayers(chess->players, first_player, second_player, first_player_data, second_player_data);
        return CHESS_EXCEEDED_GAMES;
    }

    if(chess->players == NULL)
    {
        chess->players = mapCreate(playerCopy, copyInt, playerDestroy, freeInt, compareInts);
        if(chess->players == NULL)
        {
            freeNeededPlayers(chess->players, first_player, second_player, first_player_data, second_player_data);
            chessDestroy(chess);
            return MAP_OUT_OF_MEMORY;
        }
    }

    updateNewGame(first_player_data, second_player_data, winner, play_time);

    ChessResult insert_player_one_result = convertMapResult(chess,  mapPut(chess->players, &first_player, first_player_data));
    ChessResult insert_player_two_result = convertMapResult(chess,  mapPut(chess->players, &second_player, second_player_data));

    playerDestroy(first_player_data);
    playerDestroy(second_player_data);

    if(insert_player_one_result != CHESS_SUCCESS || insert_player_two_result != CHESS_SUCCESS)
    {
        //In case we added one of them to the map
        mapRemove(chess->players, &first_player);
        mapRemove(chess->players, &second_player);

        //because this error has priority
        if(insert_player_one_result == CHESS_OUT_OF_MEMORY || insert_player_two_result == CHESS_OUT_OF_MEMORY)
        {
            return CHESS_OUT_OF_MEMORY;
        }

        //becuase the error code of the first player has priority
        if(insert_player_one_result != CHESS_SUCCESS)
        {
            return insert_player_one_result;
        }
        return insert_player_two_result;
    }
    
    return convertMapResult(chess, tournamentAddGame(this_tournament, first_player, second_player, winner, play_time));
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

    Node games = tournamentGetGames(this_tournament);

    while(games != NULL)
    {
        int first_player = gameGetFirstPlayer(games);
        int second_player = gameGetFirstPlayer(games);
        //ASSERT: first_player, second_player >= 0

        Player first_player_data = mapGet(chess->players, &first_player);
        Player second_player_data = mapGet(chess->players, &second_player);
        //ASSERT: both aren't NULL

        playerUpdateRemovedGame(first_player_data, gameGetWinner(games), true, gameGetPlayTime(games));
        playerUpdateRemovedGame(second_player_data, gameGetWinner(games), false, gameGetPlayTime(games));

        tournamentRemoveFirstGame(this_tournament);

        games = tournamentGetGames(this_tournament);
    }

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
        int temp_tournament_winner_id = tournamentGetWinnerId(tournament_iterator);

        if(temp_tournament_winner_id == TOURNAMENT_IN_PROGRESS)
        {
            Node game_iterator = tournamentGetGames(tournament_iterator);

            while(game_iterator != NULL)
            {
                int temp_first_id = gameGetFirstPlayer(game_iterator);
                int temp_second_id = gameGetSecondPlayer(game_iterator);

                if(temp_first_id == player_id)
                {
                    gameSetFirstPlayer(game_iterator, DELETED_PLAYER);
                    if(gameGetWinner(game_iterator) == FIRST_PLAYER || gameGetWinner(game_iterator) == DRAW)
                    {
                        playerAddWin(mapGet(chess->players, &temp_second_id)); //If exists - will add, else won't do anything
                        gameSetWinner(game_iterator, SECOND_PLAYER);
                        if(gameGetWinner(game_iterator) == DRAW)
                        {
                            playerSubtractDraw(mapGet(chess->players, &temp_second_id));
                        }
                    }
                }
                else if(temp_second_id == player_id)
                {
                    gameSetSecondPlayer(game_iterator, DELETED_PLAYER);
                    if(gameGetWinner(game_iterator) == SECOND_PLAYER || gameGetWinner(game_iterator) == DRAW)
                    {
                        playerAddWin(mapGet(chess->players, &temp_first_id)); //If exists - will add, else won't do anything
                        gameSetWinner(game_iterator, FIRST_PLAYER);
                        if(gameGetWinner(game_iterator) == DRAW)
                        {
                            playerSubtractDraw(mapGet(chess->players, &temp_first_id));
                        }
                    }
                }
                game_iterator = nodeGetNext(game_iterator);
            }
        }
        free(tournament_iterator_id); // because mapGetFirst/mapGetNext create a copy of the real data
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

    if(tournamentGetGames(this_tournament) == NULL)
    {
        return CHESS_NO_GAMES;
    }
    
    Node games_list = tournamentGetGames(this_tournament);

    //What if there are no games yet?
    Map tournament_players = mapCreate(playerCopy, copyInt, playerDestroy, freeInt, compareInts);
    if(tournament_players == NULL)
    {
        return CHESS_OUT_OF_MEMORY;
    }

    while(games_list != NULL)
    {
        //Put/update both players of the game into tournament_players
        Player first_player_data, second_player_data;
        int first_player = gameGetFirstPlayer(games_list), second_player = gameGetSecondPlayer(games_list);
        if(initializeBothPlayers(chess, tournament_players, first_player, second_player, &first_player_data, &second_player_data) == CHESS_OUT_OF_MEMORY)
        {
            return CHESS_OUT_OF_MEMORY;
        }
        updateNewGame(first_player_data, second_player_data, gameGetWinner(games_list), gameGetPlayTime(games_list));

        ChessResult insert_player_one_result = convertMapResult(chess,  mapPut(chess->players, first_player, first_player_data));
        ChessResult insert_player_two_result = convertMapResult(chess,  mapPut(chess->players, second_player, second_player_data));

        //They have been copied in mapPut
        playerDestroy(first_player_data);
        playerDestroy(second_player_data);

        if(insert_player_one_result != CHESS_SUCCESS || insert_player_two_result != CHESS_SUCCESS)
        {
            //because this error has priority
            if(insert_player_one_result == CHESS_OUT_OF_MEMORY || insert_player_two_result == CHESS_OUT_OF_MEMORY)
            {
                return CHESS_OUT_OF_MEMORY;
            }

            //becuase the error code of the first player has priority
            if(insert_player_one_result != CHESS_SUCCESS)
            {
                return insert_player_one_result;
            }
            return insert_player_two_result;
        }      
        games_list = nodeGetNext(games_list);  
    }

    //Need to get the scores of the players and calculate who won:
    
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

    if(playerGetNumOfGames(player_data) == 0)
    {
        return 0;
    }

    retrun (playerGetPlayTime(player_data) * 1.0) / playerGetNumOfGames(player_data);
}


ChessResult chessSavePlayersLevels (ChessSystem chess, FILE* file)
{
    if(chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    
    int num_of_players = mapGetSize(chess->players);
    int* player_id_and_level[2] = malloc(sizeof(num_of_players));
    if(player_id_and_level == NULL)
    {
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }
    int* player_iterator_id = mapGetFirst(chess->players);
    int counter = 0;
    while(player_iterator_id != NULL)
    {
        Player current_player = mapGet(chess->players, player_iterator_id);
        if(playerGetNumOfGames(current_player) != 0)
        {
            if(!putInArray(player_id_and_level, *player_iterator_id, playerGetLevel(current_player), num_of_players, counter++))
            {
                return CHESS_SAVE_FAILURE;
            }
        }

        free(player_iterator_id);
        player_iterator_id = mapGetNext(chess->players);
    }

    for(int i = 0; i < counter; i++)
    {
        if(fprintf(file, "%d %d\n", player_id_and_level[i][0], player_id_and_level[i][1]) == EOF)
        {
            return CHESS_SAVE_FAILURE;
        }
    }
    free(player_id_and_level);
    return CHESS_SUCCESS;
}

ChessResult chessSaveTournamentStatistics (ChessSystem chess, char* path_file)
{
    if(chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    bool all_in_progress = true;
    for(int* tournament_iterator_id = mapGetFirst(chess->tournaments); tournament_iterator_id != NULL; tournament_iterator_id = mapGetNext(chess->tournaments))
    {
        if(tournamentHasEnded(mapGet(chess->tournaments, tournament_iterator_id)))
        {
            all_in_progress= false;
        }
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
    for(int* tournament_iterator_id = mapGetFirst(chess->tournaments); tournament_iterator_id != NULL; tournament_iterator_id = mapGetNext(chess->tournaments))
    {
        Tournament tournament_iterator = mapGet(chess->tournaments, tournament_iterator_id);
        if(tournamentHasEnded(tournament_iterator))
        {
            if(fprintf(stream, "")
        }
    }
    
    
    
}
