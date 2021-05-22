#ifndef _PLAYER_H
#define _PLAYER_H

#include <stdbool.h>
#include "chessSystem.h"
#include "map.h"

typedef struct player_t* Player;

Player playerCreate();
void playerDestroy(Player player);
void playerFree(MapDataElement player);

void playerAddGame(Player player);

//both add a game (int num_of_games)
void playerAddWin(Player player);
void playerAddDraw(Player player);

void playerAddTime(Player player, int time);

MapDataElement playerCopy(MapDataElement player);

int playerGetScore(Player player);

int playerGetNumOfGames(Player player);
int playerGetNumOfWins(Player player);
int playerGetNumOfDraws(Player player);
int playerGetNumOfLosses(Player player);

int playerGetLevel(Player player);


int playerGetPlayTime(Player player);


void playerUpdateRemovedGame(Player player, Winner winner, bool first, int play_time);

void playerSubtractDraw(Player player);

bool sameWinner(Player current_winner, int current_winner_id, Player current_player, int current_player_id);

#endif //_PLAYER_H