#include "UringBenchmark.h"

#include <unistd.h>
#include <fcntl.h>
#include <iostream>

UringBenchmark::UringBenchmark(std::string path, uint64_t queDepth)
    : m_queDepth{queDepth}
{
    m_fd = open(path.c_str(), O_RDWR);

    io_uring_queue_init(m_queDepth, &m_uring, 0);
}

UringBenchmark::~UringBenchmark()
{
    io_uring_queue_exit(&m_uring);
    close(m_fd);
}

void UringBenchmark::Run()
{

    // sqe - submission queue entry
    auto sqe = io_uring_get_sqe(&m_uring);

    static struct io_uring_cmd cmd = {
        .data = 0,
        .cmd_op = '*',
        .cmd_len = 0};

    sqe->opcode = IORING_OP_URING_CMD;
    sqe->fd = m_fd;
    sqe->addr = (uintptr_t)&cmd;
    sqe->len = sizeof(cmd);

    io_uring_submit(&m_uring);

    struct io_uring_cqe *cqe;

    io_uring_wait_cqe(&m_uring, &cqe);

    std::cout << "uring return:" << cqe->res << "\n";

    io_uring_cqe_seen(&m_uring, cqe);
}
