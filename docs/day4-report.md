Day 4 Report: Concurrency via Dispatcher and fork()

Project: Tiny HTTP Server
Goal: Add concurrency so the server can handle multiple clients
simultaneously, without rewriting existing logic.
Date: 5/01/2026

1. What Was Built

On Day 4, I added a concurrent execution model using the Unix fork()
system call. The server now handles multiple clients at the same
time, each in its own child process. A new module, dispatcher.c,
encapsulates the concurrency strategy, keeping the server core
unchanged.

2. New Module: dispatcher.c / dispatcher.h

This module defines a single public interface:

- void dispatch_client(int client_socket, client_handler handler)

client_handler is a typedef for a function pointer: void (*)(int).

The current implementation uses fork():

- The parent process creates a child via fork().
- The child calls the provided handler (e.g., handle_client)
  and then exits.
- The parent closes its copy of the client socket and immediately
  returns to the accept loop, ready to accept another client.

If fork() fails, the handler is called in the parent process (graceful
degradation) and the socket is cleaned up afterwards.

3. Preventing Zombie Processes

In main.c, we call signal(SIGCHLD, SIG_IGN) before the accept loop.
This tells the kernel to automatically reap child processes when they
exit, preventing zombie accumulation.

4. Changes to Existing Code

- src/main.c: Added signal(SIGCHLD, SIG_IGN) after socket setup.
- src/server.c:
  - Included dispatcher.h.
  - Replaced the direct handle_client() call and socket close with
    dispatch_client(client_socket, handle_client).
  - The loop no longer closes the client socket; the dispatcher
    manages all socket lifecycle.
- Makefile: Added src/dispatcher.c to the source list.

5. Clean Architecture in Action

- Single Responsibility: server.c owns the accept loop and request
  routing; dispatcher.c owns the concurrent execution.
- Open-Closed Principle: The concurrency strategy can be swapped
  (e.g., to select() or threads) without modifying server.c or any
  other module.
- Dependency Inversion: server.c depends on the abstract interface
  (dispatch_client), not on the implementation details of fork().

6. Betty Compliance and C89

All new files pass betty with 0 errors and 0 warnings.
The dispatcher function is under 40 lines.
The code uses only C89 features (variables at top, no C99 loops).

7. Testing

The server was started, and multiple curl requests were sent
simultaneously:

curl http://localhost:8080/ & curl http://localhost:8080/about.html &

Both requests were served concurrently, with no blocking.
The server logs showed interleaved connection messages.

8. Lessons Learned

- fork() duplicates the entire process, including open file
  descriptors. The child and parent must correctly manage their
  copies to avoid resource leaks.
- signal(SIGCHLD, SIG_IGN) is a simple way to avoid zombies in
  a forking server.
- Encapsulating concurrency behind a clean interface makes the
  code future-proof and testable.
- The dispatcher pattern preserves the existing synchronous
  request handling code, making concurrency a detail, not a
  rewrite.

9. Next Steps (Day 5)

On Day 5, I will:
- Add a Makefile target for testing.
- Create a simple test suite using shell scripts.
- Polish the README with build instructions, usage, and
  architectural overview.
- Prepare a final code review and reflect on the entire journey.
