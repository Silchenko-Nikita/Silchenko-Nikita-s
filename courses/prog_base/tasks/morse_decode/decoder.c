#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* morse_decode(char *message, const char *signal) {
    int i, j, k, nullCount = 0;
    const unsigned int len = strlen(signal);
    const n = 29;
	struct Map { char *eng; const char *morse; } map[n];
	map[0].eng = "A";
	map[0].morse = ".-";

	map[1].eng = "B";
	map[1].morse = "-...";

	map[2].eng = "C";
	map[2].morse = "-.-.";

	map[3].eng = "D";
	map[3].morse = "-..";

	map[4].eng = "E";
	map[4].morse = ".";

	map[5].eng = "F";
	map[5].morse = "..-.";

	map[6].eng = "G";
	map[6].morse = "--.";

	map[7].eng = "H";
	map[7].morse = "....";

	map[8].eng = "I";
	map[8].morse = "..";

	map[9].eng = "J";
	map[9].morse = ".---";

	map[10].eng = "K";
	map[10].morse = "-.-";

	map[11].eng = "L";
	map[11].morse = ".-..";

	map[12].eng = "M";
	map[12].morse = "--";

	map[13].eng = "N";
	map[13].morse = "-.";

	map[14].eng = "O";
	map[14].morse = "---";

	map[15].eng = "P";
	map[15].morse = ".--.";

	map[16].eng = "Q";
	map[16].morse = "--.-";

	map[17].eng = "R";
	map[17].morse = ".-.";

	map[18].eng = "S";
	map[18].morse = "...";

	map[19].eng = "T";
	map[19].morse = "-";

	map[20].eng = "U";
	map[20].morse = "..-";

	map[21].eng = "V";
	map[21].morse = "...-";

	map[22].eng = "W";
	map[22].morse = ".--";

	map[23].eng = "X";
	map[23].morse = "-..-";

	map[24].eng = "Y";
	map[24].morse = "-.--";

	map[25].eng = "Z";
	map[25].morse = "--..";

	map[26].eng = " ";
	map[26].morse = "|";

	map[27].eng = ",";
	map[27].morse = "--..--";

	map[28].eng = ".";
	map[28].morse = ".-.-.-";

	char morse[1000] = "", result[1000] = "";
	int currentUnit = 0, firstOneUnit = 0, firstNullUnit = 0, unitLenght = 0;
	for(k = 0; signal[k] == '0'; k++, nullCount++){}
	for(; signal[k] == '1'; k++, firstOneUnit++){}

	if(signal[k + nullCount] != '\0')
        for(; signal[k] == '0'; k++, firstNullUnit++){}

    if(firstOneUnit != 0 && firstNullUnit != 0){
        if(firstNullUnit/firstOneUnit == 3 && (firstNullUnit % firstOneUnit  == 0)){
            unitLenght = firstOneUnit;
        }
        else if(firstNullUnit/firstOneUnit == 7 && (firstNullUnit % firstOneUnit  == 0)){
            unitLenght = firstOneUnit;
        }
        else if(firstNullUnit*1.0/firstOneUnit == 7.0/3){
            unitLenght = firstOneUnit/3;
        }
        else if(firstOneUnit/firstNullUnit == 3 && (firstOneUnit % firstNullUnit  == 0)){
            unitLenght = firstNullUnit;
        }
    }

    for(;signal[k] == '1'; k++, currentUnit++){}

    if(firstNullUnit/currentUnit == 3 && (firstNullUnit % currentUnit  == 0)){
            unitLenght = currentUnit;
        }
        else if(firstNullUnit/currentUnit == 7 && (firstNullUnit % currentUnit  == 0)){
            unitLenght = currentUnit;
        }
        else if(firstNullUnit*1.0/currentUnit == 7.0/3){
            unitLenght = currentUnit/3;
        }
        else if(currentUnit/firstNullUnit == 3 && (currentUnit % firstNullUnit  == 0)){
            unitLenght = firstNullUnit;
        }

	while(signal[k + nullCount] !='\0' && !unitLenght){
        for(currentUnit = 0; signal[k] == '0'; k++, currentUnit++){}

        if(currentUnit/firstNullUnit == 3 && (currentUnit % firstNullUnit  == 0)){
            unitLenght = firstNullUnit;
            break;
        }
        if(currentUnit/firstNullUnit == 7 && (currentUnit % firstNullUnit  == 0)){
            unitLenght = firstNullUnit;
            break;
        }
        if(firstNullUnit*1.0/currentUnit == 7.0/3){
            unitLenght = currentUnit/3;
            break;
        }
        if(firstNullUnit/currentUnit == 7 && (firstNullUnit % currentUnit  == 0)){
            unitLenght = currentUnit;
            break;
        }
        if(currentUnit*1.0/firstNullUnit == 7.0/3){
            unitLenght = firstNullUnit/3;
            break;
        }
        if(firstNullUnit/currentUnit == 3 && (firstNullUnit % currentUnit  == 0)){
            unitLenght = currentUnit;
            break;
        }

        for(currentUnit = 0; signal[k] == '1'; k++, currentUnit++){}

        if(firstNullUnit/currentUnit == 3 && (firstNullUnit % currentUnit  == 0)){
            unitLenght = currentUnit;
            break;
        }
        if(firstNullUnit/currentUnit == 7 && (firstNullUnit % currentUnit  == 0)){
            unitLenght = currentUnit;
            break;
        }
        if(firstNullUnit*1.0/currentUnit == 7.0/3){
            unitLenght = currentUnit/3;
            break;
        }
        if(currentUnit/firstNullUnit == 3 && (currentUnit % firstNullUnit  == 0)){
            unitLenght = firstNullUnit;
            break;
        }
    }
    if(unitLenght == 0) unitLenght = firstOneUnit;

	for(j = 0, i = nullCount; signal[i] == '1'; i++, j++){}

        if(j / unitLenght == 1){
            strcat(morse, ".");
        }
        if(j / unitLenght == 3){
            strcat(morse, "-");
        }

	while(signal[i + nullCount] != '\0'){
        for(j = 0; signal[i] == '0'; i++, j++){}

        if(j / unitLenght == 3){
            strcat(morse, " ");
        }else if(j / unitLenght == 7){
            strcat(morse, "*|*");
        }

        for(j = 0; signal[i] == '1'; i++, j++){}
        if(j / unitLenght == 1){
            strcat(morse, ".");
        }else if(j / unitLenght == 3){
            strcat(morse, "-");
        }
	}

    char *l;
    for(l = strtok(morse, " *"); l != NULL; l = strtok(NULL, " *")){
        for(i = 0; i < n; i++){
            if(!strcmp(map[i].morse, l)){
                strcat(result, map[i].eng);
            }
        }
    }

	strcpy(message, result);
	return message;
}
