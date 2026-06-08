
#include <string.h>
#include "editor.h"

void delete(int index){
    int prevIdx;
    int nextIdx;

    if (index < 0 || index >= MAX_LINES) return;
    if (head == -1) return;

    prevIdx = textbuffer[index].prev;
    nextIdx = textbuffer[index].next;

    if (prevIdx != -1) {
        textbuffer[prevIdx].next = nextIdx;
    } else {
        head = nextIdx;
    }

    if (nextIdx != -1) {
        textbuffer[nextIdx].prev = prevIdx;
    } else {
        tail = prevIdx;
    }

    textbuffer[index].next = -1;
    textbuffer[index].prev = -1;
}

int garbageCollection(){
    struct node temp[MAX_LINES];
    int count = 0;
    int current = head;
    int i;

    while (current != -1 && count < MAX_LINES) {
        strncpy(temp[count].statement, textbuffer[current].statement, MAX_LEN - 1);
        temp[count].statement[MAX_LEN - 1] = '\0';
        count++;
        current = textbuffer[current].next;
    }

    for (i = 0; i < MAX_LINES; i++) {
        textbuffer[i].statement[0] = '\0';
        textbuffer[i].next = -1;
        textbuffer[i].prev = -1;
    }

    for (i = 0; i < count; i++) {
        strcpy(textbuffer[i].statement, temp[i].statement);
        textbuffer[i].prev = i - 1;
        textbuffer[i].next = i + 1;
    }

    if (count > 0) {
        head = 0;
        tail = count - 1;
        textbuffer[head].prev = -1;
        textbuffer[tail].next = -1;
        free = count;
    } else {
        head = -1;
        tail = -1;
        free = 0;
    }

    return count;
}
