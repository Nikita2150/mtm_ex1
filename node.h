#ifndef NODE_H_
#define NODE_H_

#include "game.h"
#include "chessSystem.h"
#include <stdbool.h>
#include <assert.h>

typedef struct node_t* Node;

typedef enum {
    NODE_SUCCESS,
    NODE_OUT_OF_MEMORY
} NodeResult;


/**
 * Create a new node with a game given in the arguments and the next is NULL.
 * @param game - a game that will be in the node
 * @return
 * 		NULL if the malloc didn't succeed, else a new node.
 */
Node nodeCreate(Game game);

/**
 * Check if a node contains a game with two players (in the same order)
 * @param node - the node we want to search in
 * @param first_player - the id of the first player
 * @param second_player - the id of the second player
 * @return true if a game as described exists, else false.
 */
bool nodeContains(Node node, int first_player, int second_player);


/**
 * Add a node (with a specific game in it) in the start of a list of nodes
 * @param list - the current list we want to add to (if NULL, will return a single node with the game inside)
 * @param game - the game we want the added node to contain (Must be non-NULL)
 * @return - NULL if malloc didn't succeed, else a list of nodes containing the new node
 */
Node nodeAdd(Node list, Game game);

/**
 * Destroy a whole list of nodes
 * @param node - the list we want to destroy
 */
void nodeDestroy(Node node); 

/**
 * Remove the first node from a list
 * @param node - the list we want to delete the first node of
 * @return the new list without the first node
 */
Node removeFirstNode(Node node);

/**
 * Copy a node (and all of its parameters)
 * @param list - the list we want to copy
 * @return the copied list
 */
Node nodeCopy(Node list);

/**
 * Get the size of a list
 * @param list - the list we want to check the size of
 * @return the size of the list
 */
int nodeGetSize(Node list);

/**
 * Get the total play time of all the games inside the list
 * @param list - the list we want to check
 * @return the total play time of all the games in the node
 */
int nodeGetTotalPlayTime(Node list);

/**
 * Get the longest play time amongst all the games in the list
 * @param list - the list we want to check
 * @return the longest play time of all the games in the list
 */
int nodeGetLongestPlayTime(Node list);

/**
 * Get the average play time of all the games in the node
 * @param list - the list we want to check
 * @return the average play time of all the games in the list
 */
double nodeGetAveragePlayTime(Node list);

/**
 * Check if the players are new (didn't play in any game of the node yet)
 * @param games - the list of games
 * @param first_player - id of the first player
 * @param second_player - id of the second player
 * @param new_first_player - if the first player is new, will point to true, else to false
 * @param new_second_player - if the second player is new, will point to true, else to false
 */
void nodeAreNewPlayers(Node games, int first_player, int second_player, bool* new_first_player, 
                        bool* new_second_player);

/**
 * Update games (who wins, players inside) and players (number of wins, draws etc.) when removing a player
 * from the system
 * @param players - map of players
 * @param games - list of games
 * @param current_player_id - id of the player removing
 */
void nodeUpdateWhenRemovingPlayer(Map players, Node games, int current_player_id);

/**
 * Add all players to the tournament_players map according to the list of games
 * @param games - list of games
 * @param tournament_players - the map to update
 * @return
 * 		NODE_OUT_OF_MEMORY - if malloc didn't succeed
 * 		NODE_SUCCESS - if we updated successfully
 */
NodeResult nodeGetTournamentPlayersMap(Node games, Map tournament_players);

/**
 * Update the players statistics when removing a tournament
 * @param node - list of games
 * @param players  - map of games to update
 */
void nodeTournamentRemoveUpdatePlayers(Node node, Map players);

/**
 * Count how many games a player plays in
 * @param games - list of games
 * @param player_id - id of the player
 * @return number of games
 */
int nodeCountPlayerAppearances(Node games, int player_id);

/**
 * Update the property "bool player_deleted_after_end[NUM_OF_PLAYERS]" if a player was deleted after a
 * tournament was deleted
 * @param games - list of games to update
 * @param player_id - the id of the player
 */
void nodeUpdateRemovedPlayersAfterEnd(Node games, int player_id);
#endif // NODE_H_