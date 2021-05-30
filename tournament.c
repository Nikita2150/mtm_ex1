#include <stdlib.h>
#include "tournament.h"
#include "node.h"
#include "map.h"
#include "player.h"


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

Tournament tournamentCopy(Tournament tournament)
{
    if(tournament == NULL)
    {
        return NULL;
    }

    Tournament new_tournament = tournamentCreate(tournament->max_games_per_player,
												 tournament->tournament_location);
    if(new_tournament == NULL)
    {
        return NULL;
    }
    new_tournament->games = nodeCopy(tournament->games);
    if(new_tournament->games == NULL && tournament->games != NULL)
    {
        return NULL;
    }
    new_tournament->winner_id = tournament->winner_id;
    new_tournament->num_of_tournament_players = tournament->num_of_tournament_players;
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

TournamentResult tournamentAddGame(Tournament tournament,Game new_game, bool new_first_player, bool new_second_player)
{
	assert(new_game != NULL && tournament != NULL);

    //For readability:
    int new_first_player_val = (new_first_player) ? 1 : 0, new_second_player_val =(new_second_player) ? 1 : 0;

    tournament->num_of_tournament_players += new_first_player_val + new_second_player_val;

    Node new_games_list = nodeAdd(tournament->games, new_game);
    if(new_games_list == NULL)
    {
        return TOURNAMENT_OUT_OF_MEMORY;
    }
    tournament->games = new_games_list;
    return TOURNAMENT_SUCCESS;
}



bool tournamentHasEnded(Tournament tournament)
{
    assert(tournament != NULL);
    return tournament->winner_id != TOURNAMENT_IN_PROGRESS;
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


bool tournamentPlayersExceededGames(Tournament tournament, int player_id_one, int player_id_two)
{
    if(tournament == NULL)
    {
        return false;
    }
    
    Node games_list = tournament->games;

    int counter_one = nodeCountPlayerAppearances(games_list, player_id_one);
    int counter_two = nodeCountPlayerAppearances(games_list, player_id_two);
    return counter_one >= tournament->max_games_per_player || counter_two >= tournament->max_games_per_player;
}

void tournamentAreNewPlayers(Tournament tournament, int first_player, int second_player, bool* new_first_player,
							 bool* new_second_player)
{
    assert(tournament!=NULL);
    nodeAreNewPlayers(tournament->games, first_player, second_player, new_first_player, new_second_player);
}

void tournamentUpdateWhenRemovingPlayer(Map players, Tournament tournament, int current_player_id)
{
	assert(tournament != NULL);
	Node game_iterator = tournament->games;
	if(game_iterator == NULL)
	{
		return;
	}
    if(tournament->winner_id == TOURNAMENT_IN_PROGRESS)
    {
        nodeUpdateWhenRemovingPlayer(players, game_iterator, current_player_id);
    }
    else
    {
	    nodeUpdateRemovedPlayersAfterEnd(game_iterator, current_player_id);
    }
}

TournamentResult tournamentGetPlayersMap(Tournament tournament, Map tournament_players)
{
    if(tournament != NULL)
    {
        if(nodeGetTournamentPlayersMap(tournament->games, tournament_players) != NODE_SUCCESS)
        {
            return TOURNAMENT_OUT_OF_MEMORY;
        }
    }
    return TOURNAMENT_SUCCESS;
}

void tournamentRemoveUpdatePlayers(Tournament tournament, Map players)
{
    if(tournament != NULL)
    {
        nodeTournamentRemoveUpdatePlayers(tournament->games , players);
    }
}

void tournamentCalculateWinner(Tournament tournament, Map players)
{
    assert(tournament != NULL && players != NULL);

    int* player_iterator_id = mapGetFirst(players);
    //The player exists, we checked
    Player current_player = mapGet(players, player_iterator_id), max_player_score = current_player;
    int max_score = playerGetScore(current_player), max_id_score = *player_iterator_id;
    free(player_iterator_id);

    while((player_iterator_id = mapGetNext(players)) != NULL)
    {
        current_player = mapGet(players, player_iterator_id);
        assert(current_player != NULL);
        if((max_score < playerGetScore(current_player)) || (max_score == playerGetScore(current_player) &&
                !sameWinner(max_player_score, max_id_score, current_player, *player_iterator_id)))
        {
            max_player_score = current_player;
            max_score = playerGetScore(current_player);
            max_id_score = *player_iterator_id;
        }
        free(player_iterator_id);
    }

    tournament->winner_id = max_id_score;
}

bool tournamentPrintStatistics(Tournament tournament, FILE* stream)
{
    assert(stream != NULL);
    return fprintf(stream, "%d\n%d\n%.2f\n%s\n%d\n%d\n", tournament->winner_id,
              tournamentGetLongestPlayTime(tournament), tournamentGetAveragePlayTime(tournament),
              tournament->tournament_location,tournamentGetNumOfGames(tournament),
              tournament->num_of_tournament_players) != EOF;
}