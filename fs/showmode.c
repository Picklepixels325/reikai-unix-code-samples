/* fs/showmode.c */
#include <stdio.h>      /* perror, puts */
#include <stdlib.h>     /* exit */
#include <sys/types.h>  /* stat */
#include <sys/stat.h>   /* stat */
#include <unistd.h>     /* stat */

enum {
    PERMLEN = 9,        /* rw-r--r-- の長さ */
};

int main(int argc, char **argv) {
    struct stat sbuf;
    int i;
    char perm[PERMLEN+1];       /* +1 は '\0' のため */

    if (stat(argv[1], &sbuf) == -1) {  
        perror("stat");
        exit(1);
    }

    for (i = 0; i < PERMLEN; i++)
        perm[i] = '-';
    perm[PERMLEN] = '\0';

    if (sbuf.st_mode & S_IRUSR) perm[0] = 'r';
    if (sbuf.st_mode & S_IWUSR) perm[1] = 'w';
    if (sbuf.st_mode & S_IXUSR) perm[2] = 'x';
    if (sbuf.st_mode & S_ISUID) perm[2] = 's';
    if (sbuf.st_mode & S_IRGRP) perm[3] = 'r';
    if (sbuf.st_mode & S_IWGRP) perm[4] = 'w';
    if (sbuf.st_mode & S_IXGRP) perm[5] = 'x';
    if (sbuf.st_mode & S_ISGID) perm[5] = 's';
    if (sbuf.st_mode & S_IROTH) perm[6] = 'r';
    if (sbuf.st_mode & S_IWOTH) perm[7] = 'w';
    if (sbuf.st_mode & S_IXOTH) perm[8] = 'x';
    if (sbuf.st_mode & S_ISVTX) perm[8] = 't';

    puts(perm);
    return 0;
}
