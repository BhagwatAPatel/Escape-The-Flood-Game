#include <stdlib.h>
#include <stdio.h>
#include "genMap.h"

/*Activate trapdoor*/
void actTrap(GameState* state){
    int i, j;
    MapArray* map = state->map;

    /*Find all positions with trapdoors and traps and replace them with " "*/
    for(i = 0; i < map->rows; i++) {
        for(j = 0; j < map->cols; j++) {
            if (map->mapArray[i][j] == 3 || map->mapArray[i][j] == 4) {
                map->mapArray[i][j] = 0;
            }
        }
    }

    state->trapStatus = 1;
}

/*Flooding Algorithm*/
void flood(GameState* state) {
    MapArray* map = state->map;
    int i, j;

    for(i = 0; i < map->rows; i++){
        for (j = 0; j < map->cols; j++){
            if (map->mapArray[i][j] == 2) {
                /*check above it*/
                if (i > 0 && map->mapArray[i-1][j] != 1 && map->mapArray[i-1][j] != 2) {
                    map->mapArray[i-1][j] = -1;
                }
                /*check below it*/
                if (i < map->rows - 1 && map->mapArray[i+1][j] != 1 && map->mapArray[i+1][j] != 2) {
                    map->mapArray[i+1][j] = -1;
                }
                /*check left of it*/
                if (j > 0 && map->mapArray[i][j-1] != 1 && map->mapArray[i][j-1] != 2) {
                    map->mapArray[i][j-1] = -1;
                }
                /*check right of it*/
                if (j < map->cols - 1 && map->mapArray[i][j+1] != 1 && map->mapArray[i][j+1] != 2) {
                    map->mapArray[i][j+1] = -1;
                }
            }
        }
    }

    for(i = 0; i < map->rows; i++){
        for (j = 0; j < map->cols; j++){
            if (map->mapArray[i][j] == -1) {
                map->mapArray[i][j] = 2;
            }
        }
    }

}


/*Check if the position the player attempts to move to is available*/
int checkPosition(GameState* state, int moveToRow, int moveToCol) {
    MapArray* map = state->map;
    int targetPosition;
    int result = 1;

    #ifdef DEBUG
        printf("%d %d", moveToRow, moveToCol);
    #endif

    /*check the map size*/
    if (moveToRow < 0 || moveToRow >= map->rows || moveToCol < 0 || moveToCol >= map->cols) {
        result = 0;
    }
    else {

        /*Check the objects trying to be walked into*/
        targetPosition = map->mapArray[moveToRow][moveToCol];

        /*1 = Wall, 3 = Trapdoor*/
        if (targetPosition == 1 || (targetPosition == 3 && state->trapStatus == 0)) {
            result = 0;
        }
    }

    return result;
}

/*Move the player*/
int move(GameState* state, int cRow, int cCol) {
    int moveToRow, moveToCols, targetPosition, result;
    MapArray* map = state->map;

    moveToRow = state->player.rows + cRow;
    moveToCols = state->player.cols + cCol;

    if(!checkPosition(state, moveToRow, moveToCols)) {
        result = 0;
    } 
    else {
        targetPosition = map->mapArray[moveToRow][moveToCols];

        map->mapArray[state->player.rows][state->player.cols] = 0; /*reset previous position to empty*/

        /*Check if the trap is activated*/
        if (targetPosition == 4) {
            actTrap(state);
        }

        /*Check if the goal is reached*/
        if (targetPosition == 6) { 
            state->gameResult = 1;
            state->gameWon = 1;
        }

        /*Actually move the player now*/
        state->player.rows = moveToRow;
        state->player.cols = moveToCols;
        map->mapArray[moveToRow][moveToCols] = 5;

        /*if the trap is activated, begin flooding*/ 
        if(state->trapStatus) {
            flood(state);

            /*if player in water, end game*/
            if(map->mapArray[state->player.rows][state->player.cols] == 2) {
                state->gameResult = 1;
                state->gameWon = 0;
            }
        }

        result = 1;
    }

    return result;
}


/*UNDO FEATURE CODE*/

/*Create a copy of the map to record what the map was like before the player made a move*/
MapArray* copyMap(MapArray* origMap) {
    MapArray* mapCopy;
    MapArray* result;

    int i, j;

    mapCopy = (MapArray*)malloc(sizeof(MapArray));
    if (mapCopy == NULL) {
        result = NULL;
    }
    else {
        /*copy row and col size*/
        mapCopy->rows = origMap->rows;
        mapCopy->cols = origMap->cols;

        /*Allocate memory for the copy*/
        mapCopy->mapArray = (int**)malloc(mapCopy->rows * sizeof(int*));
        /*To end program if the memory allocation fails*/
        if (mapCopy->mapArray == NULL) {
            free(mapCopy);
            result = NULL;
        } 
        else {
            for (i = 0; i < mapCopy->rows; i++) {
                mapCopy->mapArray[i] = (int*)malloc(mapCopy->cols * sizeof(int));
                /*To end program if the memory allocation fails*/
                if(mapCopy->mapArray[i] == NULL) {
                    for (j = 0; j < i; j++) {
                        free(mapCopy->mapArray[j]);
                    }
                    free(mapCopy->mapArray);
                    free(mapCopy);
                    result = NULL;
                } 
                /*if everything works and the memory is available, then copy over the data from the orignal/current map to the copy map*/
                else {
                    for (j = 0; j < mapCopy->cols; j++) {
                        mapCopy->mapArray[i][j] = origMap->mapArray[i][j];
                        result = mapCopy;
                    }
                }
            }
        }
    }

    return result;
}

/*Create a copy of the game state to record what the game state was like before the player made a move*/
GameState* copyGS(GameState* origState) {
    GameState* stateCopy;
    GameState* result;

    /*Check the original contains data/isn't null*/
    if (origState == NULL) {
        result = NULL;
    } 
    else {
        /*Allocate the memory for the copy*/
        stateCopy = (GameState*)malloc(sizeof(GameState));
        
        /*Check if the copy isn't null/memory allocated*/
        if (stateCopy == NULL) {
            result = NULL;
        }
        else {
            stateCopy->map = copyMap(origState->map); /*Copy original map into the copied state*/

            /*Check that the map was actually copied*/
            if (stateCopy->map == NULL) {
                free(stateCopy);
                result = NULL;
            }
            else {
                stateCopy->player.rows = origState->player.rows; /*Copy the players location*/
                stateCopy->player.cols = origState->player.cols; /*Copy the players location*/
                stateCopy->gameResult = origState->gameResult; /*Copy the game result*/
                stateCopy->gameWon = origState->gameWon; /*Copy the game won status*/
                stateCopy->trapStatus = origState->trapStatus; /*Copy the trap status*/

                result = stateCopy;
            }

        }
    } 
    
    return result;
}