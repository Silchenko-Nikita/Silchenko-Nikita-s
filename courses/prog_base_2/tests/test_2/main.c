#include "server.h"
#include "list.h"
#include "files_manger.h"
#include "db_manager.h"

#include <stdio.h>

#define PORT 5000

int main() {
    lib_init();

	Server_t server = Server_new(PORT);
	printf("port: %d", PORT);

	Server_listen(server);

	Server_delete(server);
	lib_free();
    return 0;
}
