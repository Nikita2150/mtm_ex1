#include <stdio.h>
#include <stdlib.h>
#include "node.h"


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

Game nodeGetGame(Node node)
{
    if(node == NULL)
    {
        return NULL;
    }
    return node->game;
}

void nodeSetGame(Node node, Game game)
{
    if(node != NULL)
    {
         node->game = game;
    }
}

Node nodeGetNext(Node node)
{
    if(node == NULL)
    {
        return NULL;
    }
    return node->next;
}

void nodeSetNext(Node node, Node to_set_next)
{
    if(node != NULL)
    {
        node->next = to_set_next;
    }
}

void nodeDestroy(Node node)
{
    while(node != NULL)
    {
        Node toDelete = node;
        node = node->next;
        gameDestroy(toDelete->game);
        free(toDelete);
    }
}

bool nodeContains(Node node, int first_player, int second_player)
{
    while(node != NULL)
    {
        if(gameGetFirstPlayer(node->game) == first_player &&
         gameGetSecondPlayer(node->game) == second_player)
        {
            return true;
        }
        node = node->next;
    }
    return false;  
}

Node nodeAdd(Node list, Game game)
{
    assert(list != NULL && game != NULL);
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