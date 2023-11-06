#include "src/common.h"
#include <string.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/time.h>

#define REPEAT 10

void __attribute__((__noreturn__)) die(const char* format, ...) {
    va_list args;
    va_start(args, format);
    printf("\x1b[1;31mError: ");
    vprintf(format, args);
    printf("\x1b[0m");
    va_end(args);
    exit(1);
}

void test(const char* name) {
    static char buf[256];
    { // Check if file exists
        sprintf(buf, "./src/%s.c", name);
        struct stat path_stat;
        stat(buf, &path_stat);
        if (!S_ISREG(path_stat.st_mode))
            die("%s is not valid\n", name);
    }
    { // Run make
        sprintf(buf, "NAME=%s make build_test", name);
        if (system(buf) != 0)
            die("Failed to run makefile");
    }
    static number (*foo)(number) = NULL;
    static void *lib;
    { // Load foo
        sprintf(buf, "./src/%s.so", name);
        lib = dlopen(buf, RTLD_LAZY);
        if (!lib)
            die("Failed to load %s: %s\n", buf, strerror(errno));
        foo = dlsym(lib, "test");
        if (!foo) {
            dlclose(lib);
            die("Failed to load test fn from %s\n", buf, strerror(errno));
        }
    }
    static struct timeval start, end;
    static number out = 0, outtemp = 0;
    printf("* %s  \n    | Number | Time (ms) |\n    | --- | --- |\n", name);
    for (number size = 10; size < 100000; size *= 10) {
        gettimeofday(&start, NULL);
        for (uint_fast64_t j = 0; j < REPEAT; ++j) {
            outtemp = foo(size);
            if (j == 0)
                out = outtemp;
            else if (out != outtemp)
                die("Inconsistent output %f != %f for %s\n", out, outtemp, name);
        }
        gettimeofday(&end, NULL);
        double time = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_usec - start.tv_usec) / 1000;
        time /= REPEAT;
        printf("    | %lu | %f |\n", size, time);
    }
    dlclose(lib);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s [all|<name>] ...<names>\n", argv[0]);
        return 1;
    }
    if (argc == 2 && strcmp(argv[0], "all")) {
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir("./src/")) == NULL)
            die("Failed to open ./src/: %s\n", strerror(errno));
        while ((ent = readdir(dir)) != NULL) {
            static char *p;
            p = strrchr(ent->d_name, '.');
            if (p == NULL) continue;
            if (strcmp(p, ".c") != 0) continue;
            *p = '\0';
            test(ent->d_name);
        }
        closedir(dir);
    } else {
        for (int i = 1; i < argc; ++i)
            test(argv[i]);
    }
}