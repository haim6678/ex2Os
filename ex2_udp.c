#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>


#define MATRIX_ROW_SIZE 4
#define MATRIX_COL_SIZE 4

void ManageGame();

int GameMatrix[MATRIX_ROW_SIZE][MATRIX_COL_SIZE];

void HandleClose(int sigNum);

void PrintMazeLine();

void CreateBoard();

void SetHandler();

void MoveUp();

void MoveDown();

void MoveLeft();

void MoveRight();

void PushToEmptyInCol(int i, int SecondI, int j, int direction);

void PushToEmptyInRow(int firstColLocatio, int designateColLoc, int i, int direction);

int Check2NeighborsInCol(int rowLocation, int secondRowLoc, int j, int direc);

int Check2NeighborsInRow(int colLocation, int secLocation, int i, int direction);

void UpdateCountForUpAndLeft(int *first, int *sec);

void UpdateCountForDownAndRight(int *first, int *sec);

void HandleUserInput(int sig);

void AlarmHandler(int sig);

int inputDirection;
unsigned int waitTime;
__pid_t SecondProcessPid;

int main(int argc, char *argv[]) {


    /*if (argc < 2) {
        perror("not enough parameters");
    }*/

    //SecondProcessPid = atoi(argv[1]);
    //create the board
    CreateBoard();
    //kill(SecondProcessPid, SIGUSR1);
    //set the handle in sigint
    SetHandler();
    //todo how the user is getting the maze

    ManageGame();
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
        perror("failed in sigaction");
        exit(-1);
    }
}


void AlarmHandler(int sig) {
    signal(SIGALRM, AlarmHandler);
    srand(time(NULL));
    int temp;
    int XRandom;
    int YRandom;

    do {
        temp = (rand() % 16);
        XRandom = temp / MATRIX_ROW_SIZE;
        YRandom = temp % MATRIX_ROW_SIZE;
    } while (GameMatrix[XRandom][YRandom] != 0);

    GameMatrix[XRandom][YRandom] = 2;
    PrintMazeLine();
    if (kill(SecondProcessPid, SIGUSR1) < 0) {
        //todo handle error
    }
    alarm(waitTime);
}

void HandleUserInput(int sig) {

    //todo what if it's not legal key need also set alarm to 0?
    signal(SIGUSR1, HandleUserInput);
    alarm(0);
    switch (inputDirection) {
        case 'w' : //todo small and capital letters?
        case 'W':
            MoveUp();
            break;
        case 'x':
        case 'X':
            MoveDown();
            break;
        case 'a':
        case 'A':
            MoveLeft();
            break;
        case 'd':
        case 'D':
            MoveRight();
            break;
        case 's':
        case 'S':
            CreateBoard(); //todo check if will work
            break;
        default:
            break;
    }
    // kill(SecondProcessPid, SIGUSR1);
    int i = 0;
    Printt();
}

