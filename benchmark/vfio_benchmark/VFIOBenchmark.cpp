#include "VFIOBenchmark.h"

VFIOBenchmark::VFIOBenchmark(std::string path, size_t mmioSize)
    : m_mmioSize{mmioSize}
{
    m_fd = open(path.c_str(), O_RDWR | O_SYNC);

    m_mappedBase = mmap(nullptr, m_mmioSize, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, 0);

    if (!m_mappedBase)
        throw 666;
}

VFIOBenchmark::~VFIOBenchmark()
{
    munmap(m_mappedBase, m_mmioSize);
    close(m_fd);
}

void VFIOBenchmark::Run()
{
    auto bufferAsArray = static_cast<char *>(m_mappedBase);

    for (int i = 0; i < m_mmioSize; i++)
    {
        bufferAsArray[i] = '?';
    }
}
