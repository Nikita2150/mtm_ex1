#include <stdlib.h>
#include "tournament.h"
#include "node.h"
#include "map.h"



struct tournament_t
{
    int max_games_per_player;
    const char* tournament_location;
    int winner_id;
    Node games;
    int num_of_tournament_players;
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

    new_tournament->num_of_tournament_players = 0;
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
    if(new_tournament->games == NULL && this_tournament->games != NULL)
    {
        return NULL;
    }
    new_tournament->winner_id = this_tournament->winner_id;
    new_tournament->num_of_tournament_players = this_tournament->num_of_tournament_players;
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

MapResult tournamentAddGame(Tournament tournament, int first_player, int second_player, Winner winner, int play_time,
                            bool new_first_player, bool new_second_player)
{
    if(tournament == NULL)
    {
        return MAP_NULL_ARGUMENT;
    }
    //For readabilty: 
    int new_first_player_val = (new_first_player) ? 1 : 0, new_second_player_val = (new_second_player) ? 1 : 0;

    tournament->num_of_tournament_players += new_first_player_val + new_second_player_val;
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
    return tournament->num_of_tournament_players;
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

bool tournamentExceededGames(Tournament tournament, int player_id)
{
    if(tournament == NULL)
    {
        return false;
    }
    int counter = 0;
    Node games_list = tournament->games;
    while(games_list != NULL)
    {
        Game current_game = nodeGetGame(games_list);
        if(gameGetFirstPlayer(current_game) == player_id || gameGetSecondPlayer(current_game) == player_id)
        {
            counter++;
        }
        games_list = nodeGetNext(games_list);
    }
    return counter >= tournament->max_games_per_player;
}