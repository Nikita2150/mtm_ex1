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
    if(node == NULL)
    {
        return NULL;
    }
    node->game = game;
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
    if(node == NULL)
    {
        return NULL;
    }
    node->next = to_set_next;
    
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