#include "VFIOBenchmark.h"

#include <chrono>
#include <thread>

VFIOBenchmark::VFIOBenchmark(std::string pathToDevice, size_t mmioSize)
    : m_mmioSize{mmioSize}
{
    printf("[VFIO] Init\n");

    m_fd = open(pathToDevice.c_str(), O_RDWR | O_SYNC);
    m_mappedBase = mmap(nullptr, m_mmioSize, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0);

    printf("[VFIO] File Descriptor: %ld\n", m_fd);
    printf("[VFIO] Memory Map cb: %p\n", m_mappedBase);

    if (!m_mappedBase)
    {
        printf("[VFIO] memory map of driver failed\n");
    }
}

VFIOBenchmark::~VFIOBenchmark()
{
    auto ret = munmap(m_mappedBase, m_mmioSize);
}

void VFIOBenchmark::Benchmark()
{
    using namespace std::chrono_literals;

    std::this_thread::sleep_for(10ms);
    printf("[VFIO] Starting Benchmark\n");
}
