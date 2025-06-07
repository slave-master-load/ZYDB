#include "core/db_server.h"

int main(int argc, char** argv) {
    zydb::DBServerOptions options;
    options.port = 9999;
    options.backlog = 500;
    zydb::DBServer db_server(options);

    db_server.Start();

    return 0;
}
