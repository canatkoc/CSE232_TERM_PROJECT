#include <ncurses.h>
#include <string.h>
#include "editor.h"

#define TEXT_START_ROW 16
#define TEXT_START_COL 4
#define MAX_VISIBLE 30

static int cursorScreenLine = 0;
static int cursorScreenChar = 0;

static int selectedScreenLine = -1;


void initUI(){
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);
}

void closeUI(){
    curs_set(1);
    endwin();
}

void print(){
    int current;
    int screenRow;
    int count;

    clear();

    mvprintw(0, 0, "=== Simple Text Editor ===");
    mvprintw(2, 0, "Commands:");
    mvprintw(3, 2, "E: Edit/Open file     I: Insert line");
    mvprintw(4, 2, "D: Delete line        R: Replace character");
    mvprintw(5, 2, "P: Print/Refresh      S: Save");
    mvprintw(6, 2, "G: Garbage Collection Q: Quit");
    mvprintw(8, 0, "Move cursor: arrow keys   Select line: ENTER");

    mvprintw(10, 0, "Current file: %s", currentFile[0] ? currentFile : "(none)");
    mvprintw(11, 0, "Lines: %d", getLineCount());
    if (selectedScreenLine >= 0) {
        mvprintw(11, 20, "| Selected line: %d", selectedScreenLine + 1);
    } else {
        mvprintw(11, 20, "| No line selected (press ENTER)");
    }

    mvprintw(TEXT_START_ROW - 2, 0, "----- TEXT -----");

    current = head;
    screenRow = TEXT_START_ROW;
    count = 0;

    while (current != -1 && count < MAX_VISIBLE) {
        if (count == selectedScreenLine) {
            mvprintw(screenRow, 0, "> ");
        } else {
            mvprintw(screenRow, 0, "  ");
        }
        mvprintw(screenRow, TEXT_START_COL, "%s", textbuffer[current].word);
        current = textbuffer[current].next;
        screenRow++;
        count++;
    }

    if (head == -1) {
        mvprintw(TEXT_START_ROW, TEXT_START_COL, "(empty - use E to open a file)");
    }

    if (count == 0) {
        cursorScreenLine = 0;
        cursorScreenChar = 0;
        selectedScreenLine = -1;
    } else {
        if (cursorScreenLine < 0) cursorScreenLine = 0;
        if (cursorScreenLine > count - 1) cursorScreenLine = count - 1;
        if (selectedScreenLine > count - 1) selectedScreenLine = -1;
    }

    move(TEXT_START_ROW + cursorScreenLine, TEXT_START_COL + cursorScreenChar);

    refresh();
}

int cursorLine(){
    int current = head;
    int target;
    int i;

    if (current == -1) return -1;

    target = (selectedScreenLine >= 0) ? selectedScreenLine : cursorScreenLine;

    for (i = 0; i < target && current != -1; i++) {
        current = textbuffer[current].next;
    }

    if (current == -1) {
        current = tail;
    }

    return current;
}

void selectLine(){
    if (head == -1) {
        selectedScreenLine = -1;
        return;
    }
    selectedScreenLine = cursorScreenLine;
}

int isLineSelected(){
    return (selectedScreenLine >= 0);
}

void clearSelection(){
    selectedScreenLine = -1;
}

int cursorChar(){
    int line = cursorLine();
    int len;

    if (line == -1) return 0;

    len = (int)strlen(textbuffer[line].word);

    if (cursorScreenChar < 0) cursorScreenChar = 0;
    if (len > 0 && cursorScreenChar > len - 1) cursorScreenChar = len - 1;
    if (len == 0) cursorScreenChar = 0;

    return cursorScreenChar;
}

void replace(int lineIndex, int charIndex){
    int len;
    int newCh;

    if (lineIndex < 0 || lineIndex >= MAX_LINES) return;

    len = (int)strlen(textbuffer[lineIndex].word);
    if (len == 0) return;
    if (charIndex < 0 || charIndex >= len) return;

    mvprintw(TEXT_START_ROW - 1, 0, "Replace char at col %d: ", charIndex);
    clrtoeol();
    move(TEXT_START_ROW + cursorScreenLine, TEXT_START_COL + charIndex);
    refresh();

    newCh = getch();

    if (newCh != ERR && newCh >= 32 && newCh < 127) {
        textbuffer[lineIndex].word[charIndex] = (char)newCh;
    }
}

void moveCursor(int key){
    int count = getLineCount();
    int line;
    int len;

    switch (key) {
        case KEY_UP:
            if (cursorScreenLine > 0) cursorScreenLine--;
            break;
        case KEY_DOWN:
            if (count > 0 && cursorScreenLine < count - 1) cursorScreenLine++;
            break;
        case KEY_LEFT:
            if (cursorScreenChar > 0) cursorScreenChar--;
            break;
        case KEY_RIGHT:
            line = cursorLine();
            if (line != -1) {
                len = (int)strlen(textbuffer[line].word);
                if (len > 0 && cursorScreenChar < len - 1) cursorScreenChar++;
            }
            break;
        default:
            break;
    }

    move(TEXT_START_ROW + cursorScreenLine, TEXT_START_COL + cursorScreenChar);
    refresh();
}
