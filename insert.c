
#include <ncurses.h>
#include <string.h>
#include "editor.h"

int hasSpace(){
    return (free < MAX_LINES);
}

void insert(int index){
    int newPos;
    char line[MAX_LEN];

    if (!hasSpace()) {
        garbageCollection();
        editCounter = 0;
        if (!hasSpace()) {
            mvprintw(13, 0, "Buffer full! Cannot insert.");
            clrtoeol();
            return;
        }
    }

    mvprintw(13, 0, "Insert new line: ");
    clrtoeol();
    echo();
    getnstr(line, MAX_LEN - 1);
    noecho();

    newPos = free;
    strncpy(textbuffer[newPos].statement, line, MAX_LEN - 1);
    textbuffer[newPos].statement[MAX_LEN - 1] = '\0';
    free++;

    if (index == -1 || head == -1) {
        textbuffer[newPos].prev = -1;
        textbuffer[newPos].next = -1;
        head = newPos;
        tail = newPos;
    } else {
        int after = textbuffer[index].next;

        textbuffer[newPos].prev = index;
        textbuffer[newPos].next = after;
        textbuffer[index].next = newPos;

        if (after == -1) {
            tail = newPos;
        } else {
            textbuffer[after].prev = newPos;
        }
    }
}

void checkAutoGC(){
    if (editCounter >= 10 || free >= MAX_LINES) {
        garbageCollection();
        editCounter = 0;
    }
}
