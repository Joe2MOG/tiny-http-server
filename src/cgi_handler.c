#include "cgi_handler.h"
#include "io_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/**
 * send_cgi_error - Sends a 500 Internal Server Error and exits
 * @client_socket: Client socket to write error
 * @message: Error message to log
 */
static void send_cgi_error(int client_socket, const char *message)
{
	char error_msg[512];

	perror(message);
	snprintf(error_msg, sizeof(error_msg),
		 "HTTP/1.1 500 Internal Server Error\r\n"
		 "Content-Type: text/html\r\n"
		 "Content-Length: 39\r\n"
		 "\r\n"
		 "<html><body><h1>500 CGI Error</h1></body></html>");
	send_all(client_socket, error_msg, strlen(error_msg));
	_exit(EXIT_FAILURE);
}

/**
 * serve_cgi - Executes a CGI program and sends its output to the client
 * @client_socket: Connected client socket
 * @script_path: Path to the CGI executable (e.g., "www/cgi-bin/script")
 * @query_string: Query string from the URL
 */
void serve_cgi(int client_socket, const char *script_path,
	       const char *query_string)
{
	pid_t child_pid;
	char header[256];

	/* Send preliminary response line (CGI script outputs the rest) */
	snprintf(header, sizeof(header),
		 "HTTP/1.1 200 OK\r\n"
		 "Server: JosephHTTP/1.0\r\n");
	send_all(client_socket, header, strlen(header));

	child_pid = fork();
	if (child_pid == -1)
	{
		/* fork failure: send 500 error and return */
		send_cgi_error(client_socket, "fork failed");
		return;
	}
	if (child_pid == 0)        /* Child */
	{
		/* Set CGI environment variable */
		setenv("QUERY_STRING", query_string ? query_string : "", 1);

		/* Redirect stdout and stderr to the client socket */
		if (dup2(client_socket, STDOUT_FILENO) == -1 ||
		    dup2(client_socket, STDERR_FILENO) == -1)
		{
			send_cgi_error(client_socket, "dup2 failed");
		}

		/* Execute CGI program */
		execl(script_path, script_path, (char *)NULL);
		/* If execl returns, it failed */
		send_cgi_error(client_socket, "execl failed");
	}
	/* Parent: close client socket (child has it) and continue */
	close(client_socket);
	waitpid(-1, NULL, WNOHANG); /* reap any zombie immediately if possible */
}
