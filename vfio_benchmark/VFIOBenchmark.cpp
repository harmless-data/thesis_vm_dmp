#include "VFIOBenchmark.h"

VFIOBenchmark::VFIOBenchmark(std::string pathToDevice, size_t mmioSize)
    : m_mmioSize{mmioSize}
{
    m_fd = open(pathToDevice.c_str(), O_RDWR | O_SYNC);
    m_mappedBase = mmap(nullptr, m_mmioSize, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0);
}

VFIOBenchmark::~VFIOBenchmark()
{
    munmap(m_mappedBase, m_mmioSize);
}
