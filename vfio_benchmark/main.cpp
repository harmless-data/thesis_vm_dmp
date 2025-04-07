#include "VFIOBenchmark.h"

#include <stdio.h>

int main(int argc, char const *argv[])
{
    VFIOBenchmark vfio{"/dev/logmodule", 1024};
    return 0;
}
