#include "tests.h"

void totalTests(void){
    puts("Person module tests:");
    moduleTests_Person();
    puts("\n\nList module tests:");
    moduleTests_List();
    puts("\n\nWickets module tests:");
    moduleTests_Wickets();
    puts("\n\nTerminal module tests:");
    moduleTests_Terminal();
}
