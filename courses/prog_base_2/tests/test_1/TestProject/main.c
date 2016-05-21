#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>

#include "cmocka.h"

#include "ControlPost.h"

static void Post_getLastNCars__N__validRes(void **state){
    Post_t post = Post_new(0, 0);
    Person_t carOwner = Person_new("Nikita", "Silchenko");

    Car_t car = Car_new("007");
    Car_t car1 = Car_new("008");
    Car_t car2 = Car_new("009");
    Car_setOwner(car, carOwner);

    Transist_t transist = Transist_new(car, TRANSIST_DEST_TO);
    Transist_t transist1 = Transist_new(car1, TRANSIST_DEST_FROM);
    Transist_t transist2 = Transist_new(car2, TRANSIST_DEST_TO);

    Post_pass(post, transist);
    Post_pass(post, transist1);
    Post_pass(post, transist2);

    List_t lasr2cars = Post_getLastNCars(post, 2);

    assert_string_equal(Car_getID(car1), Car_getID(List_get(lasr2cars, 0, NULL)));
    assert_string_equal(Car_getID(car2), Car_getID(List_get(lasr2cars, 1, NULL)));

    List_delete(lasr2cars);
    Post_delete(post);
    Transist_delete(transist);
    Transist_delete(transist1);
    Transist_delete(transist2);
    Person_delete(carOwner);

    Car_delete(car);
    Car_delete(car1);
    Car_delete(car2);
}

int main(){
    const UnitTest tests[] =
    {
        unit_test(Post_getLastNCars__N__validRes),
    };
    return run_tests(tests);
}
