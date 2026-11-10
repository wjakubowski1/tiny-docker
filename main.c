#define _GNU_SOURCE // required to expose clone() and CLONE_* flags in sched.h
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define STACK_SIZE (1024 * 1024) // 1MB stack for the child process

// the function the cloned child process will execute
int child_fn(void *arg) {
  printf("Container process started! My isolated PID: %d\n", getpid());

  // changes the hostname inside the isolated UTS namespace
  const char *new_hostname = "tiny-container";
  if (sethostname(new_hostname, strlen(new_hostname)) != 0) {
    perror("sethostname failed");
    return EXIT_FAILURE;
  }

  // prepares arguments to launch a basic shell inside the container
  char *args[] = {"/bin/sh", NULL};

  // replaces the child process image with the shell
  execvp(args[0], args);

  // if execvp returns, something went wrong
  perror("execvp failed");
  return EXIT_FAILURE;
}

int main() {
  printf("Host parent process started. My real PID is: %d\n", getpid());

  // allocates 1MB of heap memory to serve as the stack for the container process
  char *stack = malloc(STACK_SIZE);
  if (stack == NULL) {
    perror("malloc failed to allocate stack");
    exit(EXIT_FAILURE);
  }

  // on x86_64 architecture the stack grows downward from high memory to low memory
  // so it must point clone() to the TOP (end) of the allocated memory block
  char *stack_top = stack + STACK_SIZE;

  // flags to isolate Hostname (UTS) and Process IDs (PID)
  // SIGCHLD tells the kernel to notify the parent when the child finishes (so waitpid works)
  int flags = CLONE_NEWUTS | CLONE_NEWPID | SIGCHLD;

  return 0;
}