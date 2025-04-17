#include "UringBenchmark.h"

#include <stdio.h>

int main(int argc, char const *argv[])
{
    UringBenchmark tester{10, "/dev/logmoudle"};
    return 0;
}
