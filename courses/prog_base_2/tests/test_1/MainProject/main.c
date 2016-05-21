#include <stdlib.h>
#include <stdio.h>

#include "ControlPost.h"

void wantedCarPassed_Person(void * receiver, Post_t sender, void * data){
    char buff[128];
    Person_t persont = (Person_t) receiver;
    puts("In callback:");
    puts(Person_toString(persont, buff));
    puts("");
}

int main()
{
    Post_t post = Post_new(0, 0);
    Post_addWantedCar(post, "007");
    Person_t carOwner = Person_new("Nikita", "Silchenko");
    Car_t car = Car_new("007");
    Car_setOwner(car, carOwner);
    Transist_t transist = Transist_new(car, TRANSIST_DEST_TO);
    Post_subscribeWantedCarTransist(post, carOwner, wantedCarPassed_Person, NULL );
    Post_pass(post, transist);
    List_t carsTo = Post_getCars(post, TRANSIST_DEST_TO);
    printf("%d\n", Post_getTransistsNum(post));
    int lSize = List_getSize(carsTo);
    for(int i = 0; i < lSize; i++){
        char buff[128];
        puts(Person_toString(Car_getOwner(List_get(carsTo, i, NULL)), buff));
    }

    Post_delete(post);
    Transist_delete(transist);
    Person_delete(carOwner);
    Car_delete(car);
    return 0;
}
