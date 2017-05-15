#include <stdio.h>

void PrintMatrix(int matrix[4][4]);

int main() {
    int GameMatrix[4][4];
    PrintMatrix(GameMatrix);
    return 0;
}


void PrintMatrix(int matrix[4][4]){
    int i=0;

    for(i;i<4;i++){
        printf("%s","|");
        int j=0;
        for(j;j<4;j++){
            printf("%s"," ");
            if((matrix[i][j])>0){
                printf("%02d",matrix[i][j]);
            }else{
                printf("%s","\t");
            }
            printf("%s"," ");
            printf("%s","|");
        }
        printf("%s","\n");
    }
}