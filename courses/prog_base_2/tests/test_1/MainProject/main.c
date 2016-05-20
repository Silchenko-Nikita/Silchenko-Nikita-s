#include <stdio.h>
#include <stdlib.h>

#include "ControlPost.h"

int main()
{
    Post_t post = Post_new();
    Transist_t transist = Transist_new();
    Car_t car = Car_new();

    Post_delete(post);
    Transist_delete(transist);
    Car_delete(car);
    return 0;
}
