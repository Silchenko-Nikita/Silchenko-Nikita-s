int run(int moves[], int movesLen, int res[], int resLen){
    enum Oper{pop = -4, oContinue, repeat, oBreak, null};
    int values[4] = {7, 15, 25, 205};
    struct Command{int oper, state;};
    struct Command d[4][4];

    d[0][0].oper = pop;
    d[0][0].state = 1;

    d[0][1].oper = 1;
    d[0][1].state = 1;

    d[0][2].oper = oBreak;

    d[0][3].oper = 6;
    d[0][3].state = 0;

    d[1][0].oper = pop;
    d[1][0].state = 2;

    d[1][1].oper = oContinue;
    d[1][1].state = 0;

    d[1][2].oper = 2;
    d[1][2].state = 2;

    d[1][3].oper = repeat;
    d[1][3].state = 3;

    d[2][0].oper = null;

    d[2][1].oper = 4;
    d[2][1].state = 3;

    d[2][2].oper = null;

    d[2][3].oper = 3;
    d[2][3].state = 3;

    d[3][0].oper = oBreak;

    d[3][1].oper = 7;
    d[3][1].state = 0;

    d[3][2].oper = 5;
    d[3][2].state = 2;

    d[3][3].oper = oContinue;
    d[3][3].state = 3;

    int i, j, curState = 0, resPos = 0, check;

    for (i = 0; i < resLen; i++){
        res[i] = 0;
    }

    for (i = 0; i < movesLen; i++){
        check = 0;
        for (j = 0; j < 4; j++){
            if (moves[i] == values[j]){
                switch (d[curState][j].oper){
                    case pop:
                        if (resPos <= 0) goto end;

                        resPos--;
                        res[resPos] = 0;
                        curState = d[curState][j].state;
                        break;
                    case oContinue:
                        curState = d[curState][j].state;
                        break;
                    case repeat:
                        i--;
                        curState = d[curState][j].state;
                        break;
                    case oBreak:
                    case null:
                        goto end;
                    default:
                        if (resPos >= resLen) goto end;

                        res[resPos] = d[curState][j].oper;
                        resPos++;
                        curState = d[curState][j].state;
                    }
                check = 1;
                break;
            }
        }
        if (!check) break;
    }

    end:
    return resPos;
}
