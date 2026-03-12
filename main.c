#include <stdio.h>
#include <stdlib.h>
#include "genMap.h"
#include "gameFunc.h"
#include "terminal.h"
#include "newSleep.h"
#include "linkedLists.h"

int main(int argc, char *argv[])
{
    MapArray* map;
    GameState* currState;
    GameState* prevState;
    GameState* stateCopy;
    LinkedList* undoList;
    char input;
    int moveSuccess;

    /*Check for correct number of command line arguments*/
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <map_file>\n", argv[0]);
        return 1; 
    }

    /*Load the map 2d array*/
    map = genMapArray(argv[1]);
    if(map == NULL) {
        return 1;
    }
    
    /*Check that the map has all the necessary objects/characters*/
    if(!validateMap(map)) {
        freeMap(map);
        return 1;
    }

    /*Generate game state*/
    currState = recordGameState(map);
    if(currState == NULL){
        freeMap(map);
        return 1;
    }

    /*Initialise the undo linkedlist*/
    undoList = linkedList();
    if (undoList == NULL) {
        freeGameState(currState);
        return 1;
    }

    system("clear");

    /*Play Game*/
    while(!currState->gameResult) {
        /*Fresh screen and print map*/
        system("tput cup 0 0");
        displayMap(currState);

        /*Accept char input immediately*/
        disableBuffer();
        scanf(" %c", &input);
        enableBuffer();

        if (input == 'u'){
            prevState = (GameState*)pop(undoList);
            if (prevState != NULL) {
                freeGameState(currState);
                currState = prevState;
            }
            else {
                printf("Nothing to undo!\n\n");
            }
        }
        else if (input == 'w' || input == 's' || input == 'a' || input == 'd') {
            /*Save a copy of the current state before moving the player*/
            stateCopy = copyGS(currState);
            
            /*move player*/
            moveSuccess = 0;
            switch(input) {
                case 'w':
                    moveSuccess = move(currState, -1, 0);
                    break;
                case 's':
                    moveSuccess = move(currState, 1, 0);
                    break;
                case 'a':
                    moveSuccess = move(currState, 0, -1);
                    break;
                case 'd':
                    moveSuccess = move(currState, 0, 1);
                    break;
            }

            /*If the move was succesful and the statecopy isn't empty than stave to undo list*/
            if (moveSuccess && stateCopy != NULL) {
                push(undoList, stateCopy);
            } else {
                /*in the event player didn't move or the state wasn't copied*/
                if (stateCopy != NULL) {
                    freeGameState(stateCopy);
                }
            }
        }
        
    }

    #ifdef DEBUG
        printf("%d", moveSuccess);
    #endif

    system("clear");
    displayMap(currState);

    if(currState->gameWon){
        printf("YOU WIN!!!\n\n");
    } 
    else if (currState->gameResult) {
        printf("You lose...\n\n");
    }

    newSleep(1.25);

    /*Clean-up used memory*/
    freeGameState(currState);
    freeLL(undoList, freeGameState);
    map = NULL;
    
    return 0;
}