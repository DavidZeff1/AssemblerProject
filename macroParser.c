#include "macroParser.h"

void macroParser(int argc, char **argv) {
    FILE *fp, *fd;
    char str[maxLineLength] = {0};
    char c;
    int strLen = 0, countLine = 0;
    int macroParsingState = 0; // 0 = not parsing, 1 = parsing macro name, 2 = parsing macro contents
    macronode *head = NULL;
    macronode *temp = NULL;

    fp = fopen("unParsedProg", "r");
    fd = fopen("Prog.as", "w");

    if (!fp || !fd) {
        fprintf(stderr, "Error opening files.\n");
        if (fp) fclose(fp);
        if (fd) fclose(fd);
        return;
    }

    while ((c = fgetc(fp)) != EOF) {
        countLine++;

        // Skip comments and handle empty lines (define SKIP_COMMENT and CLOSE_EMPTYLINE)
        SKIP_COMMENT
        CLOSE_EMPTYLINE

        if (c != ' ' && c != ',' && c != '\n') {
            str[strLen++] = c;
        } else if (strLen > 0) {
            str[strLen] = '\0';

            if (strcmp(str, "mcro") == 0) {
                macroParsingState = 1; // Start parsing macro
                RESET_STRING
                continue;
            }

            if (macroParsingState == 1) {
                head = addNode(head, str, "name");
                macroParsingState = 2; // Ready to parse macro contents
                RESET_STRING
                continue;
            }

            if (strcmp(str, "endmcro") == 0) {
                macroParsingState = 0; // End macro parsing
                RESET_STRING
                continue;
            }

            if (macroParsingState == 2) {
                str[strLen++] = c;
                str[strLen] = '\0';
                head = addNode(head, str, "contents");
                RESET_STRING
                continue;
            }

            temp = findNode(head, str);
            if (temp) {
                fprintf(fd, "%s", temp->contents);
                RESET_STRING
                temp = NULL;
                continue;
            }

            fprintf(fd, "%s%c", str, c);
            RESET_STRING
        } else {
            fprintf(fd, "%c", c);
        }
    }

    freeList(head);
    fclose(fp);
    fclose(fd);
}













	
