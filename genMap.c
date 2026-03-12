#include <stdio.h>
#include <stdlib.h>
#include "genMap.h"
#include "color.h"

/*Reads the file and loads the values into a 2d array*/
MapArray* genMapArray(char* fileName)
{
    FILE* mapFile;
    MapArray* map;
    int i, j; 

    /*Allocate memory for map*/
    map = (MapArray *)malloc(sizeof(MapArray));
    if (map == NULL) {
        fprintf(stderr, "Error: Memory allocation failed!\n");
        return NULL;
    }

    /*Open the file for reading*/
    mapFile = fopen(fileName, "r");

    /*Throw the error if the file isn't present*/
    if (mapFile == NULL) 
    {
        fprintf(stderr, "Error: Cannot open input file '%s' \n", fileName);
        free(map);
        return NULL; 
    }

    /*Read the dimensions of the map*/
    if (fscanf(mapFile, "%d %d", &map->rows, &map->cols) != 2) {
        fprintf(stderr, "Error: Cannot read dimensions from the file\n");

        if (mapFile != stdin) {
            fclose(mapFile);
        }
        free(map);
        return NULL;
    }

    /*DEBUGGING*/
    #ifdef DEBUG
        printf(" %d %d\n", map->rows, map->cols);
    #endif

    /*Validate the dimensions of the map*/
    if (map->rows <= 0 || map->cols <= 0) {
        fprintf(stderr, "Error: Invalid dimensions (%d, %d)\n", map->rows, map->cols);

        if (mapFile != stdin) {
            fclose(mapFile);
        }
        free(map);
        return NULL;
    }

    /*Allocate memory for 2D array*/
    map->mapArray = (int **)malloc(map->rows * sizeof(int *));
    if (map->mapArray == NULL) {
        fprintf(stderr, "Error: Memory allocation failed during array allocation\n");

        if (mapFile != stdin) {
            fclose(mapFile);
        }
        free(map);
        return NULL;
    }

    /*Allocate memory for each row*/
    for (i = 0; i < map->rows; i++) {
        map->mapArray[i] = (int *)malloc(map->cols * sizeof(int));
        if(map->mapArray[i] == NULL) {
            fprintf(stderr, "Error: Memory allocation failed at rows\n");

            /*memory clean-up*/
            for (j = 0; j < i; j++) {
                free(map->mapArray[j]);
            }

            free(map->mapArray);

            if (mapFile != stdin) {
                fclose(mapFile);
            }
            free(map);
            return NULL;
        }
    }

    /*Read array data and store in array*/
    for (i = 0; i < map->rows; i++) {
        for (j = 0; j < map->cols; j++) {
            if (fscanf(mapFile, "%d", &map->mapArray[i][j]) != 1) {
                fprintf(stderr, "Error: Cannot read value at position (%d, %d)\n", i, j);

                /*memory clean-up*/
                for (j = 0; j < map->rows; j++) {
                    free(map->mapArray[j]);
                }

                free(map->mapArray);

                if (mapFile != stdin) {
                    fclose(mapFile);
                }

                free(map);
                return NULL;
            }

        }
    }

    /*Check for file errors*/
    if (ferror(mapFile)) {
        fprintf(stderr, "Error: File read error\n");

        /*memory clean-up*/
        for (j = 0; j < i; j++) {
            free(map->mapArray[j]);
        }

        free(map->mapArray);

        if (mapFile != stdin) {
            fclose(mapFile);
        }
        free(map);

        return NULL;
    }

    /*Close file*/
    if (mapFile != stdin) {
        fclose(mapFile);
    }

    return map;
}

