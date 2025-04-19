#include <string>

struct WriteWrapper
{
    int index;
    char value;
};

class SyscallBenchmark
{
public:
    SyscallBenchmark(unsigned int cycles, std::string path, std::string data = "");

    int Run();
    void RunIOCtl();

private:
    const unsigned int m_cycles;
    const std::string m_path;
    const std::string m_data;
};
