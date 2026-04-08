//Author: Liam Ahern A00463807
//logger

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#define SOCKET_PATH "./logger_location"
#define BUFFER_SIZE 1024



int main() {

    //open log.txt
    FILE *log_file = fopen("../logs/logs.txt", "w");
    if (log_file == NULL) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }

    // file descriptor for the socket, address family is local unix storage, bidirection connection based, default protoccol
    int socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);

    char buffer[BUFFER_SIZE];

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

    //clear if old socket present
    unlink(SOCKET_PATH);


    //bind socket to the path, else close and return error
    //typecast generic socket address for bind usage, specify address size
    if (bind(socket_fd, (struct sockaddr*)&socket_address, sizeof(socket_address)) == -1) {
        perror("bind");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }


    // listen for clients
    if (listen(socket_fd, 1) == -1) {
        perror("listen");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }


    int client_fd;

    //wait for connections
    while (1) {
        client_fd = accept(socket_fd, NULL, NULL);
        if (client_fd == -1) {
            perror("accept client");
            close(socket_fd);
            unlink(SOCKET_PATH);
            exit(EXIT_FAILURE);
        }

        //clear buffer for incoming reading
        memset(&buffer, 0, BUFFER_SIZE);

        //read incoming message, error if byte count -1
        if (read(client_fd, buffer, BUFFER_SIZE) == -1) {
            perror("read");
            close(client_fd);
            exit(EXIT_FAILURE);
        }

        //reads from when main menu closes
        if (strncmp(buffer, "SHUTDOWN", 8) == 0) {
            time_t given_time;
            struct tm *local_time;
            char timestamp[BUFFER_SIZE];
            time(&given_time);
            local_time = localtime(&given_time);
            strftime(timestamp, BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", local_time);
            fprintf(log_file, "[%s]  MAIN_MENU: System shutdown\n", timestamp);
            fclose(log_file);
            close(client_fd);
            close(socket_fd);
            unlink(SOCKET_PATH);
            exit(EXIT_SUCCESS);
        }

        //Gets the current time, converts it to local time, formats as a string, 
        //then writes the full [timestamp] message
        time_t given_time;
        struct tm *local_time;
        char timestamp[BUFFER_SIZE];
        time(&given_time);
        local_time = localtime(&given_time);

        strftime(timestamp, BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", local_time);
        fprintf(log_file, "[%s]  %s\n", timestamp,  buffer);

        //close connection
        close(client_fd);

    }
    //release memory
    fclose(log_file);

    return 0;
}