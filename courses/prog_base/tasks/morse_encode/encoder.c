#include<string.h>

char *morse_encode(char *signal, const char *message, int unit_len, int pad_len) {
    int i, j;
    const unsigned int len = strlen(signal);
    const n = 29;
	struct Map { char eng; const char *morse; } map[n];
	map[0].eng = 'A';
	map[0].morse = ".-";

	map[1].eng = 'B';
	map[1].morse = "-...";

	map[2].eng = 'C';
	map[2].morse = "-.-.";

	map[3].eng = 'D';
	map[3].morse = "-..";

	map[4].eng = 'E';
	map[4].morse = ".";

	map[5].eng = 'F';
	map[5].morse = "..-.";

	map[6].eng = 'G';
	map[6].morse = "--.";

	map[7].eng = 'H';
	map[7].morse = "....";

	map[8].eng = 'I';
	map[8].morse = "..";

	map[9].eng = 'J';
	map[9].morse = ".---";

	map[10].eng = 'K';
	map[10].morse = "-.-";

	map[11].eng = 'L';
	map[11].morse = ".-..";

	map[12].eng = 'M';
	map[12].morse = "--";

	map[13].eng = 'N';
	map[13].morse = "-.";

	map[14].eng = 'O';
	map[14].morse = "---";

	map[15].eng = 'P';
	map[15].morse = ".--.";

	map[16].eng = 'Q';
	map[16].morse = "--.-";

	map[17].eng = 'R';
	map[17].morse = ".-.";

	map[18].eng = 'S';
	map[18].morse = "...";

	map[19].eng = 'T';
	map[19].morse = "-";

	map[20].eng = 'U';
	map[20].morse = "..-";

	map[21].eng = 'V';
	map[21].morse = "...-";

	map[22].eng = 'W';
	map[22].morse = ".--";

	map[23].eng = 'X';
	map[23].morse = "-..-";

	map[24].eng = 'Y';
	map[24].morse = "-.--";

	map[25].eng = 'Z';
	map[25].morse = "--..";

	map[26].eng = ' ';
	map[26].morse = "|";

	map[27].eng = ',';
	map[27].morse = "--..--";

	map[28].eng = '.';
	map[28].morse = ".-.-.-";

	char morse[10000] = "", result[1000000] = "";
	for (i = 0; message[i + 1] != '\0'; i++) {
		for (j = 0; j < n; j++) {
			if (message[i] == map[j].eng){
                strncat(morse, map[j].morse, 10);
                strncat(morse, " ", 1);
                break;
            }
		}
	}
	for (j = 0; j < n; j++) {
			if (message[i] == map[j].eng){
                strncat(morse, map[j].morse, 10);
                break;
            }
		}
	for (i = 0; i < pad_len; i++) {
        strncat(result, "0", 1);
    }
	for (i = 0; morse[i] != '\0'; i++) {
        if (morse[i] == '.') {
            for (j = 0; j < unit_len; j++) {
                strncat(result, "1", 1);
            }
            if (morse[i + 1] == '.' || morse[i + 1] == '-'){
            for (j = 0; j < unit_len; j++) {
                strncat(result, "0", 1);
            }
        }
        } else if (morse[i] == '-') {
            for (j = 0; j < unit_len; j++) {
                strncat(result, "111", 3);
            }
            if (morse[i + 1] == '.' || morse[i + 1] == '-'){
            for (j = 0; j < unit_len; j++) {
                strncat(result, "0", 1);
            }
        }
        } else if (morse[i] == '|') {
            for (j = 0; j < unit_len; j++) {
                strncat(result, "0", 1);
            }
        } else{
            for (j = 0; j < unit_len; j++) {
                strncat(result, "000", 3);
            }
        }
	}

	for (i = 0; i < pad_len; i++) {
        strncat(result, "0", 1);
    }
    strncpy(signal, result, len);
	return signal;
}
