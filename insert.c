#include "editor.h"

int hasSpace(void) {
    return freeIndex < MAX_LINES;
}

void checkAutoGC(void) {
    if (editCounter >= 10) {
        garbageCollection();
        editCounter = 0;
    }
}

void insert(int index) {
    char line[MAX_LEN];
    int newIdx;
    int nextIdx;

    if (index < 0 || index >= MAX_LINES) {
        return;
    }

    if (head == -1) {
        index = 0;
    }

    if (!hasSpace()) {
        garbageCollection();
    }

    if (!hasSpace()) {
        return;
    }

    if (fgets(line, MAX_LEN, stdin) == NULL) {
        return;
    }

    line[strcspn(line, "\n")] = '\0';

    newIdx = freeIndex;
    strcpy(textbuffer[newIdx].word, line);
    textbuffer[newIdx].next = -1;
    textbuffer[newIdx].prev = -1;

    if (head == -1) {
        head = newIdx;
        tail = newIdx;
    } else {
        nextIdx = textbuffer[index].next;
        textbuffer[newIdx].prev = index;
        textbuffer[newIdx].next = nextIdx;
        textbuffer[index].next = newIdx;
        if (nextIdx != -1) {
            textbuffer[nextIdx].prev = newIdx;
        } else {
            tail = newIdx;
        }
    }

    freeIndex++;
    editCounter++;
    checkAutoGC();
}
