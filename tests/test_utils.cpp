// https://stackoverflow.com/a/64359731

#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <utility>

// Like popen(), but returns two FILE*: child's stdin and stdout, respectively.
std::pair<FILE *, FILE *> popen2() {
  // pipes[0]: parent writes, child reads (child's stdin)
  // pipes[1]: child writes, parent reads (child's stdout)
  int pipes[2][2];

  pipe(pipes[0]);
  pipe(pipes[1]);

  if (fork() > 0) {
    // parent
    close(pipes[0][0]);
    close(pipes[1][1]);

    return {fdopen(pipes[0][1], "w"), fdopen(pipes[1][0], "r")};
  } else {
    // child
    close(pipes[0][1]);
    close(pipes[1][0]);

    dup2(pipes[0][0], STDIN_FILENO);
    dup2(pipes[1][1], STDOUT_FILENO);

    execl("/bin/sh", "/bin/sh", "-c", "../interpreter/ic23int", NULL);

    exit(1);
  }
}