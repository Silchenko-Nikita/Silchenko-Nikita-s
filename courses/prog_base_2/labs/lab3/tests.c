#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include <cmocka.h>

#include "tests.h"
#include "queue_pair.h"

static void new__void__queuesSizesEqueal3(void **state){
    QueuePair_t queuePair = QueuePair_new(NULL, NULL, NULL);

    assert_int_equal(Queue_getSize(QueuePair_getFirst(queuePair)), 3);
    assert_int_equal(Queue_getSize(QueuePair_getSecond(queuePair)), 3);

    QueuePair_delete(queuePair);
}

static void enqueueFirst__val__increasedSize(void **state){
    QueuePair_t queuePair = QueuePair_new(NULL, NULL, NULL);

    assert_int_equal(Queue_getSize(QueuePair_getFirst(queuePair)), 3);
    QueuePair_enqueueFirst(queuePair, 1);
    assert_int_equal(Queue_getSize(QueuePair_getFirst(queuePair)), 4);

    QueuePair_delete(queuePair);
}

static void dequeueFirst__val__increasedSize(void **state){
    QueuePair_t queuePair = QueuePair_new(NULL, NULL, NULL);

    assert_int_equal(Queue_getSize(QueuePair_getSecond(queuePair)), 3);
    QueuePair_enqueueSecond(queuePair, 1);
    assert_int_equal(Queue_getSize(QueuePair_getSecond(queuePair)), 4);

    QueuePair_delete(queuePair);
}

static void enqueueFirst__void__decreasedSize(void **state){
    QueuePair_t queuePair = QueuePair_new(NULL, NULL, NULL);

    assert_int_equal(Queue_getSize(QueuePair_getFirst(queuePair)), 3);
    QueuePair_dequeueFirst(queuePair);
    assert_int_equal(Queue_getSize(QueuePair_getFirst(queuePair)), 2);

    QueuePair_delete(queuePair);
}

static void dequeueFirst__void__decreasedSize(void **state){
    QueuePair_t queuePair = QueuePair_new(NULL, NULL, NULL);

    assert_int_equal(Queue_getSize(QueuePair_getSecond(queuePair)), 3);
    QueuePair_dequeueSecond(queuePair);
    assert_int_equal(Queue_getSize(QueuePair_getSecond(queuePair)), 2);

    QueuePair_delete(queuePair);
}

static void enqueueFirst__val__singleOverflow(void **state){
    int overflowed = 0;

    void singleOverflow(Queue_t queue){
        overflowed = 1;
    }

    QueuePair_t queuePair = QueuePair_new(NULL, singleOverflow, NULL);

    for(int i = 0; i < QUEUE_PAIR_MAX_SIZE; i++){
        QueuePair_enqueueFirst(queuePair, 1);
    }

    assert_int_equal(1, overflowed);

    QueuePair_delete(queuePair);
}

static void enquequeSecond__val__singleOverflow(void **state){
    int overflowed = 0;

    void singleOverflow(Queue_t queue){
        overflowed = 1;
    }

    QueuePair_t queuePair = QueuePair_new(NULL, singleOverflow, NULL);

    for(int i = 0; i < QUEUE_PAIR_MAX_SIZE; i++){
        QueuePair_enqueueSecond(queuePair, 1);
    }

    assert_int_equal(1, overflowed);

    QueuePair_delete(queuePair);
}

static void enqueueFirst_enqueueSecond__val__fullOverflow(void **state){
    int overflowed = 0;

    void fullOverflow(Queue_t queue1, Queue_t queue2){
        overflowed = 1;
    }

    QueuePair_t queuePair = QueuePair_new(NULL, NULL, fullOverflow);

    for(int i = 0; i < QUEUE_PAIR_MAX_SIZE; i++){
        QueuePair_enqueueFirst(queuePair, 1);
        QueuePair_enqueueSecond(queuePair, 1);
    }

    assert_int_equal(1, overflowed);

    QueuePair_delete(queuePair);
}

static void dequeueFirst_dequeueSecond__void__empty(void **state){
    int isEmpty = 0;

    void empty(Queue_t queue1, Queue_t queue2){
        isEmpty = 1;
    }

    QueuePair_t queuePair = QueuePair_new(empty, NULL, NULL);

    for(int i = 0; i < 3; i++){
        QueuePair_dequeueFirst(queuePair);
        QueuePair_dequeueSecond(queuePair);
    }

    assert_int_equal(1, isEmpty);

    QueuePair_delete(queuePair);
}

void moduleTests_QueuePair(void) {
    const UnitTest tests[] =
    {
        unit_test(new__void__queuesSizesEqueal3),
        unit_test(enqueueFirst__val__increasedSize),
        unit_test(dequeueFirst__val__increasedSize),
        unit_test(enqueueFirst__void__decreasedSize),
        unit_test(dequeueFirst__void__decreasedSize),
        unit_test(enqueueFirst__val__singleOverflow),
        unit_test(enquequeSecond__val__singleOverflow),
        unit_test(enqueueFirst_enqueueSecond__val__fullOverflow),
        unit_test(dequeueFirst_dequeueSecond__void__empty)
    };
    return run_tests(tests);
}
