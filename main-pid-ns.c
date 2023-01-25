#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#define STACK_SIZE (1024 * 1024)
static char child_stack[STACK_SIZE];

char* const child_args[] = {
    "/bin/bash",
    NULL
};

int child_main(void* arg) {
    printf("Container [%5d] - inside the container!\n", getpid());
    system("mount -t proc proc /proc");
    sethostname("container", 10);
    execv(child_args[0], child_args);
    printf("Ooops\n");
    return 1;
}

int main() {
    printf("Parent [%5d] - start a new container!\n", getpid());
    system("mount --make-private -t proc proc /proc");
    int child_pid = clone(child_main, child_stack + STACK_SIZE, CLONE_NEWUTS | CLONE_NEWPID | CLONE_NEWNS | SIGCHLD, NULL);
    waitpid(child_pid, NULL, 0);
    printf("Parent [%5d] - container [%5d] stopped!\n", getpid(), child_pid);
    return 0;
}
