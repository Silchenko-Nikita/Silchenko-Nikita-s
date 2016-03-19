#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include <cmocka.h>

#include "tests.h"
#include "../wickets.h"

static void new__void__noPassers(void **state){
    Wickets_t wickets = Wickets_new();

    assert_int_equal(Wickets_getPassersNumber(wickets), 0);

    Wickets_delete(wickets);
}

static void addPasser__passer__relevantPassersNumber(void **state){
    Wickets_t wickets = Wickets_new();

    Wickets_addPasser(wickets, Person_newRandom());
    assert_int_equal(Wickets_getPassersNumber(wickets), 1);

    Wickets_addPasser(wickets, Person_newRandom());
    assert_int_equal(Wickets_getPassersNumber(wickets), 2);

    Wickets_deepDelete(wickets);
}

static void getPasser__index__getEqualsAdd(void **state){
    Wickets_t wickets = Wickets_new();
    Person_t person = Person_newRandom();

    Wickets_addPasser(wickets, person);
    Person_t got = Wickets_getPasser(wickets, 0, NULL);

    assert_int_equal(Person_getCategory(person), Person_getCategory(got));
    assert_string_equal(Person_getFirstName(person), Person_getFirstName(got));
    assert_string_equal(Person_getSecondName(person), Person_getSecondName(got));

    Wickets_deepDelete(wickets);
}

void moduleTests_Wickets(void) {
    const UnitTest tests[] =
    {
        unit_test(new__void__noPassers),
        unit_test(addPasser__passer__relevantPassersNumber),
        unit_test(Wickets_emulatePasses)
    };
    return run_tests(tests);
}
