#include <stdio.h>
#include <stdlib.h>

void fillRand(int mat[4][4]){
    int i, j, rand1;

    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            rand1 = (rand()*999)/RAND_MAX;
            mat[i][j] = ((rand()*999)/RAND_MAX >= 500) ? rand1 : -rand1;
        }
    }
}

void rotateCCW270(int mat[4][4]){
    int i, j, mat1[4][4];

    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            mat1[j][3 - i] = mat[i][j];
        }
    }

    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            mat[i][j] = mat1[i][j];
        }
    }
}

void flipV(int mat[4][4]){
    int i, j, mat1[4][4], q;

    for(i = 0; i < 2; i++){
        for(j = 0; j < 4; j++){
            q = mat[i][j];
            mat[i][j] = mat[3 - i][j];
            mat[3 - i][j] = q;
        }
    }
}

void transposSide(int mat[4][4]){
    int i, j, mat1[4][4], q;

    for(i = 0; i < 4; i++){
        for(j = 0; j < 4 - i; j++){
            q = mat[i][j];
            mat[i][j] = mat[3 - j][3 - i];
            mat[3 - j][3 - i] = q;
        }
    }
}
