#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define SOCKET_PATH "./logger_location"


int main() {

    // file descriptor for the socket, address family is local unix storage, bidirection connection based, default protoccol
    int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    //socket property container
    struct sockaddr_un socket_address;

    //zero out memory to prevent garbage memory
    memset(&socket_address, 0, sizeof(socket_address));

    //socket unix family, specifies the addressing structure
    socket_address.sun_family = AF_UNIX;

    //copy our socket path into our socket struct, ensuring null terminator not overwritten
    strncpy(socket_address.sun_path, SOCKET_PATH, sizeof(socket_address.sun_path) -1);

    //bind socket to the path, else close and return error
    //typecast generic socket address for bind usage, specify address size
    if (bind(socket_fd, (struct sockaddr*)&socket_address, sizeof(socket_address)) == -1) {
        perror("bind");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }


    close(socket_fd);
    unlink(SOCKET_PATH);

    return 0;
}