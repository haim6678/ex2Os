#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


#define MATRIX_ROW_SIZE 4
#define MATRIX_COL_SIZE 4

void ManageGame(int *wait, __pid_t pid);

int GameMatrix[MATRIX_ROW_SIZE][MATRIX_COL_SIZE];

void TimePassed(int *wait, __pid_t pid);

void HandleClose(int sigNum);

void PrintMazeLine();

void CreateBoard(int *waitTime);

void SetHandler();

void MoveUp();

void MoveDown();

void MoveLeft();

void MoveRight();

void PushToEmptyInCol(int i, int SecondI, int j, int direction);

void PushToEmptyInRow(int firstColLocatio, int designateColLoc, int i, int direction);

void CheckKey(char *key);

int Check2NeighborsInCol(int rowLocation, int secondRowLoc, int j, int direc);

void UpdateCountForUp(int *first, int *sec);

void UpdateCountForDown(int *first, int *sec);

int main(int argc, char *argv[]) {

    __pid_t alertPid;
    int waitTime;
    if (argc < 2) {
        perror("not enough parameters");
    }

    alertPid = atoi(argv[1]);
    //create the board
    CreateBoard(&waitTime);
    //set the handle in sigint
    SetHandler();
    //todo how the user is getting the maze


    ManageGame(&waitTime, alertPid);
    return 0;

}

void SetHandler() {
    struct sigaction CloseAction;
    sigset_t closeBlock;

    //fill the sett with all signals
    sigfillset(&closeBlock);

    //remove the wanted signals
    sigdelset(&closeBlock, SIGINT); //todo need this?

    //set the handling function for sigint
    CloseAction.sa_handler = HandleClose;
    CloseAction.sa_mask = closeBlock;
    CloseAction.sa_flags = 0;

    //set for sigint
    if (sigaction(SIGINT, &CloseAction, NULL) != 0) {
        perror("faild in sigaction");
        exit(-1);
    }

}

void TimePassed(int *wait, __pid_t pid) {
    int randomPlace;
    int placeX;
    int placeY;
    srand(time(NULL)); //todo will work with 2 initialize or should i send as param from main
    *wait = (rand() % 4) + 1;
    do {
        randomPlace = (rand() % 16);
        placeX = randomPlace / MATRIX_ROW_SIZE;
        placeY = randomPlace % MATRIX_ROW_SIZE;
    } while (GameMatrix[placeX][placeY] != 0);

    GameMatrix[placeX][placeY] = 2;
    PrintMazeLine();
    if (kill(pid, SIGUSR1) < 0) {
        //todo handle error
    }
}

void CheckKey(char *key) {
    memset(key, 0, 16);
    //todo check key and copy to arr
}

void ManageGame(int *wait, __pid_t pid) {
    int pastTime = 1;

    char key[16];
    while (1) {
        //time past
        if () {

            pastTime = 1;
            TimePassed(wait, pid);
        }
        //user pressed key
        if () {
            CheckKey(key);
            pastTime = 1;
            //todo check movement and open thread to handle?????
            if (strcmp("up", key) == 0) {
                MoveUp();

            } else if (strcmp("down", key) == 0) {

                MoveDown();
            } else if (strcmp("left", key) == 0) {
                MoveLeft();

            } else if (strcmp("right", key) == 0) {
                MoveRight();

            } else if (strcmp("new", key)) {
                CreateBoard(wait);
                if (kill(pid, SIGUSR1) < 0) {
                    //todo handle error
                }
            }
            //אם הם אותו מספר- מתאחדים. אם זה מספר שנוצר ע"י תזוזוה הוא לא מתאחד
        }
    }
}

void HandleClose(int sigNum) {
    write(1, "BYE BYE", strlen("BYE BYE"));
    //TODO handle error
    exit(0);
}

void PrintMazeLine() {
    int i = 0;
    int j;
    int temp;
    char arr[64];
    char buff[32];
    //write the board game in line format to screen
    for (i; i < MATRIX_ROW_SIZE; i++) {
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
                //TODO handle error
            }
        }
    }
}

void CreateBoard(int *waitTime) {

    //declare variables
    srand(time(NULL));
    int firstSquareRandom;
    int secondSquareRandom;
    int firstX;
    int firstY;
    int secondX;
    int secondY;
    //set all matrix to 0
    memset(GameMatrix, 0, MATRIX_COL_SIZE * MATRIX_ROW_SIZE);
    //get random waiting time
    *waitTime = (rand() % 4) + 1;
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
    GameMatrix[secondX][secondY] = 2;

    PrintMazeLine();

}

int Check2NeighborsInCol(int firstRowLocation, int designateRowLoc, int j, int direction) {

    if (GameMatrix[firstRowLocation][j] == GameMatrix[designateRowLoc][j]) {
        GameMatrix[designateRowLoc][j] = 2 * GameMatrix[designateRowLoc][j];
        GameMatrix[firstRowLocation][j] = 0;
        if (direction == 1) {
            PushToEmptyInCol(firstRowLocation + 1, firstRowLocation, j, direction);
        } else {
            (firstRowLocation - 1, firstRowLocation, j, direction);
        }
        return 1;
    }
    return 0;
}

