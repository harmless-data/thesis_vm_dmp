#include "UringBenchmark.h"

UringBenchmark::UringBenchmark(uint64_t que_depth, std::string file_path)
    : que_depth{que_depth}, file_path{file_path}, uring{}
{
    io_uring_buf_ring_init(que_depth, &uring, 0);
}