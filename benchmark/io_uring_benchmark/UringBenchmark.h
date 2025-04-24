#include <liburing.h>
#include <string>

#define DEFAULT_QUE_DEPTH 64

struct io_uring_cmd
{
    __u64 data;
    __u32 cmd_op;
    __u32 cmd_len;
    __u8 cmd[0];
};

class UringBenchmark
{
public:
    UringBenchmark(std::string file_path, uint64_t que_depth = DEFAULT_QUE_DEPTH);
    ~UringBenchmark();

    void Run();

private:
    struct io_uring m_uring;
    const uint64_t m_queDepth;
    int m_fd;
};
