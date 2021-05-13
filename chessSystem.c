#include "chessSystem.h"
#include "map.h"
#include "tournament.h"
#include "player.h"


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


//For our usage
static int* intCreate(int num)
{
    int* new_int = malloc(sizeof(*new_int));
    if(new_int == NULL)
    {
        return NULL;
    }
    *new_int = num;
    return new_int;
}


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

    int* tour_id = intCreate(tournament_id);
    
    if(tour_id == NULL)
    {
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY; 
    }
    
    if(mapContains(chess->tournaments, tour_id))
    {
        free(tour_id);
        return CHESS_TOURNAMENT_ALREADY_EXISTS;
    }

    Tournament new_tournament = tournamentCreate(max_games_per_player, tournament_location);
    if(new_tournament == NULL)
    {
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }
    
    switch(mapPut(chess->tournaments, tour_id, new_tournament))
    {
        free(tour_id);
        tournamentDestroy(new_tournament);
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
    int* tournament_id_key = intCreate(tournament_id);
    if(tournament_id_key == NULL)
    {
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }
    
    bool not_exist = mapContains(chess->tournaments, tournament_id_key);
    

    if(!not_exist)
    {
        return CHESS_TOURNAMENT_NOT_EXIST;
    }
    Tournament this_tournament = mapGet(chess->tournaments, tournament_id_key);
    
    free(tournament_id_key);
    tournament_id_key = NULL;
    
        Player first_player_data, second_player_data;

    int* first_player_id = intCreate(first_player);
    if(first_player_id == NULL)
    {
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }
    
    first_player_data = initializePlayer(chess, first_player_id);
    if(first_player_data == NULL)
    {
        free(first_player_id);
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }

    int* second_player_id = intCreate(second_player);
    if(second_player_id == NULL)
    {
        chessDestroy(chess);
        free(first_player_id);
        return CHESS_OUT_OF_MEMORY;
    }
    
    second_player_data = initializePlayer(chess, second_player_id);
    if(second_player_data == NULL)
    {
        free(first_player_id);
        free(second_player_id);
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }

    if(!(playerGetIsOut(second_player_data) || playerGetIsOut(first_player_data)))
    {
        int* tournament_iterator = mapGetFirst(chess->tournaments);
        while(tournament_iterator != NULL)
        {
            if(tournamentContains(mapGet(chess->tournaments, tournament_iterator), first_player, second_player))
            {
                return CHESS_GAME_ALREADY_EXISTS;
            }
            tournament_iterator = mapGetNext(chess->tournaments);
        }
    }
    

    if(play_time < 0)
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

    mapPut(chess->players, first_player_id, first_player_data);
    free(first_player_id);
    mapPut(chess->players, second_player_id, second_player_data);
    free(second_player_id);
    
    switch(tournamentAddGame(this_tournament, first_player, second_player, winner, play_time))
    {
        case MAP_OUT_OF_MEMORY:
            chessDestroy(chess);
            return CHESS_OUT_OF_MEMORY;
        case MAP_SUCCESS:
            return CHESS_SUCCESS;
        
    }
}

