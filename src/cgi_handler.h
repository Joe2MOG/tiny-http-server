#ifndef CGI_HANDLER_H
#define CGI_HANDLER_H

/**
 * serve_cgi - Handles a CGI request by executing the script
 * @client_socket: Connected client socket
 * @script_path: Path to CGI program (relative to current directory)
 * @query_string: Query string from the URL
 */
void serve_cgi(int client_socket, const char *script_path,
	       const char *query_string);

#endif /* CGI_HANDLER_H */
