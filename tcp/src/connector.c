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
    struct sockaddr_in listener_address;
    int connect_status;
    char *send_buf, *recv_buf;
    int recv_status, recv_len;
    int send_status, send_len;

    sock_status = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_status == -1)
    {
        printf("socket failed!");
        return -1;
    }
    sock_fd = sock_status;

    listener_address.sin_addr.s_addr = INADDR_ANY; // or "0.0.0.0"
    listener_address.sin_port = htons(10654);
    listener_address.sin_family = AF_INET;

    connect_status = connect(sock_fd, (struct sockaddr *)&listener_address, sizeof(listener_address));
    if (connect_status != 0)
    {
        printf("connect failed!");
        return -1;
    }

    send_buf = (char *)malloc(256 * sizeof(char));
    strcpy(send_buf, "Hello!");
    send_status = send(sock_fd, send_buf, sizeof(send_buf), 0);
    if (send_status == -1)
    {
        printf("send failed!");
        return -1;
    }
    send_len = send_status;

    recv_buf = (char *)malloc(256 * sizeof(char));
    recv_status = recv(sock_fd, recv_buf, sizeof(recv_buf), 0);
    if (recv_status == -1)
    {
        printf("recv failed!");
        return -1;
    }
    recv_len = recv_status;
    printf(recv_buf);

    free(send_buf);
    free(recv_buf);
    
    close(sock_fd);

    return 0;
}