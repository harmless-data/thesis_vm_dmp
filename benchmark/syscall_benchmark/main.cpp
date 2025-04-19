#include "SyscallBenchmark.h"

int main(int argc, char const *argv[])
{
    SyscallBenchmark bench{1000, "/dev/logmodule"};
    bench.RunIOCtl();
    return 0;
}
