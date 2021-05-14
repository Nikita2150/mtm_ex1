#ifndef GAME_H_
#define GAME_H_

#include <stdio.h>

typedef struct game_t* Game;


Game gameCreate(int first_player, int second_player, Winner winner, int play_time);

int gameGetFirstPlayer(Game game);
int gameGetSecondPlayer(Game game);

void gameSetFirstPlayer(Game game, int first_to_set);
void gameSetSecondPlayer(Game game, int second_to_set);

Winner gameGetWinner(Game game);

int gameGetPlayTime(Game game);

void gameDestroy(Game game);


#endif // GAME_H_

