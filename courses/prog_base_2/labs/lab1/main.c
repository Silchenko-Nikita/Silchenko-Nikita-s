#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include <cmocka.h>

#include "console.h"
#include "wickets.h"
#include "list.h"
#include "tests/tests.h"

int main()
{
//    totalTests();
    srand(time(NULL));
    Wickets_t wickets = Wickets_new();
    Terminal_t terminal1 = Terminal_new(1, TERMINAL_PRINT_ALL, wickets, NULL);
    Terminal_t terminal2 = Terminal_new(2, TERMINAL_PRINT_TEACHERS, wickets, NULL);
    Terminal_t terminal3 = Terminal_new(3, TERMINAL_PRINT_STUDENTS | TERMINAL_PRINT_PERSONNEL, wickets, NULL);

    Wickets_addPasser(wickets, Person_new(PERSON_PERSONNEL, "John", "Galt", NULL));
    Wickets_emulatePasses(wickets, 3);
    Wickets_addPasser(wickets, Person_new(PERSON_STUDENT, "Nikita", "Silchenko", NULL));
    Wickets_emulatePasses(wickets, 2);
    Wickets_addPasser(wickets, Person_new(PERSON_TEACHER, "Ruslan", "Hadyniak", NULL));

    Console_initialize();

    Console_setSize(150, 50);

    Console_printTerminal(terminal1, 1, 1, 30, 40, BACKGROUND_WHITE, BACKGROUND_GREEN);
    Console_printTerminal(terminal2, 32, 1, 30, 40, BACKGROUND_WHITE, BACKGROUND_GREEN);
    Console_printTerminal(terminal3, 63, 1, 30, 40, BACKGROUND_WHITE, BACKGROUND_GREEN);

    Terminal_deepDelete(terminal1);
    Terminal_delete(terminal2);
    Terminal_delete(terminal3);

    Console_setDefaultTextColor();
    Console_putCursor(0, 45);
    return EXIT_SUCCESS;
}
