#ifndef TOURNAMENT_H_
#define TOURNAMENT_H_

#include <stdbool.h>
#include "chessSystem.h"

typedef struct tournament_t* Tournament;

Tournament tournamentCreate(int max_games_per_player, const char* tournament_location);

bool tournamentContains(Tournament tournament, int first_player, int second_player);

void tournamentDestroy(Tournament tournament);

Tournament tournamentCopy(Tournament tournament);

MapResult tournamentAddGame(Tournament tournament, int first_player, int second_player, Winner winner, int play_time);

int tournamentGetMaxGamesPerPlayer(Tournament tournament);

Node getGamesOfTournament(Tournament tournament);

void tournamentRemoveFirstGame(Tournament tournament);


#endif