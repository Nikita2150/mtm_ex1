#include <stdlib.h>
#include "tournament.h"
#include "node.h"
#include "map.h"
#include "set.h"



struct tournament_t
{
    int max_games_per_player;
    const char* tournament_location;
    int winner_id;
    Node games;
    Set tournament_players;
};

Tournament tournamentCreate(int max_games_per_player, const char* tournament_location)
{
    Tournament new_tournament = malloc(sizeof(*new_tournament));
    if(new_tournament == NULL)
    {
        return NULL;
    }
    new_tournament->winner_id = TOURNAMENT_IN_PROGRESS;
    new_tournament->max_games_per_player = max_games_per_player;
    new_tournament->tournament_location = tournament_location;

    new_tournament->tournament_players = setCreate();
    if(new_tournament->tournament_players == NULL)
    {
        tournamentDestroy(new_tournament);
        return NULL;
    }
    new_tournament->games = NULL; //Empty games node
    return new_tournament;
}

void tournamentDestroy(Tournament tournament)
{
    if(tournament != NULL)
    {
        nodeDestroy(tournament->games);
        setDestroy(tournament->tournament_players);
        free(tournament);
    }
}
void tournamentFree(MapDataElement tournament)
{
    if(tournament != NULL)
    {
        tournamentDestroy((Tournament) tournament);
    }
}

MapDataElement tournamentCopy(MapDataElement tournament)
{
    if(tournament == NULL)
    {
        return NULL;
    }
    Tournament this_tournament = (Tournament) tournament;
    Tournament new_tournament = tournamentCreate(this_tournament->max_games_per_player, this_tournament->tournament_location);
    if(new_tournament == NULL)
    {
        return NULL;
    }
    new_tournament->games = nodeCopy(this_tournament->games);
    if(new_tournament->games == NULL)
    {
        return NULL;
    }
    new_tournament->winner_id = this_tournament->winner_id;
    new_tournament->tournament_players = setCopy(this_tournament->tournament_players);
    if(new_tournament->tournament_players == NULL)
    {
        return NULL;
    }
    return (MapDataElement) new_tournament; //If = NULL, we'll return NULL
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
    if(setAdd(tournament->tournament_players, first_player) == SET_OUT_OF_MEMORY)
    {
        return MAP_OUT_OF_MEMORY;
    }
    if(setAdd(tournament->tournament_players, second_player) == SET_OUT_OF_MEMORY)
    {
        setRemove(tournament->tournament_players, second_player);
        return MAP_OUT_OF_MEMORY;
    }
    Game new_game = gameCreate(first_player, second_player, winner, play_time);
    if(new_game == NULL)
    {
        setRemove(tournament->tournament_players, first_player);
        setRemove(tournament->tournament_players, second_player);
        return MAP_OUT_OF_MEMORY;
    }
    Node new_games_list = nodeAdd(tournament->games, new_game);
    if(new_games_list == NULL)
    {
        setRemove(tournament->tournament_players, first_player);
        setRemove(tournament->tournament_players, second_player);
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

Node tournamentGetGames(Tournament tournament)
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

int tournamentGetWinnerId(Tournament tournament)
{
    assert(tournament != NULL);
    return tournament->winner_id;
}

bool tournamentHasEnded(Tournament tournament)
{
    assert(tournament != NULL);
    return tournamentGetWinnerId(tournament) != TOURNAMENT_IN_PROGRESS;
}

const char* tournamentGetLocation(Tournament tournament)
{
    assert(tournament != NULL);
    return tournament->tournament_location;
}

int tournamentGetLongestPlayTime(Tournament tournament)
{
    assert(tournament != NULL);
    return nodeGetLongestPlayTime(tournament->games);
}
int tournamentGetNumOfGames(Tournament tournament)
{
    assert(tournament != NULL);
    return nodeGetSize(tournament->games);
}
int tournamentGetNumOfPlayers(Tournament tournament)
{
    assert(tournament != NULL);
    return setGetSize(tournament->tournament_players);
}
int tournamentGetPlayTime(Tournament tournament)
{
    assert(tournament != NULL);
    return nodeGetTotalPlayTime(tournament->games);
}
double tournamentGetAveragePlayTime(Tournament tournament)
{
    assert(tournament != NULL);
    return nodeGetAveragePlayTime(tournament->games);
}

void tournamentSetWinnerId(Tournament tournament, int winner_id)
{
    assert(tournament != NULL);

    tournament->winner_id = winner_id;
}