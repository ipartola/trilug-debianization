#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


int do_exit = 0;

#define LOCK_FILE "/var/run/trilug-simple/trilug-simple.pid"
#define LOG_FILE "/var/tmp/trilug.log"
#define DAEMON_OPTION "-d"

void handle_signal(int signum) {
    do_exit = 1;
}

void daemonize() {
    pid_t pid = fork();
    int fp;
    char str[16];

    if (pid != 0) {
        exit(pid < 0 ? EXIT_FAILURE : EXIT_SUCCESS);
    }
    
    umask(0);

    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    ;
    if ((fp = open(LOCK_FILE, O_RDWR|O_CREAT, 0644)) < 0 || (lockf(fp, F_TLOCK, 0)) < 0) {
        exit(EXIT_FAILURE);
    }

    snprintf(str, sizeof(str) - 2, "%d\n", getpid());
    write(fp, str, strlen(str));

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

int main(int argc, char *argv[]) {
    FILE *f = fopen(LOG_FILE, "a");
    time_t t;

    signal(SIGINT, handle_signal); 
    signal(SIGTERM, handle_signal); 

    if (argc > 1 && 0 == strcmp(DAEMON_OPTION, argv[1])) {
        daemonize();
    }

    while (!do_exit) {
        time(&t);
        fprintf(f, "Hello TriLUG! It's %s", ctime(&t));
        fflush(f);
        sleep(3);
    }

    fclose(f);
    unlink(LOCK_FILE);
}
