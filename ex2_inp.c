#include <stdio.h>
#include <bits/signum.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

#define MATRIX_ROW_SIZE 4
#define MATRIX_COL_SIZE 4

void PrintMatrix();

void HandleSiguser1(int sigNum);

void HandleClose(int sigNum);

void GetFromStdin();

void SetSiguserSinal();

int GameMatrix[MATRIX_ROW_SIZE][MATRIX_COL_SIZE] = {0};
int fd;

/**
 *
 * the output - 0 for success
 * the operation - runs the program
 */
int main() {

    //open file
    fd = open("boardFile.txt", O_CREAT | O_RDONLY, 0666);
    if (fd < 0) {
        perror("failed to open file");
        exit(-1);
    }
    if (dup2(fd, 0) < 0) {
        perror("failed to dup");
        exit(-1);
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
    printf("%s \n", "BYE BYE");
    exit(0);
}


/**
 * the operation - after getting a signal we need to get the current board
 * state,this function does this.
 */
void GetFromStdin() {
    int i = 0;
    int j = 0;
    char buff;
    char num[5];
    int number = 0;
    int numLength = 0;
    if (read(STDIN_FILENO, &buff, 1) < 0) {
        perror("failed to read from file");
        exit(-1);
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
                perror("failed to read from file");
                exit(-1);
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
            perror("failed to read from file");
            exit(-1);
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
    for (i; i < 4; i++) {
        write(STDOUT_FILENO, "|", strlen("|"));
        int j = 0;
        for (j; j < 4; j++) {
            if ((GameMatrix[i][j]) > 0) {
                memset(temp, 32, 0);
                sprintf(temp, "%04d", GameMatrix[i][j]);
                if(write(STDOUT_FILENO, temp, strlen(temp))<0){
                    perror("failed to write to file");
                    exit(-1);
                }
            } else {
                if(write(STDOUT_FILENO, "    ", strlen("    "))<0){
                    perror("failed to write to file");
                    exit(-1);
                }
            }
            if(write(STDOUT_FILENO, " ", strlen(" "))<0){
                perror("failed to write to file");
                exit(-1);
            }
            if(write(STDOUT_FILENO, "|", strlen("|"))<0){
                perror("failed to write to file");
                exit(-1);
            }
        }
        if(write(STDOUT_FILENO, "\n", strlen("\n"))<0){
            perror("failed to write to file");
            exit(-1);
        }
    }
}