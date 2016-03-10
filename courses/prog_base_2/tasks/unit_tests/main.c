#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include <cmocka.h>

#include "sentence_list.h"

static void new__void__zeroSize(void **state)
{
    SentenceList_t list = SentenceList_new();
    assert_int_equal(SentenceList_getSize(list), 0);
    SentenceList_delete(list);
}

static void add__index_el__addsAmountEqualsSize(void **state)
{
    SentenceList_t list = SentenceList_new();
    SentenceList_add(list, 0, "dasdsa das!");
    assert_int_equal(SentenceList_getSize(list), 1);

    SentenceList_add(list, 1, "das dasdsa das!");
    assert_int_equal(SentenceList_getSize(list), 2);
    SentenceList_delete(list);
}

static void get__index__getEqualsAdd(void **state)
{
    SentenceList_t list = SentenceList_new();
    SentenceList_add(list, 0, "dasdsa das!");
    assert_string_equal(SentenceList_get(list, 0), "dasdsa das!");

    SentenceList_add(list, 1, "Sentece.");
    SentenceList_add(list, 0, "Sentece1?");

    assert_string_equal(SentenceList_get(list, 2), "Sentece.");
    assert_string_equal(SentenceList_get(list, 0), "Sentece1?");
    SentenceList_delete(list);
}

static void set__index_el__getEqualsSet(void **state)
{
    SentenceList_t list = SentenceList_new();
    SentenceList_add(list, 0, "dasdsa das!");
    SentenceList_add(list, 0, "Sentece.");
    assert_string_equal(SentenceList_get(list, 0), "Sentece.");
    SentenceList_delete(list);
}

static void remove__index__zeroSize(void **state)
{
    SentenceList_t list = SentenceList_new();
    SentenceList_add(list, 0, "dasdsa das!");
    SentenceList_remove(list, 0);
    assert_int_equal(SentenceList_getSize(list), 0);
    SentenceList_delete(list);
}

static void getLongestSent__void__ResEqualsLongestSent(void **state)
{
    SentenceList_t list = SentenceList_new();
    SentenceList_add(list, 0, "dasdsa ds adas!");
    SentenceList_add(list, 0, "dasd.");
    SentenceList_add(list, 0, "Write in C!");
    SentenceList_add(list, 0, "How are u?");
    assert_string_equal(SentenceList_get_longest_sent(list), "dasdsa ds adas!");
    SentenceList_delete(list);
}

static void getLongestWord__void__ResEqualsLongestWord(void **state)
{
    SentenceList_t list = SentenceList_new();
    SentenceList_add(list, 0, "dasdsa ds adas!");
    SentenceList_add(list, 1, "Write in C!");
    char * word = SentenceList_get_longest_word(list);
    assert_string_equal(word, "dasdsa");
    free(word);
    SentenceList_delete(list);
}

int main(void) {
    const struct CMUnitTest tests[] =
    {
        cmocka_unit_test(new__void__zeroSize),
        cmocka_unit_test(add__index_el__addsAmountEqualsSize),
        cmocka_unit_test(get__index__getEqualsAdd),
        cmocka_unit_test(set__index_el__getEqualsSet),
        cmocka_unit_test(getLongestSent__void__ResEqualsLongestSent),
        cmocka_unit_test(getLongestWord__void__ResEqualsLongestWord)

    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
