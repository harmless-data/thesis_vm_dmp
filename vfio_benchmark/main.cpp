#include "VFIOBenchmark.h"

#include <stdio.h>

int main(int argc, char const *argv[])
{
    VFIOBenchmark vfio{"/dev/logmodule", 1024};

    for (int i = 0; i < 10; i++)
    {
        vfio.Benchmark();
    }
    return 0;
}
