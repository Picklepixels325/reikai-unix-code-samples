#include <setjmp.h>	/* siglongjmp, sigsetjmp */
#include <signal.h>	/* sigaction, sigemptyset, sigismember, sigprocmask */
#include <stdio.h>	/* printf */
static sigjmp_buf env;

void sig_handler (int sig)
{
    printf ("handler\n");
    siglongjmp (env, 1);
}

int main (void)
{
    int dummy;
    int *p = (int *)0x123;
    struct sigaction act;
    sigset_t old_sigset;

    act.sa_handler = sig_handler;
    sigemptyset (&act.sa_mask);
    act.sa_flags = 0;
    sigaction (SIGSEGV, &act, NULL);
    sigaction (SIGBUS, &act, NULL);
    if (sigsetjmp (env, 1) == 0) {
        printf ("main(1)\n");
        dummy = *p;
    }
    printf ("main(2)\n");
    sigprocmask (SIG_SETMASK, NULL, &old_sigset);
    printf ("%d\n", sigismember (&old_sigset, SIGSEGV));
}
