#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

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




static Player initializePlayer(ChessSystem chess, int* player_id)
{
    Player player;
    if(!mapContains(chess->players, player_id))
    {
        player = playerCreate();
        if(player == NULL)
        {
            return NULL;
        }
    }
    else
    {
        player = mapGet(chess->players, player_id);
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

    if(tournament_id < 0)
    {
        return CHESS_INVALID_ID;
    }
    if(tournament_location == "")//Is this the only empty string?
    {
        return CHESS_INVALID_LOCATION;
    }
    

    if(chess->tournaments == NULL)
    {
        chess->tournaments = mapCreate(tournamentCopy, copyInt, tournamentDestroy, freeInt, compareInts);
    }

    int* tournament_id_key = &tournament_id;
    
    if(mapContains(chess->tournaments, tournament_id_key))
    {
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }

    Tournament new_tournament = tournamentCreate(max_games_per_player, tournament_location);
    if(new_tournament == NULL)
    {
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }
    
    ChessResult insert_tournament_result = convertMapResult(chess, mapPut(chess->tournaments, tournament_id_key, new_tournament));
    tournamentDestroy(new_tournament); //because we copied the tournament

    return insert_tournament_result;
        
}


ChessResult chessAddGame(ChessSystem chess, int tournament_id, int first_player, int second_player, Winner winner, int play_time)
{
    if(chess == NULL)
    {
        return CHESS_NULL_ARGUMENT;
    }
    if(tournament_id < 0 || first_player < 0 || second_player < 0 || first_player == second_player)
    {
        return CHESS_INVALID_ID;
    }

    int* tournament_id_key = &tournament_id;  

    if(!mapContains(chess->tournaments, tournament_id_key))
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    Tournament this_tournament = mapGet(chess->tournaments, tournament_id_key);

    
    Player first_player_data, second_player_data;

    int* first_player_id = &first_player;
    
    first_player_data = initializePlayer(chess, first_player_id);
    if(first_player_data == NULL)
    {
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }

    //NOT SURE ABOUT THE PRIORITY OF CHESS_EXCEEDED_GAMES
    if(playerNumOfGames(first_player_data) >= tournamentGetMaxGamesPerPlayer(this_tournament))
    {
        return CHESS_EXCEEDED_GAMES;
    }
    int* second_player_id = &second_player;
    
    second_player_data = initializePlayer(chess, second_player_id);
    if(second_player_data == NULL)
    {
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }

    if(playerNumOfGames(second_player_data) >= tournamentGetMaxGamesPerPlayer(this_tournament))
    {
        return CHESS_EXCEEDED_GAMES;
    }

    if(!(playerGetIsOut(second_player_data) || playerGetIsOut(first_player_data)))
    {
        if(tournamentContains(mapGet(chess->tournaments, this_tournament), first_player, second_player)
            || tournamentContains(mapGet(chess->tournaments, this_tournament), second_player, first_player))
        {
            return CHESS_GAME_ALREADY_EXISTS;
        }
    }
    

    if(play_time <= 0)
    {
        return CHESS_INVALID_PLAY_TIME;
    }

    if(chess->players == NULL)
    {
        chess->players = mapCreate(playerCopy, copyInt, playerDestroy, freeInt, compareInts);
        if(chess->players == NULL)
        {
            chessDestroy(chess);
            return MAP_OUT_OF_MEMORY;
        }
    }



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
    ChessResult insert_player_one_result = convertMapResult(chess,  mapPut(chess->players, first_player_id, first_player_data));
    ChessResult insert_player_two_result = convertMapResult(chess,  mapPut(chess->players, second_player_id, second_player_data));

    playerDestroy(first_player_data);
    playerDestroy(second_player_data);

    if(insert_player_one_result != CHESS_SUCCESS || insert_player_two_result != CHESS_SUCCESS)
    {
        //In case we added one of them to the map
        mapRemove(chess->players, first_player_id);
        mapRemove(chess->players, second_player_id);

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

    Node games = getGamesOfTournament(this_tournament);

    while(games != NULL)
    {
        int first_player = gameGetFirstPlayer(games);
        int second_player = gameGetFirstPlayer(games);
        //ASSERT: first_player, second_player >= 0

        Player first_player_data = mapGet(chess->players, &first_player);
        Player second_player_data = mapGet(chess->players, &second_player);
        //ASSERT: both aren't NULL

        updateRemovedGame(first_player_data, gameGetWinner(games), true, gameGetPlayTime(games));
        updateremovedGame(second_player_data, gameGetWinner(games), false, gameGetPlayTime(games));

        tournamentRemoveFirstGame(this_tournament);

        games = getGamesOfTournament(this_tournament);
    }

    mapRemove(chess->tournaments, &tournament_id);

    return CHESS_SUCCESS;
}