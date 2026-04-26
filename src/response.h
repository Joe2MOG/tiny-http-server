#ifndef RESPONSE_H
#define RESPONSE_H

#include <stddef.h>

void build_http_response(char *buffer, size_t buffer_size, const char *body);
void send_http_response(int client_socket, const char *response);

#endif /* RESPONSE_H */
