
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
VALUE
captain_stack_hoge(int argc, VALUE *argv, VALUE self)
{
  VALUE buf[100];
  int lines[100];
  int n;
  n = rb_profile_frames(0, 100, buf, lines);
  return INT2FIX(n);
}

int
get_timestamp()
{
    return (int)time(NULL);
}

void
do_something(int seconds)
{
    int start = get_timestamp();
    while (1) {
        if (get_timestamp() > (start + seconds)) { break; }
    }
}

void
signal_handler(int sig)
{
    char out[20] = "Caught signal";
    write(1, "Caught signal", 20);
}


int main2(int argc, char *argv[])
{
    /* Set up SIGPROF handler */
    struct sigaction sa = {0};
    sa.sa_handler = signal_handler;  /* TODO: Replace with sigaction-style */
    sa.sa_flags |= SA_RESTART;  /* Block syscalls during while signal is handled */
    sigemptyset(&sa.sa_mask);  /* Don't block any other signals */

    /* Install! */
    if (sigaction(SIGPROF, &sa, NULL) != 0) {
        puts("sigaction failed");
        exit(EXIT_FAILURE);
    }


    /* Temporarily block SIGPROF */
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGPROF);
    if (sigprocmask(SIG_SETMASK, &mask, NULL) != 0) {
        puts("sigprocmask failed");
        exit(EXIT_FAILURE);
    }


    /* Set up timer */
    timer_t timerid;
    struct sigevent sev = {0};
    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGPROF;
    sev.sigev_value.sival_ptr = &timerid;

    if (timer_create(CLOCK_MONOTONIC, &sev, &timerid) != 0) {
        puts("timer_create failed");
        exit(EXIT_FAILURE);
    }
    printf("timerid = 0x%lx\n", (long)timerid);

    struct itimerspec its = {0};
    its.it_value.tv_sec = 1;
    its.it_interval.tv_sec = 1;
    if (timer_settime(timerid, 0, &its, NULL) != 0) {
        puts("timer_settime failed");
        exit(EXIT_FAILURE);
    }


    /* Unblock SIGPROF */
    if (sigprocmask(SIG_UNBLOCK, &mask, NULL) != 0) {
        puts("sigprocmask failed");
        exit(EXIT_FAILURE);
    }

    /* Do some work! */
    // do_something(10);
}

/* vim: set sw=4 ts=4 sts=4 et : */
