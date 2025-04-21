
#include <sys/mman.h>
#include <sys/stat.h>

#include <fcntl.h>
#include <unistd.h>
#include <string>

class VFIOBenchmark
{
public:
    VFIOBenchmark(std::string path, size_t mmioSize);
    ~VFIOBenchmark();

    void Run();

private:
    void *m_mappedBase;
    size_t m_mmioSize;
    int m_fd;
};