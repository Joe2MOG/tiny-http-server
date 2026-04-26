#ifndef SERVER_H
#define SERVER_H

void handle_client(int client_socket);
void run_accept_loop(int server_socket, int port);

#endif /* SERVER_H */
