#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#define STACK_SIZE (1024 * 1024)
static char child_stack[STACK_SIZE];
char* const child_args[] = {
    "/bin/bash",
    NULL
};
int child_main(void* arg) {
    printf("Container - inside the container!\n");
    sethostname("container", 10);
    execv(child_args[0], child_args);
    printf("Error!\n");
    return 1;
}
int main() {
    printf("Parent - start a new continer!\n");
    int child_pid = clone(child_main, child_stack + STACK_SIZE, CLONE_NEWUTS | SIGCHLD, NULL);
    waitpid(child_pid, NULL, 0);
    printf("Parent - continer stopped!\n");
    return 0;
}
