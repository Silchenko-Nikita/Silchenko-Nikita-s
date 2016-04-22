#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include "cmocka.h"

#include "tests.h"
#include "queue.h"

static void new__void__isEmpty(void **state){
    Queue_t queue = Queue_new();

    assert_true(Queue_isEmpty(queue));

    Queue_delete(queue);
}

static void enqueue__val__increasedSize(void **state){
    Queue_t queue = Queue_new();

    assert_int_equal(Queue_getSize(queue), 0);
    Queue_enqueue(queue, 1);
    assert_int_equal(Queue_getSize(queue), 1);

    Queue_delete(queue);
}

static void dequeue__void__validRes_decreasedSize(void **state){
    Queue_t queue = Queue_new();

    Queue_enqueue(queue, 1);
    Queue_enqueue(queue, 2);

    assert_int_equal(Queue_dequeue(queue), 1);
    assert_int_equal(Queue_getSize(queue), 1);

    assert_int_equal(Queue_dequeue(queue), 2);
    assert_int_equal(Queue_getSize(queue), 0);

    Queue_delete(queue);
}

static void head__void__validRes(void **state){
    Queue_t queue = Queue_new();

    Queue_enqueue(queue, 1);
    assert_int_equal(Queue_head(queue), 1);
    Queue_enqueue(queue, 2);
    assert_int_equal(Queue_head(queue), 1);

    Queue_delete(queue);
}

static void subscribeSingleOverflowEvent__callback_data__callbackCalled(void **state){
    Queue_t queue = Queue_new();
    int callbackIsCalled = 0;

    void callback(Queue_t caller, void * callerData){
        callbackIsCalled = 1;

        assert_true(caller == queue);
        assert_string_equal((char *)callerData, "test string");
    }

    Queue_subscribeSingleOverflowEvent(queue, callback, "test string");

    for(int i = 0; i < 11; i++){
        Queue_enqueue(queue, 1);
    }

    assert_true(callbackIsCalled);

    Queue_delete(queue);
}


static void subscribePairOverflowEvent__callback_data__callbackCalled(void **state){
    Queue_t queue1 = Queue_new();
    Queue_t queue2 = Queue_new();

    for(int i = 0; i < 3; i++){
        Queue_enqueue(queue1, 1);
        Queue_enqueue(queue2, 1);
    }

    Queue_bindQueues(queue1, queue2);

    int callbackIsCalled = 0;

    void callback(Queue_t caller, void * callerData, Queue_t boundQueue){
        callbackIsCalled = 1;

        assert_true(caller == queue1);
        assert_true(boundQueue == queue2);
        assert_string_equal((char *)callerData, "test string");
    }

    Queue_subscribePairOverflowEvent(queue1, callback, "test string");

     for(int i = 0; i < 7; i++){
        Queue_enqueue(queue1, 1);
        Queue_enqueue(queue2, 1);
    }

    Queue_delete(queue1);
    Queue_delete(queue2);
}


static void subscribePairEmptyEvent__callback_data__callbackCalled(void **state){
    Queue_t queue1 = Queue_new();
    Queue_t queue2 = Queue_new();

    for(int i = 0; i < 3; i++){
        Queue_enqueue(queue1, 1);
        Queue_enqueue(queue2, 1);
    }

    Queue_bindQueues(queue1, queue2);

    int callbackIsCalled = 0;

    void callback(Queue_t caller, void * callerData, Queue_t boundQueue){
        callbackIsCalled = 1;

        assert_true(caller == queue1);
        assert_true(boundQueue == queue2);
        assert_string_equal((char *)callerData, "test string");
    }

    Queue_subscribePairOverflowEvent(queue1, callback, "test string");

     for(int i = 0; i < 3; i++){
        Queue_dequeue(queue1);
        Queue_dequeue(queue2);
    }

    Queue_delete(queue1);
    Queue_delete(queue2);
}


void moduleTests_Queue(void) {
    const UnitTest tests[] =
    {
        unit_test(new__void__isEmpty),
        unit_test(enqueue__val__increasedSize),
        unit_test(dequeue__void__validRes_decreasedSize),
        unit_test(head__void__validRes),
        unit_test(subscribeSingleOverflowEvent__callback_data__callbackCalled),
        unit_test(subscribePairOverflowEvent__callback_data__callbackCalled),
        unit_test(subscribePairEmptyEvent__callback_data__callbackCalled)
    };
    return run_tests(tests);
}
