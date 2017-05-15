#include <stdio.h>
#include <bits/signum.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define BUFF_SIZE 256
#define MATRIX_ROW_SIZE 4
#define MATRIX_COL_SIZE 4

void PrintMatrix(int matrix[4][4]);

void HandleSiguser1(int sigNum);

void HandleClose(int sigNum);

void GetFromStdin();

int GameMatrix[MATRIX_ROW_SIZE][MATRIX_COL_SIZE] = {0};

int main() {

    //declare on the sigaction members
    struct sigaction InputAction;
    struct sigaction CloseAction;
    sigset_t inputBlock;
    sigset_t closeBlock;

    //fill the sett with all signals
    sigfillset(&inputBlock);
    sigfillset(&closeBlock);

    //remove the wanted signals
    sigdelset(&inputBlock, SIGINT);
    sigdelset(&inputBlock, SIGUSR1);//todo need this?
    sigdelset(&closeBlock, SIGINT);

    //set the handling function for siusr
    InputAction.sa_handler = HandleSiguser1;
    InputAction.sa_mask = inputBlock;
    InputAction.sa_flags = 0;
    //set the handling function for sigint
    CloseAction.sa_handler = HandleClose;
    CloseAction.sa_mask = closeBlock;
    CloseAction.sa_flags = 0;

    //set for sigusr1
    if (sigaction(SIGUSR1, &InputAction, NULL) != 0) {
        perror();
    }
    if (sigaction(SIGINT, &CloseAction, NULL) != 0) {
        perror();
    }

    PrintMatrix(GameMatrix);
    return 0;
}

void HandleSiguser1(int sigNum) {
    GetFromStdin();
}

void HandleClose(int sigNum){
    printf("%s","BYE BYE");
    exit(0);
}
void GetFromStdin() {
    int i = 0;
    int j = 0;
    char buff[BUFF_SIZE];
    if (read(STDIN_FILENO, buff, BUFF_SIZE) < 0) {
        //todo handle error
    }
    char *numArr = strtok(buff, ",");

    while ((numArr != NULL) && (i < MATRIX_ROW_SIZE * MATRIX_COL_SIZE)) {
        GameMatrix[i][j] = atoi(numArr);

        numArr = strtok(NULL, ",");

        j++;
        if (j % 4 == 0) {
            j = 0;
            i++;
        }
    }
}

void PrintMatrix(int matrix[4][4]) {
    int i = 0;
    char temp[32];
    //todo handle write errors
    for (i; i < 4; i++) {
        write(STDOUT_FILENO, "|", strlen("|"));
        int j = 0;
        for (j; j < 4; j++) {
            printf("%s", " ");
            if ((matrix[i][j]) > 0) {
                memset(temp, 32, 0);
                sprintf(temp, "%04d", matrix[i][j]);
                write(STDOUT_FILENO, temp, strlen(temp));
            } else {
                write(STDOUT_FILENO, "    ", strlen("    "));
            }
            write(STDOUT_FILENO, " ", strlen(" "));
            write(STDOUT_FILENO, "|", strlen("|"));
        }
        write(STDOUT_FILENO, "\n", strlen("\n"));
    }
}