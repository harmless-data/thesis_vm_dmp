#include "VFIOBenchmark.h"

#include <unistd.h>

#define DEVICE_NAME "/dev/logmodule"

int main(int argc, char const *argv[])
{
    VFIOBenchmark bench(DEVICE_NAME, sysconf(_SC_PAGESIZE));

    bench.Run();

    return 0;
}
