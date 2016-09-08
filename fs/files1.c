/* fs/files1.c */
#include <limits.h>     /* PATH_MAX */
#include <errno.h>      /* errno */
#include <sys/types.h>  /* closedir, lstat, opendir,
                                      readdir, rewinddir */
#include <stdio.h>      /* fprintf, perror, printf, putchar */
#include <stdlib.h>     /* exit */
#include <string.h>     /* strcmp */
#include <sys/stat.h>   /* lstat */
#include <dirent.h>     /* closedir, opendir, readdir, rewinddir */
#include <unistd.h>     /* chdir, getcwd, lstat */

void listdir(DIR *, int);
void showdir(char *, int);

void listdir(DIR *dp, int lev) {
    struct stat sb;
    struct dirent *dep;
    int i, errflag;
    int ndir, nreg, nchr, nblk, nfifo, nsock, nlnk, nunknown;

    errno = 0;
    ndir = nreg = nchr = nblk = nfifo = nsock = nlnk = nunknown = 0;
    while ((dep = readdir(dp)) != NULL) {
        if (lstat(dep->d_name, &sb) != 0)
            continue;   /* エラーが起きたら無視 */
        switch (sb.st_mode & S_IFMT) {
            case S_IFDIR:   ndir++; break;
            case S_IFREG:   nreg++; break;
            case S_IFCHR:   nchr++; break;
            case S_IFBLK:   nblk++; break;
            case S_IFIFO:  nfifo++; break;
            case S_IFSOCK: nsock++; break;
            case S_IFLNK:   nlnk++; break;
            default:    nunknown++; break;
        }
    }
    ndir -= 2;  /* . と .. は数えない */
    errflag = errno ? 1 : 0;    /* エラーかどうかを覚えておく */
    for (i = 0; i < ndir;     i++) putchar('/');
    for (i = 0; i < nreg;     i++) putchar('-');
    for (i = 0; i < nchr;     i++) putchar('%');
    for (i = 0; i < nblk;     i++) putchar('#');
    for (i = 0; i < nfifo;    i++) putchar('|');
    for (i = 0; i < nsock;    i++) putchar('=');
    for (i = 0; i < nlnk;     i++) putchar('@');
    for (i = 0; i < nunknown; i++) putchar('?');
    putchar('\n');

    if (errflag) return;        /* エラーがあったら降りない */

    rewinddir(dp);
    errno = 0;
    while ((dep = readdir(dp)) != NULL) {
        if (!strcmp(dep->d_name, ".")) continue;
        if (!strcmp(dep->d_name, "..")) continue;
        if (lstat(dep->d_name, &sb) != 0)
            continue;   /* エラーは無視 */
        if ((sb.st_mode & S_IFMT) == S_IFDIR)
            showdir(dep->d_name, lev + 1);      /* 相互再帰で下を処理 */
    }
}

void showdir(char *dir, int lev) {
    DIR *dp;
    char prevdir[PATH_MAX+1];
    int i;

    if (getcwd(prevdir, sizeof(prevdir)) == NULL) {
        perror("getcwd");
        return;         /* 戻ってこられないから降りない */
    }
    for (i = 0; i < lev * 2; i++)
        putchar(' ');
    printf("%s: ", dir);
    if (chdir(dir) != 0) {
        printf("<cannot chdir>\n");
    } else if ((dp = opendir(".")) == NULL) {
        printf("<cannot list>\n");
    } else {
        listdir(dp, lev);       /* listdir は相互再帰で showdir を呼ぶ */
        closedir(dp);
    }
    if (chdir(prevdir) != 0) {
        perror("chdir");
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "usage: files1 <directory>\n");
        exit(1);
    }
    showdir(argv[1], 0);
    return 0;
}
