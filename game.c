#include <stdio.h>
#include "game.h"
#include <stdlib.h>

#include "chessSystem.h"

struct game_t{

    int first_player;
    int second_player;

    Winner winner;

    int play_time;
};

Game gameCreate(int first_player, int second_player, Winner winner, int play_time)
{
    Game game = malloc(sizeof(*game));
    if(game == NULL)
    {
        return NULL;
    }
    game->first_player = first_player;
    game->second_player = second_player;

    game->winner = winner;

    game->play_time = play_time;

    return game;
}

void gameDestroy(Game game)
{
    if(game != NULL)
    {
        free(game);
    }
}

int gameGetFirstPlayer(Game game)
{
    assert(game != NULL);
    return game->first_player;
}
int gameGetSecondPlayer(Game game)
{
    assert(game != NULL);
    return game->second_player;
}

void gameSetFirstPlayer(Game game, int first_to_set)
{
    if(game != NULL)
    {
        game->first_player = first_to_set;
    } 
}
void gameSetSecondPlayer(Game game, int second_to_set)
{
    if(game != NULL)
    {
        game->second_player = second_to_set;
    }
}

//ASSERT: game exists
Winner gameGetWinner(Game game)
{
    assert(game != NULL);
    return game->winner;
}

//ASSERT: game exists
int gameGetPlayTime(Game game)
{
    assert(game != NULL);
    return game->play_time;
}


Game gameCopy(Game game)
{
    assert(game != NULL);
    Game new_game = malloc(sizeof(*new_game));
    if(new_game == NULL)
    {
        return NULL;
    }
    new_game->first_player = game->first_player;
    new_game->play_time = game->play_time;
    new_game->second_player = game->second_player;
    new_game->winner = game->winner;

    return new_game;
}

void gameSetWinner(Game game, Winner winner)
{
    if(game != NULL)
    {
        game->winner = winner;
    }
}