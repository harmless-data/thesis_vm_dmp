#include <string>

struct WriteWrapper
{
    int index;
    char value;
};

class SyscallBenchmark
{
public:
    SyscallBenchmark(std::string path, size_t cycles);
    ~SyscallBenchmark();

    int Run();

    void RunVfioComp();

private:
    size_t m_cycles;
    int m_fd;
};
