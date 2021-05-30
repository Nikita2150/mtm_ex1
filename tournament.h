#ifndef TOURNAMENT_H_
#define TOURNAMENT_H_

#include <stdbool.h>
#include "chessSystem.h"
#include "node.h"
#include "map.h"
#include <assert.h>

typedef enum {
    TOURNAMENT_SUCCESS,
    TOURNAMENT_OUT_OF_MEMORY
} TournamentResult;

#define TOURNAMENT_IN_PROGRESS -1

typedef struct tournament_t* Tournament;


/**
 * Create a tournament struct with no games
 * @param max_games_per_player
 * @param tournament_location
 * @return a new tournament with the relevant fields
 */
Tournament tournamentCreate(int max_games_per_player, const char* tournament_location);

/**
 * Check if a tournament contains a game with those players (in the same order)
 * @param tournament - tournament to check
 * @param first_player - id of the first player
 * @param second_player - id of the second player
 * @return
 * 		true - the tournament contains a game as described
 * 		false - else
 */
bool tournamentContains(Tournament tournament, int first_player, int second_player);

/**
 * Destroy (free) a tournament and all of its fields
 * @param tournament - the tournament to destroy
 */
void tournamentDestroy(Tournament tournament);

/**
 * Copy a tournament and all of its fields
 * @param tournament - the tournament to copy
 * @return
 * 		NULL - one of the allocations failed or tournament is null.
 * 		else - the copied tournament struct
 */
Tournament tournamentCopy(Tournament tournament);

/**
 * Add a game into the list of games and update the field "num_of_tournament_players" if at least one
 * of the players is new
 * @param tournament - the tournament to update (Must be non-NULL)
 * @param new_game - the new game to add (Must be non-NULL)
 * @param new_first_player - if the first player of the game is new (didn't play in any game in this tournament)
 * @param new_second_player - if the second player of the game is new (didn't play in any game in this tournament)
 * @return
 * 		TOURNAMENT_OUT_OF_MEMORY - if an allocation failed
 * 		TOURNAMENT_SUCCESS - added successfully
 */
TournamentResult tournamentAddGame(Tournament tournament,Game new_game,bool new_first_player, bool new_second_player);

/**
 * Check if a tournament ended
 * @param tournament - the tournament to check (Must be non-NULL)
 * @return
 * 		true - if the tournament ended
 * 		false - else
 */
bool tournamentHasEnded(Tournament tournament);

/**
 * Get the longest play time of a tournament
 * @param tournament - the tournament to check (Must be non-NULL)
 * @return the longest play time of a tournament
 */
int tournamentGetLongestPlayTime(Tournament tournament);

/**
 * Get the number of games in a tournament
 * @param tournament - the tournament to check (Must be non-NULL)
 * @return numbers of games in a tournament
 */
int tournamentGetNumOfGames(Tournament tournament);

/**
 * Get the number of players in a tournament
 * @param tournament - the tournament to check (Must be non-NULL)
 * @return number of players in a tournament
 */
int tournamentGetNumOfPlayers(Tournament tournament);

/**
 * Get the total play time of a tournament
 * @param tournament - the tournament to check (Must be non-NULL)
 * @return the total play time of a tournament
 */
int tournamentGetPlayTime(Tournament tournament);

/**
 * Get the average play time of a tournament
 * @param tournament - the tournament to check (Must be non-NULL)
 * @return the average play time of a tournament
 */
double tournamentGetAveragePlayTime(Tournament tournament);

/**
 * Check if at least one of the players can't play in games anymore (played in too many games)
 * @param tournament - the tournament to check
 * @param player_id_one - id of the first player
 * @param player_id_two - id of the second player
 * @return
 * 		true - if at least one of the players exceeds the number of games
 * 		false - else
 */
bool tournamentPlayersExceededGames(Tournament tournament, int player_id_one, int player_id_two);

/**
 * Check if the players are new in tournament (didn't play in any game in the tournament)
 * @param tournament - the tournament to check
 * @param first_player - id of the first player
 * @param second_player - id of the second player
 * @param new_first_player - will point to true if the first player is new, else to false
 * @param new_second_player - will point to true if the second player is new, else to false
 */
void tournamentAreNewPlayers(Tournament tournament, int first_player, int second_player, bool* new_first_player,
							 bool* new_second_player);

/**
 * Update the games and players in the tournament when removing a player
 * @param players - map of players
 * @param tournament - the tournament to update
 * @param current_player_id - the id of the player which is being removed
 */
void tournamentUpdateWhenRemovingPlayer(Map players, Tournament tournament, int current_player_id);

/**
 * Insert all the players (according to the games in the tournament) into a new map
 * @param tournament - update the map according to this tournament
 * @param tournament_players - should be a new created map of players
 * @return
 * 		TOURNAMENT_OUT_OF_MEMORY - if an allocation failed
 * 		TOURNAMENT_SUCCESS - if inserted successfully
 */
TournamentResult tournamentGetPlayersMap(Tournament tournament, Map tournament_players);

/**
 * Update the players in the tournament before removing it
 * @param tournament - the tournament that is being removed
 * @param players - the map of players to update
 */
void tournamentRemoveUpdatePlayers(Tournament tournament, Map players);


/**
 * Calculate the winner of a tournament and assign the winner_id field to the result
 * @param tournament - the tournament to check
 * @param players - map of players (the one created with tournamentGetPlayersMap - so we will have all
 * the data according only to this tournament)
 */
void tournamentCalculateWinner(Tournament tournament, Map players);

/**
 * Print the tournament statistics into the file
 * @param tournament - the tournament we need to print
 * @param stream - the file we need to update
 * @return
 * 		true - if printed successfuly
 * 		false - else
 */
bool tournamentPrintStatistics(Tournament tournament, FILE* stream);


#endif