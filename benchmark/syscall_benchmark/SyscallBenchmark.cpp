#include "SyscallBenchmark.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

SyscallBenchmark::SyscallBenchmark(unsigned int cycles, std::string path, std::string data)
    : m_cycles{cycles}, m_path{path}, m_data{data}
{
}

int SyscallBenchmark::Run()
{
    for (int i{0}; i < m_cycles; i++)
    {
        printf("\r%d/%d", i + 1, m_cycles);
        auto fd = open(m_path.c_str(), O_RDWR);

        auto written = write(fd, m_data.c_str(), 4);

        if (written < 0)
        {
            throw 666;
        }

        auto _ = close(fd);
    }
    printf("\n");
    return 0;
}

void SyscallBenchmark::RunIOCtl()
{
    WriteWrapper toWrite;

    toWrite.index = 0;
    toWrite.value = "0";

    auto fd = open(m_path.c_str(), O_RDWR);

    int ret = ioctl(fd, 123, NULL);

    auto _ = close(fd);
}