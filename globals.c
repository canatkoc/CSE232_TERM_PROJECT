#include "editor.h"

struct node textbuffer[MAX_LINES];
int head = -1;
int tail = -1;
int freeIndex = 0;
int editCounter = 0;
char currentFile[256] = "";
