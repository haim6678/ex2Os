#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

void CreateBoard();

#define MATRIX_ROW_SIZE 4
#define MATRIX_COL_SIZE 4
int GameMatrix[MATRIX_ROW_SIZE][MATRIX_COL_SIZE];

void PrintMazeLine();

int main(int argc, char *argv[]) {

    if (argc < 2) {
        perror("not enough parameters");
    }

    __pid_t alertPid = atoi(argv[1]);

    return 0;

}

void PrintMazeLine() {
    int i=0;
    int j;
    int temp;
    char arr[64];
    char buff[32];
    //write the board game in line format to screen
    for (i ; i < MATRIX_ROW_SIZE; i++) {
        for (j = 0; j < MATRIX_COL_SIZE; j++) {
            temp = GameMatrix[i][j];

            snprintf(buff, 32, "%04d", temp);
            strcpy(arr, buff);
            //add comma
            if (i != MATRIX_ROW_SIZE - 1 || j != MATRIX_COL_SIZE - 1) {
                strcat(arr, ",");
            }
            //write number to STDOUT
            if (write(1, arr, strlen(arr)) < strlen(arr)) {
                //TODO error write
            }
        }
    }
}

void CreateBoard() {

    //declare variables
    srand(time(NULL));
    int waitTime;
    int firstSquareRandom;
    int secondSquareRandom;
    int firstX;
    int firstY;
    int secondX;
    int secondY;
    //set all matrix to 0
    memset(GameMatrix, 0, MATRIX_COL_SIZE * MATRIX_ROW_SIZE);

    //get random waiting time
    waitTime = (rand() % 4) + 1;
    //get random 2 squares
    firstSquareRandom = (rand() % 16);
    secondSquareRandom = (rand() % 16);
    //make sure they are different
    while (firstSquareRandom == secondSquareRandom) {
        secondSquareRandom = (rand() % 16);
    }
    firstX = firstSquareRandom / MATRIX_ROW_SIZE;
    firstY = firstSquareRandom % MATRIX_ROW_SIZE;
    secondX = secondSquareRandom / MATRIX_ROW_SIZE;
    secondY = secondSquareRandom % MATRIX_ROW_SIZE;
    GameMatrix[firstX][firstY] = 2;
    GameMatrix[secondX][secondY = 2;

}