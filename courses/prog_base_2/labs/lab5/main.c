#include "web.h"
#include "data_handler.h"

#include <stdio.h>

#define PORT 5000

int main() {
    lib_init();

	Web_t web = Web_new(PORT);
	printf("port: %d", PORT);

	Web_listen(web);

	Web_delete(web);
	lib_free();
    return 0;
}
