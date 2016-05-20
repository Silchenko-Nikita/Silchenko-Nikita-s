#include "web.h"

int main() {
    lib_init();

	Web_t web = Web_new(5000);

	Web_listen(web);

	Web_delete(web);
	lib_free();
    return 0;
}
