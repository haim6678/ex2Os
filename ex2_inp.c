#include <stdio.h>
#include <bits/signum.h>
#include <signal.h>

void PrintMatrix(int matrix[4][4]);
void HandleSiguser1(int sigNum);

int main() {
    int GameMatrix[4][4] = {0};

    //declare on the sigaction members
    struct sigaction InputAction;
    sigset_t inputBlock;

    //fill the sett with all signals
    sigfillset(&inputBlock);
    //remove the sigint signal
    sigdelset(&inputBlock, SIGINT);
    //set the handling function
    InputAction.sa_handler = HandleSiguser1;
    InputAction.sa_mask = inputBlock;
    InputAction.sa_flags = 0;
    //set for sigusr1
    sigaction(SIGUSR1, &InputAction, NULL);

    return 0;
}

void HandleSiguser1(int sigNum){

}

void PrintMatrix(int matrix[4][4]){
    int i=0;

    for(i;i<4;i++){
        printf("%s","|");
        int j=0;
        for(j;j<4;j++){
            printf("%s"," ");
            if((matrix[i][j])>0){
                printf("%04d",matrix[i][j]);
            }else{
                printf("%s","    ");
            }
            printf("%s"," ");
            printf("%s","|");
        }
        printf("%s","\n");
    }
}