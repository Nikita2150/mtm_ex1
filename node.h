#ifndef NODE_H_
#define NODE_H_

#include "game.h"
#include "chessSystem.h"
#include <stdbool.h>

typedef struct node_t* Node;

Node nodeCreate(Game game);

Game nodeGetGame(Node node);
void nodeSetGame(Node node, Game game);

Node nodeGetNext(Node node);
void nodeSetNext(Node node, Node to_set_next);

bool nodeIsEqual(Node node1, Node node2);

bool nodeContains(Node node, int first_player, int second_player);

Node nodeAdd(Node list, Game game);

void nodeDestroy(Node node); 

Node removeFirstNode(Node node);

Node nodeCopy(Node list);

//setnext

#endif // NODE_H_