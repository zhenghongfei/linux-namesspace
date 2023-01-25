#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <sys/capability.h>
#include <sys/capability.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define STACK_SIZE (1024 * 1024)
/* Space for child's stack */
static char child_stack[STACK_SIZE];
#define errExit(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)
int child_main(void *arg) {
    for (;;) {
        printf("eUID = %ld;  eGID = %ld;  ",
                (long) geteuid(), (long) getegid());
        cap_t caps = cap_get_proc();
        /* Show the capabilities of child process */
        printf("Container - capabilities: %s\n", cap_to_text(caps, NULL));
        if (arg == NULL) break;
        sleep(5);
    }
    return 0;
}
int main(int argc, char *argv[]) {
    cap_t caps = cap_get_proc();
    printf("Parent - capabilities:%s\n", cap_to_text(caps, NULL));

    /* Create child; child commences execution in child_main() */
    pid_t pid = clone(child_main, child_stack + STACK_SIZE, 
                CLONE_NEWUSER | SIGCHLD, argv[1]);
    /* If your kernel don't support USER_NS, it will fail */
    if (pid == -1) errExit("clone");
    /* Parent falls through to here.  Wait for child. */
    if (waitpid(pid, NULL, 0) == -1) errExit("waitpid");
    exit(EXIT_SUCCESS);
}
