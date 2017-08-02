//
// Created by lijianqi on 2017/8/2.
//
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "linenoise.h"
#include "sds.h"


static void completionCallback(const char *buf, linenoiseCompletions *lc) {
    size_t startpos = 0;
    int mask;
    int i;
    size_t matchlen;
    sds tmp;

    if (strncasecmp(buf,"help ",5) == 0) {
        linenoiseAddCompletion(lc,"hello");
    }

}

static void repl(void) {
    sds historyfile = NULL;

    char *line;
    int argc;
    sds *argv;


    linenoiseSetMultiLine(1);
    linenoiseSetCompletionCallback(completionCallback);

    while((line = linenoise("hello> ")) != NULL) {
        /* Do something with the string. */
        if (line[0] != '\0' && line[0] != '/') {
            printf("echo: '%s'\n", line);

        } else if (line[0] == '/') {
            printf("Unreconized command: %s\n", line);
        }
        free(line);
    }

}

int main(int argc, char **argv){

    repl();
    return 0;
}