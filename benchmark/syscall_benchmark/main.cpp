#include "SyscallBenchmark.h"

#include <unistd.h>

#define DEVICE_NAME "/dev/logmodule"

int main(int argc, char const *argv[])
{
    SyscallBenchmark bench(DEVICE_NAME, sysconf(_SC_PAGESIZE));

    bench.RunVfioComp();

    return 0;
}
