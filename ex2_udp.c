#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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
    int i, j, num;
    char str[10];
    char buf[5];
    //write the board game in line format to screen
    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            num = board[i][j];
            //convert number to char for printing
            strcpy(str, itoa(num, buf, 10));
            //add comma
            if (i != SIZE - 1 || j != SIZE - 1) {
                strcat(str, ",");
            }
            //write number to STDOUT
            if (write(1, str, strlen(str)) < strlen(str)) {
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