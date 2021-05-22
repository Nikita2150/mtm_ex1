#ifndef SET_H_
#define SET_H_
#include <stdbool.h>

typedef struct set* Set;

typedef enum {
SET_SUCCESS, SET_OUT_OF_MEMORY, SET_NULL_ARG
} SetResult;

Set setCreate();

void setDestroy(Set set);

SetResult setAdd(Set set, int player_id);

SetResult setRemove(Set set, int player_id);

bool setContains(Set set, int player_id);

int setGetSize(Set set);

int setGetFirst(Set set);
int setGetNext(Set set);

Set setCopy(Set set);

#endif