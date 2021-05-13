#include "chessSystem.h"
#include "map.h"
#include "tournament.h"
#include "player.h"


struct chess_system_t
{
    Map tournaments; // Key - tournament_id, Data - tournament
    Map players;
};

// FUNCTIONS FOR MAP

//copyTournament from "tournament.h"
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
//

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
        tournamentDestroy(chess->tournaments);
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
        chess->tournaments = mapCreate(copyTournament, copyInt, tournamentDestroy, freeInt, compareInts);
    }

    int* tour_id = malloc(sizeof(*tour_id));
    if(tour_id == NULL)
    {
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY; 
    }
    
    *tour_id = tournament_id;
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
    int* first_player_id = malloc(sizeof(*first_player_id));
    Player first_player_d, second_player_d;
    if(first_player_id == NULL)
    {
        chessDestroy(chess);
        return CHESS_OUT_OF_MEMORY;
    }
    if(mapContains(chess->players, first_player_id))
    {
        
    }
}