#include <stdio.h>
#include <bits/signum.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUFF_SIZE 256
#define MATRIX_ROW_SIZE 4
#define MATRIX_COL_SIZE 4
#define READ_SINE 79

void PrintMatrix();

void HandleSiguser1(int sigNum);

void HandleClose(int sigNum);

void GetFromStdin();

void SetSiguserSinal();

int GameMatrix[MATRIX_ROW_SIZE][MATRIX_COL_SIZE] = {0};


/**
 *
 * the output - 0 for success
 * the operation - runs the program
 */
int main() {


    int fd = open("boardFile.txt", O_CREAT | O_RDONLY, 0666);
    if (fd < 0) {
        //todo handle
    }
    if (dup2(fd, 0) < 0) {
        //todo handle
    }
    //declare on the sigaction members
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
        perror("faild in sigaction");
        exit(-1);
    }

    SetSiguserSinal();
    //run the process indefinitely
    while (1) {


    }
    return 0;
}

/**
 * the operation - this function define the beavihor of the handling
 * in the siuser signal
 */
void SetSiguserSinal() {
    struct sigaction InputAction;
    sigset_t inputBlock;

    sigfillset(&inputBlock);
    //set the handling function for siusr
    InputAction.sa_handler = HandleSiguser1;
    InputAction.sa_mask = inputBlock;
    InputAction.sa_flags = 0;
    //remove the wanted signals
    sigdelset(&inputBlock, SIGINT);

    if (sigaction(SIGUSR1, &InputAction, NULL) != 0) {
        perror("faild in sigaction");
        exit(-1);
    }
}

/**
 *
 *the input - the sigNum
 * the operation - this function is handling the recieve of the sigusr1 signal
 */
void HandleSiguser1(int sigNum) {

    SetSiguserSinal();
    GetFromStdin();
    PrintMatrix();
}

/**
 *
 *the input - the sigNum
 * the operation - this function is handling the recieve of the sigint signal
 */
void HandleClose(int sigNum) {
    printf("%s", "BYE BYE");
    exit(0);
}


/**
 * the operation - after getting a signal we need to get the current board
 * state,this function does this.
 */
void GetFromStdin() {
    int i = 0;
    int j = 0;
    printf("%s \n", "reading from file inp");
    char buff;
    char num[5];
    int number = 0;
    int numLength = 0;
    if (read(STDIN_FILENO, &buff, 1) < 0) {
        //todo handle error
    }
    while ((buff != '\0') && (buff != '\n')) {
        if (buff == ',') {
            number = atoi(num);
            numLength = 0;
            memset(num, 0, 5);
            GameMatrix[i][j] = number;
            j++;
            if (j == MATRIX_COL_SIZE) {
                i++;
                j = 0;
            }
            if (read(STDIN_FILENO, &buff, 1) < 0) {
                //todo handle error
            }
            continue;
        }
        if (numLength > 0) {
            strcat(num, &buff);
        } else {
            strcpy(num, &buff);
        }
        numLength++;
        if (read(STDIN_FILENO, &buff, 1) < 0) {
            //todo handle error
        }
    }
}

/**
 *
 *   the operation - this function is in charge of printing the board to the game
 */
void PrintMatrix() {
    int i = 0;
    char temp[32];
    //todo handle write errors
    printf("%s \n", "printing board");
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
    printf("%s \n", "finish printing board");
}