#include "UringBenchmark.h"

#include <unistd.h>
#include <fcntl.h>
#include <iostream>

UringBenchmark::UringBenchmark(std::string path, uint64_t queDepth)
    : m_queDepth{queDepth}
{
    m_fd = open(path.c_str(), O_RDWR);

    if (io_uring_queue_init(m_queDepth, &m_uring, IORING_SETUP_SQPOLL) < 0)
    {
        std::cerr << "Uring Failed to set up\n";
        return;
    }

    // io_uring_queue_init_params() <- EH ? should probably do this
}

UringBenchmark::~UringBenchmark()
{
    io_uring_queue_exit(&m_uring);
    close(m_fd);
}

void UringBenchmark::Run()
{
    BasicUringWrite();
}

void UringBenchmark::StressTest(unsigned int n_items)
{
    // EYO, ARE THE BUFFERS MMAPED?
    static const char buf[] = "buf";

    for (int i = 0; i < n_items; i++)
    {

        m_sqe = io_uring_get_sqe(&m_uring);

        if (!m_sqe)
        {
            std::cerr << "Failed to get SQE\n";
            return;
        }

        m_sqe->opcode = IORING_OP_WRITE;
        m_sqe->fd = m_fd;
        m_sqe->addr = (uintptr_t)buf;
        m_sqe->len = sizeof(buf);
    }

    io_uring_submit(&m_uring);

    io_uring_wait_cqe(&m_uring, &m_cqe);

    std::cout << "uring return:" << m_cqe->res << "\n";

    io_uring_cqe_seen(&m_uring, m_cqe);
}

void UringBenchmark::BasicUringWrite()
{
    static const char buf[] = "buf";

    m_sqe = io_uring_get_sqe(&m_uring);

    if (!m_sqe)
    {
        std::cerr << "Failed to get SQE\n";
        return;
    }

    m_sqe->opcode = IORING_OP_WRITE;
    m_sqe->fd = m_fd;
    m_sqe->addr = (uintptr_t)buf;
    m_sqe->len = sizeof(buf);

    io_uring_submit(&m_uring);

    io_uring_wait_cqe(&m_uring, &m_cqe);

    std::cout << "uring return:" << m_cqe->res << "\n";

    io_uring_cqe_seen(&m_uring, m_cqe);
}
