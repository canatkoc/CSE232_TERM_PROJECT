#include "editor.h"
#include <stdio.h>
#include <string.h>

void edit(char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) return;

    initBuffer();

    char line[MAX_LEN];
    int i = 0;

    while (fgets(line, MAX_LEN, f) && i < MAX_LINES) {
        line[strcspn(line, "\n")] = '\0';
        strcpy(textbuffer[i].word, line);
        textbuffer[i].prev = i - 1;
        textbuffer[i].next = i + 1;
        i++;
    }

    if (i > 0) {
        head = 0;
        tail = i - 1;
        textbuffer[head].prev = -1;
        textbuffer[tail].next = -1;
        freeIndex = i;
    }

    fclose(f);
}

void save() {
    if (currentFile[0] == '\0') return;

    FILE *f = fopen(currentFile, "w");
    if (f == NULL) return;

    int current = head;
    while (current != -1) {
        fprintf(f, "%s\n", textbuffer[current].word);
        current = textbuffer[current].next;
    }

    fclose(f);
}
