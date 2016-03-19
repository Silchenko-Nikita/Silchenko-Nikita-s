#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include <cmocka.h>

#include "../person.h"
#include "tests.h"

static void new__category_firstName_secondName__relevantCharacteristics(void **state){
    Person_t person = Person_new(PERSON_STUDENT, "Nikita", "Silchenko", NULL);

    assert_int_equal(Person_getCategory(person), PERSON_STUDENT);
    assert_string_equal(Person_getFirstName(person), "Nikita");
    assert_string_equal(Person_getSecondName(person), "Silchenko");

    Person_delete(person);
}

void moduleTests_Person(void) {

    const UnitTest tests[] =
    {
        unit_test(new__category_firstName_secondName__relevantCharacteristics)
    };

    return run_tests(tests);
}
