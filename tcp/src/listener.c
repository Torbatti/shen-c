#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
    int sock_status, sock_fd;
    int setsockopt_status;
    struct sockaddr_in listener_address;
    int bind_status;
    int listen_status;
    int accept_status, accept_fd;
    int recv_status, recv_len;
    int send_status, send_len;

    sock_status = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_status == -1)
    {
        printf("socket failed!");
        return -1;
    }
    sock_fd = sock_status;

    // No idea what opt int is
    int opt = 1;
    setsockopt_status = setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if (accept_status == -1)
    {
        printf("accept failed!");
        return -1;
        // continue;
    }

    listener_address.sin_addr.s_addr = INADDR_ANY; // or "0.0.0.0"
    listener_address.sin_port = htons(10654);
    listener_address.sin_family = AF_INET;

    bind_status = bind(sock_fd, (struct sockaddr *)&listener_address, sizeof(listener_address));
    // funny enough bind can fail but c/c++ vscode toolchain doesnt mention this
    if (bind_status == -1)
    {
        printf("bind failed!");
        return -1;
    }

    listen_status = listen(sock_fd, 128);
    if (listen_status != 0)
    {
        printf("listen failed!");
        return -1;
    }

    while (1)
    {
        accept_status = accept(sock_fd, 0, 0);
        if (accept_status == -1)
        {
            printf("accept failed!");
            return -1;
            // continue;
        }
        accept_fd = accept_status;

        char recv_buf[256];
        recv_status = recv(accept_fd, &recv_buf, sizeof(recv_buf), 0);
        if (recv_status == -1)
        {
            printf("recv failed!");
            return -1;
            // continue;
        }
        recv_len = recv_status;

        char *send_buf = (char *)malloc(recv_len * sizeof(char));
        memcpy(send_buf, recv_buf, recv_len);

        send_status = send(accept_fd, send_buf, sizeof(send_buf), 0);
        if (send_status == -1)
        {
            printf("send failed!");
            return -1;
            // continue;
        }
        send_len = send_status;

        close(accept_fd);
    }

    close(sock_fd);

    return 0;
}