void Printt() {
    char temp[32];
    int i = 0;
    //todo handle write errors
    for (i; i < 4; i++) {
        write(STDOUT_FILENO, "|", strlen("|"));
        int j = 0;
        for (j; j < 4; j++) {
            printf("%s", " ");
            if ((GameMatrix[i][j]) > 0) {
                memset(temp, 32, 0);
                sprintf(temp, "%04d", GameMatrix[i][j]);
                write(STDOUT_FILENO, temp, strlen(temp));
            } else {
                write(STDOUT_FILENO, "    ", strlen("    "));
            }
            write(STDOUT_FILENO, " ", strlen(" "));
            write(STDOUT_FILENO, "|", strlen("|"));
        }
        write(STDOUT_FILENO, "\n", strlen("\n"));
    }
    write(STDOUT_FILENO, "\n", strlen("\n"));
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void ManageGame() {

    signal(SIGALRM, AlarmHandler); //todo do i need to block other signals while handling?
    signal(SIGUSR1, HandleUserInput); //todo do i need to block other signals while handling?
    __pid_t myPid = getpid();
    int i = 0;
    char temp[32];
    GameMatrix[0][0] = 4;
    GameMatrix[1][0] = 4;
    GameMatrix[0][1] = 16;
    GameMatrix[0][3] = 4;
    Printt();
    while (1) {
        waitTime = 100;
        if (i > 1) {
            i = 0;
            int temp;
            int XRandom;
            int YRandom;

            do {
                temp = (rand() % 16);
                XRandom = temp / MATRIX_ROW_SIZE;
                YRandom = temp % MATRIX_ROW_SIZE;
            } while (GameMatrix[XRandom][YRandom] != 0);

           // GameMatrix[XRandom][YRandom] = 2;
        }
        //alarm(waitTime);
        system("/bin/stty raw");
        inputDirection = getchar(); //todo what happend while waiting and then getting signal do we go back here or skip the getchr?
        system("/bin/stty cooked");
        kill(myPid, SIGUSR1);
        alarm(0);
        srand(time(NULL));
        waitTime = (rand() % 4) + 1;
        i++;
        //PrintMazeLine(); //make shure
        //if (kill(SecondProcessPid, SIGUSR1) < 0) {
        //todo handle error
        //}
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

void CreateBoard() {

    //declare variables
    srand(time(NULL));
    int firstSquareRandom;
    int secondSquareRandom;
    int firstX;
    int firstY;
    int secondX;
    int secondY;
    //set all matrix to 0
    memset(GameMatrix, 0, sizeof(GameMatrix[0][0])*MATRIX_COL_SIZE * MATRIX_ROW_SIZE);
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
    //GameMatrix[firstX][firstY] = 2;
    //GameMatrix[secondX][secondY] = 2;

    //PrintMazeLine();
}

int Check2NeighborsInCol(int firstRowLocation, int designateRowLoc, int j, int direction) {

    if ((designateRowLoc < MATRIX_ROW_SIZE) && (designateRowLoc >= 0) &&
        (GameMatrix[firstRowLocation][j] == GameMatrix[designateRowLoc][j])) {
        GameMatrix[designateRowLoc][j] = 2 * GameMatrix[designateRowLoc][j];
        GameMatrix[firstRowLocation][j] = 0;
        if (direction == 1) {
            PushToEmptyInCol(firstRowLocation + 1, firstRowLocation, j, direction);
        } else {
            PushToEmptyInCol(firstRowLocation - 1, firstRowLocation, j, direction);
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
        Printt();
        GameMatrix[designateRowLoc][j] = GameMatrix[firstRowLocation][j];
        GameMatrix[firstRowLocation][j] = 0;
        if (direction == 1) {
            UpdateCountForUpAndLeft(&firstRowLocation, &designateRowLoc);

        } else {
            UpdateCountForDownAndRight(&firstRowLocation, &designateRowLoc);
        }
    }
}

void MoveUp() {
    int j = 0;
    int rowLocation;
    //iterate on every column
    for (j; j < MATRIX_COL_SIZE; j++) {
        //check from bottom to top on each row
        for (rowLocation = MATRIX_ROW_SIZE - 1; rowLocation >= 0; rowLocation--) {
            //if the row above has empty space then push it up and push everything from bottom to top
            if ((GameMatrix[rowLocation][j] != 0) && (GameMatrix[rowLocation - 1][j] == 0)) {
                PushToEmptyInCol(rowLocation, rowLocation - 1, j, 1);
                //if the row above has numeric val then check if need to unify them
            } else if ((GameMatrix[rowLocation][j] != 0) && (GameMatrix[rowLocation - 1][j] != 0)) {
                if (Check2NeighborsInCol(rowLocation, rowLocation - 1, j, 1) == 1) {
                    //if the next row has a numeric val the go over it
                    if ((rowLocation - 2 >= 0) && (GameMatrix[rowLocation - 2][j] != 0)) {
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
                    if ((rowLocation + 2 < MATRIX_ROW_SIZE) && (GameMatrix[rowLocation + 2][j] != 0)) {
                        rowLocation++;
                    }
                }
            }
        }
    }
}

int Check2NeighborsInRow(int firstColLocatio, int designateColLoc, int i, int direction) {
    if ((designateColLoc < MATRIX_COL_SIZE) && (designateColLoc >= 0) &&
        (GameMatrix[i][firstColLocatio] == GameMatrix[i][designateColLoc])) {
        GameMatrix[i][designateColLoc] = 2 * GameMatrix[i][designateColLoc];
        GameMatrix[i][firstColLocatio] = 0;
        if (direction == 1) {
            PushToEmptyInRow(firstColLocatio + 1, firstColLocatio, i, direction);
        } else {
            PushToEmptyInRow(firstColLocatio - 1, firstColLocatio, i, direction);
        }
        return 1;
    }
    return 0;
}

void PushToEmptyInRow(int firstColLocatio, int designateColLoc, int i, int direction) {
    while ((firstColLocatio >= 0) && (firstColLocatio < MATRIX_COL_SIZE) &&
           (designateColLoc >= 0) && (designateColLoc < MATRIX_COL_SIZE)) {


        GameMatrix[i][designateColLoc] = GameMatrix[i][firstColLocatio];
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
        for (colLocation = MATRIX_COL_SIZE - 1; colLocation >= 0; colLocation--) {
            //if the row above has empty space then push it up and push everything from bottom to top
            if ((GameMatrix[i][colLocation] != 0) && (GameMatrix[i][colLocation - 1] == 0)) {
                PushToEmptyInRow(colLocation, colLocation - 1, i, 1);
                //if the row above has numeric val then check if need to unify them
            } else if ((GameMatrix[i][colLocation] != 0) && (GameMatrix[i][colLocation - 1] != 0)) {
                if (Check2NeighborsInRow(colLocation, colLocation - 1, i, 1) == 1) {
                    if ((colLocation - 2 >= 0) && (GameMatrix[i][colLocation - 2] != 0)) {
                        colLocation--;
                    }
                }
            }
        }
    }
}

void MoveRight() {
    int i = 0;
    int colLocation;
    //iterate over all the rows
    for (i; i < MATRIX_ROW_SIZE; i++) {
        //for every row go all over the columns
        for (colLocation = 0; colLocation < MATRIX_COL_SIZE; colLocation++) {
            //if the row above has empty space then push it up and push everything from bottom to top
            if ((GameMatrix[i][colLocation] != 0) && (GameMatrix[i][colLocation + 1] == 0)) {
                PushToEmptyInRow(colLocation, colLocation + 1, i, 0);
                //if the row above has numeric val then check if need to unify them
            } else if ((GameMatrix[i][colLocation] != 0) && (GameMatrix[i][colLocation + 1] != 0)) {
                if (Check2NeighborsInRow(colLocation, colLocation + 1, i, 0) == 1) {
                    if ((colLocation + 2 < MATRIX_COL_SIZE) && (GameMatrix[i][colLocation + 2] != 0)) {
                        colLocation++;
                    }
                }
            }
        }
    }
}