/*Display format the playable map*/
void displayMap(GameState* state) {
    int i, j;
    MapArray* map = state->map;

    /*Vertical top border*/
    printf("\n*");

    for (i = 0; i < map->cols + 1; i++) {
        printf("*");
    }

    printf("\n");

    for(i = 0; i < map->rows; i++) {
        
        printf("*"); /*Horizontal left border*/

        /*Iterate through the 2D array and replace values with their corresponding symbols*/
        for (j = 0; j < map->cols; j++) {
            switch(map->mapArray[i][j]){
                case 0:
                    printf(" ");
                    break;
                case 1:
                    printf("O");
                    break;
                case 2:
                    setBackground("blue");
                    printf("~");
                    setBackground("reset");
                    break;
                case 3:
                    printf("X");
                    break;
                case 4:
                    setBackground("red");
                    printf("@");
                    setBackground("reset");
                    break;
                case 5: 
                    printf("P");
                    break;
                case 6:
                    setBackground("green");
                    printf("G");
                    setBackground("reset");
                    break;
                default:
                    printf("?");
                    break;
            }
        }
        printf("*\n"); /*Horizontal right border*/
    }

    /*Vertical bottom border*/
    for (i = 0; i < map->cols + 1; i++) {
        printf("*");
    }

    printf("*\n");

    /*Vertical bottom divider*/
    for (i = 0; i < map->cols + 1; i++) {
        printf("_");
    }

    printf("_\n\n");

    /*Display the control menu*/
    displayControl();
}

/*Display the control menu for the game*/
void displayControl(void) {
    printf("Press 'w' to move UP");
    printf("\nPress 's' to move DOWN");
    printf("\nPress 'a' to move LEFT");
    printf("\nPress 'd' to move RIGHT");
    printf("\nPress 'u' to UNDO move");
    printf("\n\n");
}

/*Check to ensure the map has all requirements*/
int validateMap(MapArray* map) {
    int i, j;
    int checkPlayer = 0;
    int checkGoal = 0;
    int checkTrap = 0;
    int checkWater = 0;
    int valid = 1;

    /*Checking that the map has a player, goal, trap, and water*/
    for (i = 0; i < map->rows; i++) {
        for (j = 0; j < map->cols; j++) {
            if (map->mapArray[i][j] == 5) {
                checkPlayer = 1;
            }
            if (map->mapArray[i][j] == 6) {
                checkGoal = 1;
            }
            if (map->mapArray[i][j] == 4) {
                checkTrap = 1;
            }
            if (map->mapArray[i][j] == 2) {
                checkWater = 1;
            }
        }
    }

    /*Print the corresponding error message*/
    if (!checkPlayer)
    {
        fprintf(stderr, "Map doesn't contain a player ('5')!\n");
        valid = 0;
    }
    if (!checkGoal)
    {
        fprintf(stderr, "Map doesn't contain a goal ('6')!\n");
        valid = 0;
    }
    if (!checkTrap)
    {
        fprintf(stderr, "Map doesn't contain a trap ('4')!\n");
        valid = 0;
    }
    if (!checkWater)
    {
        fprintf(stderr, "Map doesn't contain any water ('2')!\n");
        valid = 0;
    }

    return valid;
}

GameState* recordGameState(MapArray* map) {
    int i, j;
    GameState* state;
    GameState* result = NULL;

    /*Allocate memory for game state*/
    state = (GameState* )malloc(sizeof(GameState));
    if (state == NULL) {
        return NULL;
    }

    state->player.rows = -1; /*Temporary coordinates for player*/
    state->player.cols = -1; /*Temporary coordinates for player*/

    /*Initialise the other variables in GameState*/
    state->map = map;
    state->gameResult = 0;
    state->gameWon = 0;
    state->trapStatus = 0;

    /*Find the location of the player*/
    for (i = 0; i < map->rows; i++){
        for(j = 0; j < map->cols; j++) {
            if (map->mapArray[i][j] == 5) {
                state->player.rows = i;
                state->player.cols = j;
            }
        }
    }

    /*Check if the player was found*/
    if (state->player.rows == -1 || state->player.cols == -1) {
        fprintf(stderr, "Player not found!");
        free(state);
    } 
    else {
        result = state;
    }

    return result; 

}

/*Free the allocated memory for the game state*/
void freeGameState(void* gameStatePtr) {
    GameState* state = (GameState*)gameStatePtr;

    if (state == NULL) {
        return;
    }

    freeMap(state->map);
    free(state);

}

/*Free the allocated memory for the map*/
void freeMap(MapArray* map) {
    int i;
    
    if (map == NULL) {
        return;
    }

    if (map->mapArray != NULL) {
        for (i = 0; i < map->rows; i++) {
            free(map->mapArray[i]);
        }
        free(map->mapArray);
    }
    
    free(map);
}