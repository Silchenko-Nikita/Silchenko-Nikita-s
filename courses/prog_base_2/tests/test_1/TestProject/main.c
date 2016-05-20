#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include "cmocka.h"

static void new__void__isEmpty(void **state){
    Queue_t queue = Queue_new();

    assert_true(Queue_isEmpty(queue));

    Queue_delete(queue);
}

int main(){
    const UnitTest tests[] =
    {
        unit_test(new__void__isEmpty),
    };
    return run_tests(tests);
    return 0;
}
