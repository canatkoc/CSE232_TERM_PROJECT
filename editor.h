
#ifndef EDITOR_H
#define EDITOR_H

#define MAX_LINES 100
#define MAX_LEN 40

struct node{
    char statement[MAX_LEN];
    int next;
    int prev;
};

extern struct node textbuffer[MAX_LINES];

extern int head;
extern int tail;
extern int free;
extern int editCounter;

extern char currentFile[100];

void initBuffer();

void edit(char *filename);
void save();

void insert(int index);
void delete(int index);
void replace(int charIndex);
int hasSpace();

void print();

int cursorLine();
int cursorChar();
void moveCursor(int key);
void selectLine();
int isLineSelected();
void clearSelection();

int garbageCollection();

void initUI();
void closeUI();

void checkAutoGC();
int getLineCount();
void debugPrintBuffer();

#endif
