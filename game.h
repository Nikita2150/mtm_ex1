#ifndef GAME_H_
#define GAME_H_

#include <stdio.h>
#include "chessSystem.h"
#include <assert.h>
#include <stdbool.h>
#include "map.h"

#define NUM_OF_PLAYERS 2
#define FIRST_PLAYER_PLACE 0
#define SECOND_PLAYER_PLACE 1

typedef enum {
    GAME_SUCCESS,
    GAME_OUT_OF_MEMORY
} GameResult;

#define DELETED_PLAYER -2

typedef struct game_t* Game;

/**
 * Create a new game struct
 * @param first_player
 * @param second_player
 * @param winner
 * @param play_time
 * @return a new game struct
 */
Game gameCreate(int first_player, int second_player, Winner winner, int play_time);

/**
 * Destroy a single game (free it)
 * @param game - the game to destroy
 */
void gameDestroy(Game game);

/**
 * Copy a game and all its fields
 * @param game - the game to copy
 * @return
 * 		NULL - if the game given is null or if there is an allocation error.
 * 		A new copied game if the copy was successfull.
 */
Game gameCopy(Game game);

/**
 * Check if the players are new (didn't play in the current game)
 * @param current_game - the game to check
 * @param first_player - id of the first player
 * @param second_player - id of the second player
 * @param new_first_player - will point to false if the player is not new
 * @param new_second_player - will point to false if the player is not new
 */
void gameAreNewPlayers(Game current_game, int first_player, int second_player, bool* new_first_player, 
                        bool* new_second_player);

/**
 * Update the games and players when removing a player
 * @param players - map of players to update
 * @param current_game - game to update
 * @param current_player_id - id of the player that is being removed
 */
void gameUpdateWhenRemovingPlayer(Map players, Game current_game, int current_player_id);

/**
 * Add the players of a game to a map of players
 * @param game - the game where the players are stored (Must be non-NULL)
 * @param tournament_players - map that is being updated (Must be non-NULL)
 * @return
 *		GAME_OUT_OF_MEMORY - if an allocation failed
 *		GAME_SUCCESS - if the add was successfull
 */
GameResult gameAddPlayersToMap(Game game, Map tournament_players);

/**
 * Update the players of a removed game (when a tournament is being removed)
 * @param game - the game that is being removed
 * @param players - map of players to be updated
 */
void gameTournamentRemoveUpdatePlayers(Game game, Map players);

/**
 * Check if the game contains those two players (in the same order)
 * @param game - the game to check
 * @param first_player - id of the first player
 * @param second_player - id of the second player
 * @return
 * 		true - the game contains
 * 		false - else
 */
bool gameContains(Game game, int first_player, int second_player);

/**
 * Get the play time of a game
 * @param game - the game (Must be non-NULL)
 * @return the play time of a game
 */
int gameGetPlayTime(Game game);

/**
 * Check if a player is playing in a game
 * @param game - the game to check
 * @param first_player - id of the player
 * @return
 * 		true - if the player is playing in the game
 * 		false - else
 */
bool gamePlayerIsPlaying(Game game, int first_player);

/**
 * After removing a player from a ended tournament - need to update
 * "player_deleted_after_end" fields accordingly
 * @param game - the game to update
 * @param player_id - id of the player that has been removed
 */
void gameUpdateRemovedPlayersAfterEnd(Game game, int player_id);


#endif // GAME_H_

