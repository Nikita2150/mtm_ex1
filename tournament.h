#ifndef TOURNAMENT_H_
#define TOURNAMENT_H_

#include <stdbool.h>
#include "chessSystem.h"
#include "node.h"
#include "map.h"
#include <assert.h>

#define TOURNAMENT_IN_PROGRESS -1

typedef struct tournament_t* Tournament;

Tournament tournamentCreate(int max_games_per_player, const char* tournament_location);

bool tournamentContains(Tournament tournament, int first_player, int second_player);

void tournamentDestroy(Tournament tournament);
void tournamentFree(MapDataElement tournament);

MapDataElement tournamentCopy(MapDataElement tournament);

MapResult tournamentAddGame(Tournament tournament, int first_player, int second_player, Winner winner, int play_time,
                            bool new_first_player, bool new_second_player);

int tournamentGetMaxGamesPerPlayer(Tournament tournament);

Node tournamentGetGames(Tournament tournament);
int tournamentGetWinnerId(Tournament tournament);

bool tournamentHasEnded(Tournament tournament);

void tournamentRemoveFirstGame(Tournament tournament);

const char* tournamentGetLocation(Tournament tournament);

int tournamentGetLongestPlayTime(Tournament tournament);
int tournamentGetNumOfGames(Tournament tournament);
int tournamentGetNumOfPlayers(Tournament tournament);
int tournamentGetPlayTime(Tournament tournament);
double tournamentGetAveragePlayTime(Tournament tournament);

void tournamentSetWinnerId(Tournament tournament, int winner_id);

bool tournamentExceededGames(Tournament tournament, int player_id);
#endif