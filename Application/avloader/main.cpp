#include <zstack.h>

#include "main.h"

struct application app;

int main(int argc, char *argv[])
{
    if (param_parser(argc, argv, &app) == -1) {
        print_usage();
        return -1;
    }

    log_init(app.param.log_config);

    // load file

    // write adata
    
    // write register

    // communicate with it, use global variable address
    // the tool depend on cpu_comm.so load, otherwise mmap failed

    return 0;
}
