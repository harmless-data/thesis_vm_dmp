
#include <sys/mman.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <string>

class VFIOBenchmark
{
public:
    VFIOBenchmark(std::string pathToDevice, size_t mmioSize);
    ~VFIOBenchmark();

    void Benchmark();

private:
    void *m_mappedBase;
    size_t m_mmioSize;
    int m_fd;
};