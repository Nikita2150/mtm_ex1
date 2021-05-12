#ifndef TOURNAMENT_H_
#define TOURNAMENT_H_


typedef struct tournament_t* Tournament;

Tournament tournamentCreate(int max_games_per_player, const char* tournament_location);

void tournamentDestroy(Tournament tournament);

Tournament copyTournament(Tournament tournament);

#endif