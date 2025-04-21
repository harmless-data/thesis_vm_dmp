#include "SyscallBenchmark.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

SyscallBenchmark::SyscallBenchmark(std::string path, size_t cycles)
    : m_cycles{cycles}
{
    m_fd = open(path.c_str(), O_RDWR);
}

SyscallBenchmark::~SyscallBenchmark()
{
    close(m_fd);
}

int SyscallBenchmark::Run()
{
    for (int i{0}; i < m_cycles; i++)
    {
        auto written = write(m_fd, NULL, 4);

        if (written < 0)
            throw 666;
    }
    return 0;
}

void SyscallBenchmark::RunVfioComp()
{
    for (int i = 0; i < 10; i++)
    {
        ioctl(m_fd, i, "!");
        sleep(1);
    }
}