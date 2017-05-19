#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>


#define MATRIX_ROW_SIZE 4
#define MATRIX_COL_SIZE 4
#define WRITE_SIZE 128

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
int fd;

/**
 *
 * the input: argc -num of args
 *             argv -the args
 * the output: sucess or not
 * the operation: the main function that runs the program
 */
int main(int argc, char *argv[]) {


    if (argc < 2) {
        perror("not enough parameters");
        exit(0);
    }

    //printf("%s \n", "open file upd");
    fd = open("boardFile.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        //todo handle
    }
    SecondProcessPid = atoi(argv[1]);
    //create the board
    //printf("%s \n", "create board upd");
    CreateBoard();
    //printf("%s \n", "print board in line upd");
    PrintMazeLine();
    //kill(SecondProcessPid, SIGUSR1);
    //set the handle in sigint
    SetHandler();
    //printf("%s \n", "run game upd");
    //run the game in a loop
    ManageGame();
    return 0;

}

/**
 * hte operation: setting the handler for the
 * sigint signal, and setting who is going to take
 * care of it.
 */
void SetHandler() {
    struct sigaction CloseAction;
    sigset_t closeBlock;

    //fill the sett with all signals
    sigfillset(&closeBlock);

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

/**
 *
 *the input - sig number
 *the operation - handles the alarm signal according
 * to what is define in the exersize.get a random
 * place on board and set it to be the value 2.
 */
void AlarmHandler(int sig) {
    //redefine the handler
    //printf("%s \n", "got alarm sig upd");
    SetAlarmSignal();
    srand(time(NULL));
    int temp;
    int XRandom;
    int YRandom;
    //make sure the spot is empty
    //printf("%s \n", " gessing new pos");
    do {
        temp = (rand() % 16);
        XRandom = temp / MATRIX_ROW_SIZE;
        YRandom = temp % MATRIX_ROW_SIZE;
    } while (GameMatrix[XRandom][YRandom] != 0);
    //printf("%s \n", "finish gessing new pos");
    //printf("%s \n", "new pos is:");
    //printf("%d,%d", XRandom, YRandom);
    GameMatrix[XRandom][YRandom] = 2;
    //write the board and notify the other process
    PrintMazeLine();
    if (kill(SecondProcessPid, SIGUSR1) < 0) {
        //todo handle error
    }

    waitTime = 5;
    //reset the wait time
    alarm(waitTime);
}

/**
 *
 *the input - sig number
 *the operation - handles the user key press  according
 * to what is define . moving the board to the wanted direction.
 */
void HandleUserInput(int sig) {

    //printf("%s \n", "we got input");
    //todo what if it's not legal key need also set alarm to 0?
    SetUsrSignal();
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
    alarm(0);
    //printf("%s \n", "convert board after getting movement");
    //write the board and notify the other process
    PrintMazeLine();
    if (kill(SecondProcessPid, SIGUSR1) < 0) {
        //todo handle
    }

}

void SetAlarmSignal() {
    struct sigaction alarmAction;
    sigset_t alarmBlock;

    //fill the sett with all signals
    sigfillset(&alarmBlock);
    sigdelset(&alarmBlock, SIGINT);
    //set the handling function for sigint

    alarmAction.sa_handler = AlarmHandler; //todo need to reset after every handle??
    alarmAction.sa_mask = alarmBlock;
    alarmAction.sa_flags = 0;

    //set for sigint
    if (sigaction(SIGALRM, &alarmAction, NULL) != 0) {
        perror("faild in sigaction");
        exit(-1);
    }
}

void SetUsrSignal() {
    struct sigaction alarmAction;
    sigset_t alarmBlock;

    //fill the sett with all signals
    sigfillset(&alarmBlock);
    sigdelset(&alarmBlock, SIGINT);
    //set the handling function for sigint

    alarmAction.sa_handler = HandleUserInput; //todo need to reset after every handle??
    alarmAction.sa_mask = alarmBlock;
    alarmAction.sa_flags = 0;

    //set for sigint
    if (sigaction(SIGUSR1, &alarmAction, NULL) != 0) {
        perror("faild in sigaction");
        exit(-1);
    }
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

/**
 * the operation_ the function that runs the game.
 * get user key and respond.
 * or wakes an alarm and reset the board.
 */
void ManageGame() {

    SetAlarmSignal();
    SetUsrSignal();
    __pid_t myPid = getpid();
    ////printf("%s \n", "get in the manage game loop");
    while (1) {

        waitTime = 5;
        alarm(waitTime);
        system("/bin/stty raw");
        inputDirection = getchar(); //todo what happend while waiting and then getting signal do we go back here or skip the getchr?
        system("/bin/stty cooked");
        kill(myPid, SIGUSR1);
        alarm(0);
        srand(time(NULL));
        waitTime = (rand() % 4) + 1;
    }
}

/**
 *
 * the input - sigNum
 * the operation - after getting sigint we shutting down thw
 * process.
 */
void HandleClose(int sigNum) {
    write(1, "BYE BYE", strlen("BYE BYE"));
    //TODO handle error
    exit(0);
}

/**
 * the operation - convert the current board situation
 * and write it down.
 */
void PrintMazeLine() {
    int i = 0;
    int j;
    int temp;
    char arr[WRITE_SIZE];//todo check if need to put /0 at start
    char buff[32];
    memset(arr,0,WRITE_SIZE);
    arr[0] = '\0';

    //write the board game in line format to screen
    for (i; i < MATRIX_ROW_SIZE; i++) {
        for (j = 0; j < MATRIX_COL_SIZE; j++) {
            temp = GameMatrix[i][j];

            memset(buff, 0, 32);

            snprintf(buff, 4, "%d", temp);
            strcat(arr, buff);
            //add comma
            if (i != MATRIX_ROW_SIZE - 1 || j != MATRIX_COL_SIZE - 1) {
                strcat(arr, ",");
            }
            //write number to STDOUT
            //if (write(fd, arr, strlen(arr)) < strlen(arr)) {
            //TODO handle error
            //}
        }
    }
    arr[strlen(arr)] = '\n';
    write(fd, arr, strlen(arr));
}

/**
 * the operation -  creating a new game board.
 */
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
    memset(GameMatrix, 0, sizeof(GameMatrix[0][0]) * MATRIX_COL_SIZE * MATRIX_ROW_SIZE);
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
    GameMatrix[secondX][secondY] = 2;


}


/**
 * the operation - moves the board up
 */
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

/**
 * the operation - moves the board down
 */
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

/**
 * the operation - moves the board left
 */
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

/**
 * the operation - moves the board right
 */
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

/**
 *
 * the input - firstRowLocation - firs col index
 *             designateRowLoc  - second col index
 *              i - the row index
 *             direction - say if we need to go left or right to col
 * the output - 1 if we combined 2 0 otherwise
 * the operation - gets 2 indexes in a given row and check if need to combine them
 *                 if we need then combine, and push the other to the empty space created.
 */
int Check2NeighborsInRow(int firstColLocatio, int designateColLoc, int i, int direction) {
    //check if similar
    if ((designateColLoc < MATRIX_COL_SIZE) && (designateColLoc >= 0) &&
        (GameMatrix[i][firstColLocatio] == GameMatrix[i][designateColLoc])) {
        GameMatrix[i][designateColLoc] = 2 * GameMatrix[i][designateColLoc];
        //check if need to go right or left and update counters and push to empty space
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

/**
 *
 * the input - firstRowLocation - firs row index
 *             designateRowLoc  - second row index
 *              j - the col index
 *             direction - say if we need to go up or down to col
 * the output - 1 if we combined 2 0 otherwise
 * the operation - gets 2 indexes in a given col and check if need to combine theb
 *                 if we need then combine and push the other to the empty space created.
 */
int Check2NeighborsInCol(int firstRowLocation, int designateRowLoc, int j, int direction) {
    //check if similar
    if ((designateRowLoc < MATRIX_ROW_SIZE) && (designateRowLoc >= 0) &&
        (GameMatrix[firstRowLocation][j] == GameMatrix[designateRowLoc][j])) {
        GameMatrix[designateRowLoc][j] = 2 * GameMatrix[designateRowLoc][j];
        GameMatrix[firstRowLocation][j] = 0;
        //check if need to go up or down and update counters and push to empty space
        if (direction == 1) {
            PushToEmptyInCol(firstRowLocation + 1, firstRowLocation, j, direction);
        } else {
            PushToEmptyInCol(firstRowLocation - 1, firstRowLocation, j, direction);
        }
        return 1;
    }
    return 0;
}

/**
 *
 * the input - firstRowLocation - firs col index
 *             designateRowLoc  - second col index
 *              i - the row index
 *             direction - say if we need to go left or right to col
 * the output - 1 if we combined 2 0 otherwise
 * the operation - gets 2 indexes in a given col and push in a loop everyone 1 step left/right.
 */
void PushToEmptyInRow(int firstColLocatio, int designateColLoc, int i, int direction) {
    //run in a loop until you finish with to row and move each one one step
    while ((firstColLocatio >= 0) && (firstColLocatio < MATRIX_COL_SIZE) &&
           (designateColLoc >= 0) && (designateColLoc < MATRIX_COL_SIZE)) {
        GameMatrix[i][designateColLoc] = GameMatrix[i][firstColLocatio];
        GameMatrix[i][firstColLocatio] = 0;
        //check if need to go right or left and update counters
        if (direction == 1) {
            UpdateCountForUpAndLeft(&firstColLocatio, &designateColLoc);

        } else {
            UpdateCountForDownAndRight(&firstColLocatio, &designateColLoc);
        }

    }
}

/**
 *
 * the input - firstRowLocation - firs row index
 *             designateRowLoc  - second row index
 *              j - the col index
 *             direction - say if we need to go up or down to col
 * the output - 1 if we combined 2 0 otherwise
 * the operation - gets 2 indexes in a given col and push in a loop everyone 1 step up/down.
 */
void PushToEmptyInCol(int firstRowLocation, int designateRowLoc, int j, int direction) {
    //run in a loop until you finish with to col and move each one one step
    while ((firstRowLocation >= 0) && (firstRowLocation < MATRIX_ROW_SIZE) &&
           (designateRowLoc >= 0) && (designateRowLoc < MATRIX_ROW_SIZE)) {
        GameMatrix[designateRowLoc][j] = GameMatrix[firstRowLocation][j];
        GameMatrix[firstRowLocation][j] = 0;
        //check if need to go up or down and update counters
        if (direction == 1) {
            UpdateCountForUpAndLeft(&firstRowLocation, &designateRowLoc);

        } else {
            UpdateCountForDownAndRight(&firstRowLocation, &designateRowLoc);
        }
    }
}

/**
 *
 * the input first - first counter
 *            sec  - second counter
 * the operation - gets 2 numbers and updates them upward
 */
void UpdateCountForUpAndLeft(int *first, int *sec) {
    (*first)++;
    (*sec)++;
}

/**
 *
 * the input first - first counter
 *            sec  - second counter
 * the operation - gets 2 numbers and updates them down
 */
void UpdateCountForDownAndRight(int *first, int *sec) {
    (*first)--;
    (*sec)--;
}
