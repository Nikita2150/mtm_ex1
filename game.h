#ifndef GAME_H_
#define GAME_H_

#include <stdio.h>

typedef struct game_t* Game;

typedef enum{
    FIRST,
    SECOND,
    DRAW,
} Winner;

//creating 
Game gameCreate(int first_player, int second_player, Winner winner, int play_time);

int gameGetFirst_player(Game game);
int gameGetSecond_player(Game game);

int gameSetFirst_player(Game game, int first_to_set);
int gameSetSecond_player(Game game, int second_to_set);

Winner gameGetWinner(Game game);

int gameGetPlat_time(Game game);


//
void gameDestroy(Game game);

#endif // GAME_H_

