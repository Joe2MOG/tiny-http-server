#ifndef SOCKET_UTILS_H
#define SOCKET_UTILS_H

int create_socket(void);
int set_socket_reuse_address(int server_socket);
int bind_socket_to_address(int server_socket, int port);
int start_listening(int server_socket);
int create_listening_socket(int port);

#endif /* SOCKET_UTILS_H */
