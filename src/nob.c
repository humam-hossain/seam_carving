#define NOB_IMPLEMENTATION
#include "nob.h"

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    
    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, "gcc");
    nob_cmd_append(&cmd, "-Wall", "-Wextra", "-ggdb");
    nob_cmd_append(&cmd, "-o", "bin/main", "src/main.c");
    nob_cmd_append(&cmd, "-lm");

    if(!nob_cmd_run_sync(cmd)) return 1;

    return 0;
}