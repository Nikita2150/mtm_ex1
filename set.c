#include "set.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define INITIAL_SIZE 2
#define EXPAND_FACTOR 2
#define ELEMENT_NOT_FOUND -1

#define SET_ENDED -1



struct set {
 int* ids;
 int size; 
 int maxSize; 
 int iterator;
};

static SetResult expand(Set set)
{
    int newSize = EXPAND_FACTOR * set->maxSize;
    int* newElements = realloc(set->ids, newSize * sizeof(*newElements));
    if (newElements == NULL)
    {
        return SET_OUT_OF_MEMORY;
    }
    set->ids = newElements;
    set->maxSize = newSize;
    return SET_SUCCESS;
}

static int find(Set set, int player_id)
{
    for (int i = 0; i < set->size; i++) 
    {
        if (set->ids[i] == player_id)
        {
            return i;
        }
    }
    return ELEMENT_NOT_FOUND;
}

Set setCreate() 
{
    Set set = malloc(sizeof(*set));
    if (set == NULL)
    {
        return NULL;
    }
    set->ids = malloc(INITIAL_SIZE * sizeof(int*));
    if (set->ids == NULL) 
    {
        free(set);
        return NULL;
    }
    set->size = 0;
    set->maxSize = INITIAL_SIZE;
    return set;
}

SetResult setAdd(Set set, int player_id) 
{
    assert(set != NULL);

    if (setContains(set, player_id))
    {
        return SET_SUCCESS;
    }
    if (set->size == set->maxSize)
    {
        if (expand(set) == SET_OUT_OF_MEMORY)
        {
            return SET_OUT_OF_MEMORY;
        }
    }
    int new_int = player_id;
    set->ids[set->size++] = new_int;
    return SET_SUCCESS;
}

int setGetSize(Set set)
{
    assert(set != NULL);
    return set->size;
}

bool setContains(Set set, int player_id)
{
    assert(set != NULL);
    return find(set, player_id) != ELEMENT_NOT_FOUND;
}

SetResult setRemove(Set set, int player_id)
{
    assert(set != NULL);

    int index = find(set, player_id);
    if (index == ELEMENT_NOT_FOUND)
    {
        return SET_SUCCESS;
    }
    set->ids[index] = set->ids[set->size - 1];
    set->size--;
    return SET_SUCCESS;
}

int setGetFirst(Set set)
{
    assert(set != NULL);
    set->iterator = 0;
    return setGetNext(set);
}

int setGetNext(Set set)
{
    assert(set != NULL);
    if (set->iterator >= set->size)
    {
        return SET_ENDED;
    }
    return set->ids[set->iterator++];
}


void setDestroy(Set set)
{
    if (set == NULL)
    {
        return;
    }
    while(setGetSize(set) > 0)
    {
    setRemove(set, setGetFirst(set));
    }
    free(set->ids);
    free(set);
}

//NEED TO FILL UP
Set setCopy(Set set)
{
    return NULL;
}