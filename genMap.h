#ifndef GENMAP_H
#define GENMAP_H
/*Struct to create 2d array for the map*/
typedef struct {
    int** mapArray;
    int rows;
    int cols;
} MapArray;

/*Struct to track player*/
typedef struct {
    int rows;
    int cols;
} PlayerPosition; 

/*Struct to track the state of the game/map*/
typedef struct {
    PlayerPosition player;
    MapArray *map;
    int gameResult; /*Game over or no*/
    int gameWon;
    int trapStatus; /*Track if the trap has been triggered or not*/
} GameState;

MapArray* genMapArray(char* fileName);
void displayMap(GameState* state);
void freeMap(MapArray* map);
void displayControl(void);
int validateMap(MapArray* map);
GameState* recordGameState(MapArray* map);
void freeGameState(void* gameStatePtr);

#endif
