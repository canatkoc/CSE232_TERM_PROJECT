#include "editor.h"
#include <stdio.h>

void initBuffer(){
    int i;

    head = -1;
    tail = -1;
    free = 0;
    editCounter = 0;

    for (i = 0; i < MAX_LINES; i++){
        textbuffer[i].statement[0] = '\0';
        textbuffer[i].next = -1;
        textbuffer[i].prev = -1;
    }
}
int getLineCount() {
    int count = 0;
    int current = head;
    while (current != -1) {
        count++;
        current = textbuffer[current].next;
    }
    return count;
}

void debugPrintBuffer() {
    int i;
    printf("HEAD: %d | TAIL: %d | FREE: %d\n", head, tail, free);
    for (i = 0; i < MAX_LINES; i++) {
        if (textbuffer[i].statement[0] != '\0') {
            printf("[%d] prev:%d | '%s' | next:%d\n",
                i,
                textbuffer[i].prev,
                textbuffer[i].statement,
                textbuffer[i].next);
        }
    }
}
