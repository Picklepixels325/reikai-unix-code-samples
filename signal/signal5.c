#include <signal.h>	/* signal */
#include <stdio.h>	/* fprintf */
#include <unistd.h>	/* sleep */
void handler (int sig)
{
    fprintf (stderr, "@");
    sleep (3);
}
int main (void)
{
    signal (SIGINT, handler);
    while (1) {
        sleep (1);
        fprintf (stderr, ".");
    }
}
