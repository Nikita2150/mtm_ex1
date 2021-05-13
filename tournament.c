#include "stdlib.h"
#include "tournament.h"
#include "node.h"
#include "map.h"

struct tournament_t
{
    int max_games_per_player;
    const char* tournament_location;
    
    Node games;
    Map players;
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
    new_tournament->players = NULL;
    return new_tournament;
}

void tournamentDestroy(Tournament tournament)
{
    if(tournament != NULL)
    {
        nodeDestroy(tournament->games);
        mapDestroy(tournament->players);
        free(tournament);
    }
}

Tournament copyTournament(Tournament tournament)
{
    if(tournament == NULL)
    {
        return NULL;
    }
    Tournament new_tournament = tournamentCreate(tournament->max_games_per_player, tournament->tournament_location);
    return new_tournament; //If = NULL, we'll return NULL
}
