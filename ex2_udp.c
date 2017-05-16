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

int main(int argc, char *argv[]) {

    __pid_t alertPid;
    int waitTime;
    if (argc < 2) {
        perror("not enough parameters");
    }

    alertPid = atoi(argv[1]);
    CreateBoard(&waitTime);

    //todo how the user is getting the maze
    if (kill(alertPid, SIGUSR1) < 0) {
        //todo handle error
    }

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
    srand(time(NULL)); //todo will work with 2 initilize or should i send as param from main
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
            pastTime = 1;
            //check movement and open thread to handle?????
            if (strcmp("up", key) == 0) {


            }
            if (strcmp("down", key) == 0) {


            }
            if (strcmp("left", key) == 0) {


            }
            if (strcmp("right", key) == 0) {


            }

        }
    }
}

void HandleClose(int sigNum) {

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