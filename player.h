#ifndef _PLAYER_H
#define _PLAYER_H

#include <stdbool.h>
#include "chessSystem.h"
#include "map.h"

typedef enum
{
    PLAYER_SUCCESS,
    PLAYER_OUT_OF_MEMORY
} PlayerResult;

typedef struct player_t* Player;


/**
 * Creates a player
 * @return A player struct with 0 in all the parameters
 */
Player playerCreate();

/**
 * Destroy a player (free it)
 * @param player - the player to destroy
 */
void playerDestroy(Player player);

/**
 * Copy a player and all of its fields
 * @param player - the player to copy
 * @return the copied player
 */
Player playerCopy(Player player);

/**
 * Get the score of a player according to the formula
 * @param player - the player to get the score of
 * @return the score of the player
 */
double playerGetScore(Player player);

/**
 * Get the level of a player according to the formula
 * @param player - the player to get the level of
 * @return the level of the player
 */
double playerGetLevel(Player player);

/**
 * Updating the statistics of a player when removing a game (happens when removing a tournament)
 * @param player - the player to update
 * @param winner - who won in the current game
 * @param first - if the player is the first player of the game removing
 * @param play_time - the play time of the game
 */
void playerUpdateRemovedGame(Player player, Winner winner, bool first, int play_time);

/**
 * compare two potential winners who have the same score
 * @param current_winner - current winner Player data
 * @param current_winner_id - current winner id
 * @param current_player - new potential winner's Player data
 * @param current_player_id - new potential winner's id
 * @return true if amongst those two players the winner is "current_winner_id", else false
 */
bool sameWinner(Player current_winner, int current_winner_id, Player current_player, int current_player_id);

/**
 * Update two players when adding a game
 * @param first_player_data - player data of the first player
 * @param second_player_data - player data of the second player
 * @param winner - the winner of the game
 * @param play_time - the play time of the game
 */
void updateNewGame(Player first_player_data, Player second_player_data, Winner winner, int play_time);

/**
 * When removing a player - update the second player of the game (if exists) - number of wins, etc.
 * @param players - the map of players to be updated
 * @param winner - the previous winner of the game (before removing the player)
 * @param current_player_id - the player that is being removed
 * @param first_player - id of the first player
 * @param second_player - id of the second player
 */
void playerUpdateWhenRemoving(Map players, Winner winner, int current_player_id, int first_player, 
                                    int second_player);

/**
 * Initialize a player -
 * 		if exists in the map - get a copy of the data from the map.
 * 		else, create it.
 * @param players - map of players
 * @param player_id - the id of the player we need to initialize
 * @return
 * 		NULL - if the malloc didn't succeed (when copying or creating the player).
 * 		else, the player data that is matching the id.
 */
Player initializePlayer(Map players, int* player_id);

/**
 * Initialize two players (will only initialize players with id > 0)
 * @param players - map of players
 * @param first_player_id - id of the first player
 * @param second_player_id - id of the second player
 * @param first_player_data - will point to the initialized first player (or NULL if id is less than 1)
 * @param second_player_data - will point to the initialized second player (or NULL if id is less than 1)
 * @return
 * 		PLAYER_OUT_OF_MEMORY - if a malloc didn't succeed ("first_player_data" and "second_player_data" will be
 * 															undefined)
* 		PLAYER_SUCCESS - if we initialized both players
 */
PlayerResult initializeBothPlayers(Map players, int first_player_id, int
							second_player_id, Player* first_player_data, Player* second_player_data);

/**
 * Calculate the average play time of a player
 * @param player - the player
 * @return average play time of a player
 */
double playerAverageTime(Player player);


/**
 * Check if a player has games
 * @param player - the player to check
 * @return
 * 		true - the player has games
 * 		false - the player doesn't have games
 */
bool playerHasGames(Player player);
#endif //_PLAYER_H