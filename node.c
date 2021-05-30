#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "map.h"
#include "chessSystem.h"


struct node_t
{
    Game game;
    struct node_t* next;
};

Node nodeCreate(Game game)
{
    Node new_node = malloc(sizeof(*new_node));
    if(new_node == NULL)
    {
        return NULL;
    }
    new_node->game = game;
    new_node->next = NULL;

    return new_node;
}


void nodeDestroy(Node node)
{
    while(node != NULL)
    {
        node = removeFirstNode(node);
    }
}

bool nodeContains(Node node, int first_player, int second_player)
{
    while(node != NULL)
    {
        if(gameContains(node->game, first_player, second_player))
        {
            return true;
        }
        node = node->next;
    }
    return false;  
}

Node nodeAdd(Node list, Game game)
{
    assert(game != NULL);
    Node new_node = nodeCreate(game);
    if(new_node == NULL)
    {
        return NULL;
    }

    new_node->next = list;
    return new_node;
}

Node removeFirstNode(Node node)
{
    if(node == NULL)
    {
        return NULL;
    }
    Node temp = node;
    node = node->next;
    gameDestroy(temp->game);
    free(temp);

    return node;
}

Node nodeCopy(Node list)
{
    if(list == NULL)
    {
        return NULL;
    }

    Game new_game = gameCopy(list->game);
    if(new_game == NULL && list->game != NULL)
    {
        return NULL;
    }
    Node new_list = nodeCreate(new_game);
    if(new_list == NULL)
    {
        gameDestroy(new_game);
        return NULL;
    }
    
    list = list->next;
    Node head = new_list;
    while(list != NULL)
    {
        Game add_to_list_game = gameCopy(list->game);
        if(add_to_list_game == NULL && list->game != NULL)
        {
            nodeDestroy(new_list);
            return NULL;
        }
        Node add_to_list = nodeCreate(add_to_list_game);

        if(add_to_list == NULL)
        {
            gameDestroy(add_to_list_game);
            nodeDestroy(new_list);
            return NULL;
        }
        
        head->next = add_to_list;
        head = add_to_list;
    }
    return new_list;
}


int nodeGetSize(Node list)
{
    int counter = 0;
    while(list != NULL)
    {
        counter++;
        list = list->next;
    }
    return counter;
}
int nodeGetTotalPlayTime(Node list)
{
    int total_play_time = 0;
    while(list != NULL)
    {
        total_play_time += gameGetPlayTime(list->game);
        list = list->next;
    }
    return total_play_time;
}
int nodeGetLongestPlayTime(Node list)
{
    int max_play_time = 0;
    while(list != NULL)
    {
        if(gameGetPlayTime(list->game) > max_play_time)
        {
            max_play_time = gameGetPlayTime(list->game);
        }
        list = list->next;
    }
    return max_play_time;
}
double nodeGetAveragePlayTime(Node list)
{
    if(nodeGetSize(list) == 0)
    {
        return 0;
    }

    return (nodeGetTotalPlayTime(list) * 1.0) / nodeGetSize(list);
}

void nodeAreNewPlayers(Node games, int first_player, int second_player, bool* new_first_player, 
                        bool* new_second_player)
{
    while(games != NULL)
	{
		gameAreNewPlayers(games->game, first_player, second_player, new_first_player, new_second_player);
        games = games->next;
	}
}

void nodeUpdateWhenRemovingPlayer(Map players, Node games, int current_player_id)
{
    while(games != NULL)
    {
        Game current_game = games->game;
        gameUpdateWhenRemovingPlayer(players, current_game, current_player_id);
        games = games->next;
    }
}

NodeResult nodeGetTournamentPlayersMap(Node games, Map tournament_players)
{
    while(games != NULL)
    {
        if(gameAddPlayersToMap(games->game, tournament_players) != GAME_SUCCESS)
        {
            return NODE_OUT_OF_MEMORY;
        }
        games = games->next;
    }
    return NODE_SUCCESS;
}

void nodeTournamentRemoveUpdatePlayers(Node node, Map players)
{
    while(node != NULL)
    {
        gameTournamentRemoveUpdatePlayers(node->game, players);
        node = node->next;
    }
}

int nodeCountPlayerAppearances(Node games, int player_id)
{
    int count = 0;
    while(games != NULL)
    {
        Game current_game = games->game;
        if(gamePlayerIsPlaying(current_game, player_id))
        {
            count++;
        }
        games = games->next;
    }
    return count;
}

void nodeUpdateRemovedPlayersAfterEnd(Node games, int player_id)
{
	while(games != NULL)
	{
		gameUpdateRemovedPlayersAfterEnd(games->game, player_id);
		games = games->next;
	}
}