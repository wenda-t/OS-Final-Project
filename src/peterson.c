#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>

//method for logging
void log_action(const char *message) {
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, "./logger_location", sizeof(addr.sun_path) - 1);

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == 0) {
        write(fd, message, strlen(message));
    }
    close(fd);
}

//to be used in main method
log_action("Peterson: X");
