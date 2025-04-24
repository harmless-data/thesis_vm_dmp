#include "UringBenchmark.h"

#include <stdio.h>

int main(int argc, char const *argv[])
{
    UringBenchmark bench{"/dev/logmodule"};

    bench.Run();

    return 0;
}
