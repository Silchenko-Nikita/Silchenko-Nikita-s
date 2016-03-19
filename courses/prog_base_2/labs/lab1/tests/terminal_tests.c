#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include <cmocka.h>

#include "../terminal.h"
#include "tests.h"

static void new__id_printSett_wickets__relevantContent(void **state){
    Wickets_t wickets = Wickets_new();
    Terminal_t terminal = Terminal_new(1, TERMINAL_PRINT_STUDENTS, wickets, NULL);

    assert_int_equal(Terminal_getId(terminal), 1);
    assert_int_equal(Terminal_getPrintSettings(terminal), TERMINAL_PRINT_STUDENTS);

    Terminal_deepDelete(terminal);
}

static void setPrintSettings__printSett__relevantContent(void **state){
    Wickets_t wickets = Wickets_new();
    Terminal_t terminal = Terminal_new(1, TERMINAL_PRINT_STUDENTS, wickets, NULL);

    Terminal_setPrintSettings(terminal, TERMINAL_PRINT_NOBODY);

    assert_int_equal(Terminal_getPrintSettings(terminal), TERMINAL_PRINT_NOBODY);

    Terminal_deepDelete(terminal);
}

static void setWickets__wickets__relevantContent(void **state){
    Wickets_t wickets1 = Wickets_new();
    Wickets_t wickets2 = Wickets_new();
    Terminal_t terminal = Terminal_new(1, TERMINAL_PRINT_NOBODY, wickets1, NULL);
    Terminal_setWickets(terminal, wickets2);

    assert_true(wickets2 == Terminal_getWickets(terminal));

    Terminal_deepDelete(terminal);
}

void moduleTests_Terminal(void) {

    const UnitTest tests[] =
    {
        unit_test(new__id_printSett_wickets__relevantContent),
        unit_test(setPrintSettings__printSett__relevantContent),
        unit_test(setWickets__wickets__relevantContent)
    };

    return run_tests(tests);
}
