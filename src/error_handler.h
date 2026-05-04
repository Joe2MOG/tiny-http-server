#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

void send_404_response(int client_socket);
void send_403_response(int client_socket);
void send_501_response(int client_socket);

#endif /* ERROR_HANDLER_H */