void UpdateCountForUpAndLeft(int *first, int *sec) {
    (*first)++;
    (*sec)++;
}

void UpdateCountForDownAndRight(int *first, int *sec) {
    (*first)--;
    (*sec)--;
}

void PushToEmptyInCol(int firstRowLocation, int designateRowLoc, int j, int direction) {

    while ((firstRowLocation >= 0) && (firstRowLocation < MATRIX_ROW_SIZE) &&
           (designateRowLoc >= 0) && (designateRowLoc < MATRIX_ROW_SIZE)) {

        GameMatrix[designateRowLoc][j] = GameMatrix[firstRowLocation][j];
        GameMatrix[firstRowLocation][j] = 0;
        if (direction == 1) {
            UpdateCountForUp(&firstRowLocation, &designateRowLoc);

        } else {
            UpdateCountForDown(&firstRowLocation, &designateRowLoc);
        }

    }
}

void MoveUp() {
    int j = 0;
    int rowLocation;
    //iterate on every column
    for (j; j < MATRIX_COL_SIZE; j++) {

        //check from bottom to top on each row
        for (rowLocation = MATRIX_ROW_SIZE - 1; rowLocation <= 1; rowLocation--) {
            //if the row above has empty space then push it up and push everything from bottom to top
            if ((GameMatrix[rowLocation][j] != 0) && (GameMatrix[rowLocation - 1][j] == 0)) {
                PushToEmptyInCol(rowLocation, rowLocation - 1, j, 1);
                //if the row above has numeric val then check if need to unify them
            } else if ((GameMatrix[rowLocation][j] != 0) && (GameMatrix[rowLocation - 1][j] != 0)) {
                if (Check2NeighborsInCol(rowLocation, rowLocation - 1, j, 1) == 1) {
                    //if the next row has a numeric val the go over it
                    if (GameMatrix[rowLocation - 1][j] != 0) {
                        rowLocation--;
                    }
                }
            }
        }
    }
}

void MoveDown() {
    int j = 0;
    int rowLocation;
    //iterate on every column
    for (j; j < MATRIX_COL_SIZE; j++) {

        //check from top to bottom on each row
        for (rowLocation = 0; rowLocation < MATRIX_ROW_SIZE; rowLocation++) {
            //if the row above has empty space then push it up and push everything from bottom to top
            if ((GameMatrix[rowLocation][j] != 0) && (GameMatrix[rowLocation + 1][j] == 0)) {
                PushToEmptyInCol(rowLocation, rowLocation + 1, j, 0);
                //if the row above has numeric val then check if need to unify them
            } else if ((GameMatrix[rowLocation][j] != 0) && (GameMatrix[rowLocation + 1][j] != 0)) {
                if (Check2NeighborsInCol(rowLocation, rowLocation + 1, j, 0) == 1) {
                    if (GameMatrix[rowLocation + 1][j] != 0) {
                        rowLocation++;
                    }
                }
            }
        }
    }
}

void PushToEmptyInRow(int firstColLocatio, int designateColLoc, int i, int direction) {
    while ((firstColLocatio >= 0) && (firstColLocatio < MATRIX_COL_SIZE) &&
           (designateColLoc >= 0) && (designateColLoc < MATRIX_COL_SIZE)) {

        GameMatrix[i][firstColLocatio] = GameMatrix[i][designateColLoc];
        GameMatrix[i][firstColLocatio] = 0;
        if (direction == 1) {
            UpdateCountForUpAndLeft(&firstColLocatio, &designateColLoc);

        } else {
            UpdateCountForDownAndRight(&firstColLocatio, &designateColLoc);
        }

    }
}

void MoveLeft() {
    int i = 0;
    int colLocation;
    //iterate over all the rows
    for (i; i < MATRIX_ROW_SIZE; i++) {

        //for every row go all over the columns
        for (colLocation = MATRIX_COL_SIZE - 1; colLocation <= 0; colLocation--) {
            //if the row above has empty space then push it up and push everything from bottom to top
            if ((GameMatrix[i][colLocation] != 0) && (GameMatrix[i][colLocation - 1] == 0)) {
                PushToEmptyInRow(colLocation, colLocation - 1, i, 1);
                //if the row above has numeric val then check if need to unify them
            } else if ((GameMatrix[i][colLocation] != 0) && (GameMatrix[i][colLocation - 1] != 0)) {
                if (Check2NeighborsInRow(colLocation, colLocation - 1, i, 1) == 1) {
                    if (GameMatrix[i][colLocation - 1] != 0) {
                        colLocation++;
                    }
                }

            }
        }
    }

    void MoveRight() {

    }