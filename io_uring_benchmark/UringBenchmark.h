#include <liburing.h>

#include <string>

class UringBenchmark
{
public:
    UringBenchmark(uint64_t que_depth, std::string file_path);

private:
    struct io_uring uring;
    const uint64_t que_depth;
    const std::string file_path;
};

UringBenchmark::UringBenchmark(uint64_t que_depth, std::string file_path)
    : que_depth{que_depth}, file_path{file_path}
{
}
