#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include <cmocka.h>

#include "tests.h"
#include "../list.h"

static void new__void__zeroSize(void **state){
    List_t list = List_new();

    assert_int_equal(List_getSize(list), 0);

    List_delete(list);
}

static void add__index_el__addsAmountEqualsSize(void **state){
    List_t list = List_new();

    List_add(list, 0, "dasdsa das");
    assert_int_equal(List_getSize(list), 1);

    List_add(list, 1, "das dasdsa das");
    assert_int_equal(List_getSize(list), 2);

    List_delete(list);
}


static void get__index__getEqualsAdd(void **state){
    List_t list = List_new();

    List_add(list, 0, "dasdsa das");
    assert_string_equal(List_get(list, 0, NULL), "dasdsa das");

    List_add(list, 1, "dffdsfds");
    List_add(list, 0, "dccdsdc");

    assert_string_equal(List_get(list, 2, NULL), "dffdsfds");
    assert_string_equal(List_get(list, 0, NULL), "dccdsdc");

    List_delete(list);
}

static void append__el__elOnTop_relevantSize(void **state){
    List_t list = List_new();
    List_append(list, "dasdsa das");
    assert_string_equal(List_get(list, 0, NULL), "dasdsa das");

    List_append(list, "1");
    assert_string_equal(List_get(list, 1, NULL), "1");

    assert_int_equal(List_getSize(list), 2);

    List_delete(list);
}

static void set__index_el__getEqualsSet(void **state){
    List_t list = List_new();

    List_add(list, 0, "dasdsa das");
    List_set(list, 0, "2", NULL);
    assert_string_equal(List_get(list, 0, NULL), "2");

    List_delete(list);
}

static void remove__index__decreasedSize(void **state){
    List_t list = List_new();

    List_add(list, 0, "dasddas");
    List_remove(list, 0, NULL);

    assert_int_equal(List_getSize(list), 0);

    List_delete(list);
}

void moduleTests_List(void) {
    const UnitTest tests[] =
    {
        unit_test(new__void__zeroSize),
        unit_test(add__index_el__addsAmountEqualsSize),
        unit_test(get__index__getEqualsAdd),
        unit_test(append__el__elOnTop_relevantSize),
        unit_test(set__index_el__getEqualsSet),
        unit_test(remove__index__decreasedSize)
    };
    return run_tests(tests);
}
