#ifndef _PLAYER_H
#define _PLAYER_H

#include <stdbool.h>

typedef struct player_t* Player;

Player playerCreate();
void playerDestroy(Player player);

void playerAddGame(Player player);

//both add a game (int num_of_games)
void playerAddWin(Player player);
void playerAddDraw(Player player);

void playerAddTime(Player player, int time);

Player playerCopy(Player player);

bool playerGetIsOut(Player player);


#endif //_PLAYER_H