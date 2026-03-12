#ifndef GAMEFUNC_H
#define GAMEFUNC_H

void actTrap(GameState* state);
void flood(GameState* state);
int checkPosition(GameState* state, int moveToRow, int moveToCol);
int move(GameState* state, int cRow, int cCol);
MapArray* copyMap(MapArray* origMap);
GameState* copyGS(GameState* origState);

#endif