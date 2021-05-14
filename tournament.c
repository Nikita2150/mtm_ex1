#include "stdlib.h"
#include "tournament.h"
#include "node.h"
#include "map.h"

struct tournament_t
{
    int max_games_per_player;
    const char* tournament_location;
    
    Node games;
};

Tournament tournamentCreate(int max_games_per_player, const char* tournament_location)
{
    Tournament new_tournament = malloc(sizeof(*new_tournament));
    if(new_tournament == NULL)
    {
        return NULL;
    }

    new_tournament->max_games_per_player = max_games_per_player;
    new_tournament->tournament_location = tournament_location;

    new_tournament->games = NULL; //Empty games node
    return new_tournament;
}

void tournamentDestroy(Tournament tournament)
{
    if(tournament != NULL)
    {
        nodeDestroy(tournament->games);
        free(tournament);
    }
}

Tournament tournamentCopy(Tournament tournament)
{
    if(tournament == NULL)
    {
        return NULL;
    }
    Tournament new_tournament = tournamentCreate(tournament->max_games_per_player, tournament->tournament_location);
    return new_tournament; //If = NULL, we'll return NULL
}

bool tournamentContains(Tournament tournament, int first_player, int second_player)
{
    if(tournament == NULL)
    {
        return false;
    }
    return nodeContains(tournament->games, first_player, second_player);
}

MapResult tournamentAddGame(Tournament tournament, int first_player, int second_player, Winner winner, int play_time)
{
    if(tournament == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    Game new_game = gameCreate(first_player, second_player, winner, play_time);
    if(new_game == NULL)
    {
        return MAP_OUT_OF_MEMORY;
    }
    Node new_games_list = nodeAdd(tournament->games, new_game);
    if(new_games_list == NULL)
    {
        return MAP_OUT_OF_MEMORY;
    }
    tournament->games = new_games_list;
    return MAP_SUCCESS;
}

int tournamentGetMaxGamesPerPlayer(Tournament tournament)
{
    if(tournament == NULL)
    {
        return -1;
    }
    return tournament->max_games_per_player;
}

Node getGamesOfTournament(Tournament tournament)
{
    if(tournament == NULL)
    {
        return NULL;
    }
    return tournament->games;
}

void tournamentRemoveFirstGame(Tournament tournament)
{
    if(tournament != NULL)
    {
        tournament->games = removeFirstNode(tournament->games);
    }
}