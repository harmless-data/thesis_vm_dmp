#include <liburing.h>
#include <string>

#define DEFAULT_QUE_DEPTH 64

class UringBenchmark
{
public:
    UringBenchmark(std::string file_path, uint64_t que_depth = DEFAULT_QUE_DEPTH);
    ~UringBenchmark();

    void Run();

private:
    void StressTest(unsigned int n_items);
    void BasicUringWrite();

    struct io_uring m_uring;

    struct io_uring_sqe *m_sqe; // Submission Queue Entry
    struct io_uring_cqe *m_cqe; // Completion Queue Entry

    struct io_uring_params *m_params;

    const uint64_t m_queDepth;
    int m_fd;
};
