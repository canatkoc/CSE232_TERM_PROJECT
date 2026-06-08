
#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include "editor.h"

struct node textbuffer[MAX_LINES];

int head = -1;
int tail = -1;
int free = 0;
int editCounter = 0;

char currentFile[100] = "";

int main(){
    int ch;
    int running = 1;
    char filename[100];

    initBuffer();
    initUI();
    print();

    while (running) {
        ch = getch();

        if (ch == 'E' || ch == 'e'){
            echo();
            mvprintw(13, 0, "Enter filename: ");
            clrtoeol();
            getnstr(filename, 99);
            noecho();

            strcpy(currentFile, filename);
            edit(filename);
            print();
        }

        else if (ch == 'P' || ch == 'p'){
            print();
        }

        else if (ch == 'I' || ch == 'i'){
            if (head == -1) {
                insert(-1);
                editCounter++;
                checkAutoGC();
            } else if (isLineSelected()) {
                insert(cursorLine());
                editCounter++;
                checkAutoGC();
                clearSelection();
            } else {
                mvprintw(13, 0, "Select a line first (move cursor + ENTER)");
                clrtoeol();
                refresh();
            }
            print();
        }

        else if (ch == 'D' || ch == 'd'){
            if (isLineSelected()) {
                delete(cursorLine());
                editCounter++;
                checkAutoGC();
                clearSelection();
            } else {
                mvprintw(13, 0, "Select a line first (move cursor + ENTER)");
                clrtoeol();
                refresh();
            }
            print();
        }

        else if (ch == 'R' || ch == 'r'){
            if (isLineSelected()) {
                replace(cursorChar());
            } else {
                mvprintw(13, 0, "Select a line first (move cursor + ENTER)");
                clrtoeol();
                refresh();
            }
            print();
        }

        else if (ch == 'S' || ch == 's'){
            save();
            print();
        }

        else if (ch == 'G' || ch == 'g'){
            garbageCollection();
            editCounter = 0;
            print();
        }

        else if (ch == 'Q' || ch == 'q'){
            running = 0;
        }

        else if (ch == KEY_UP || ch == KEY_DOWN ||
                 ch == KEY_LEFT || ch == KEY_RIGHT){
            moveCursor(ch);
        }

        else if (ch == '\n' || ch == '\r' || ch == KEY_ENTER){
            selectLine();
            print();
        }
    }

    closeUI();
    return 0;
}
