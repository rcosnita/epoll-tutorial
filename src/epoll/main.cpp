#include <unistd.h>
#include <sys/epoll.h>
#include <cstring>
#include <iostream>

static const auto kMaxEvents = 5;
static const auto kReadSize = 10;

int main() {
    auto epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "Failed to create epoll file descriptor" << std::endl;
        return 1;
    }

    epoll_event event {
        .events = EPOLLIN,
        .data = {
            .fd = 0,
        }
    };

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &event)) {
        std::cerr << "Failed to add file descriptor to epoll" << std::endl;
        close(epoll_fd);
        return 1;
    }

    bool running = true;
    epoll_event events[kMaxEvents];
    char readBuffer[kReadSize + 1];

    while (running) {
        printf("\nPolling for input...\n");
        auto eventCount = epoll_wait(epoll_fd, events, kMaxEvents, 30000);
        std::cout << eventCount << " ready events" << std::endl;
        for (auto i = 0; i < eventCount; i++) {
            std::cout << "Reading file descriptor " << events[i].data.fd << std::endl;
            auto bytesRead = read(events[i].data.fd, readBuffer, kReadSize);
            std::cout << bytesRead << " bytes read" << std::endl;
            readBuffer[bytesRead] = '\0';

            std::cout << "Read " << readBuffer << std::endl;

            if(!std::strncmp(readBuffer, "stop\n", 5)) {
                running = false;
            }
        }
    }

    if (close(epoll_fd)) {
        std::cerr << "Failed to close epoll file descriptor" << std::endl;
        return 1;
    }

    return 0;
}
