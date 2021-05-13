#ifndef _PLAYER_H
#define _PLAYER_H

#include <stdbool.h>

typedef struct player_t* Player;

Player playerCreate();
void playerDestroy(Player player);

#endif //_PLAYER_H