// main.c
// 1. Kisi: Ana program, komut dongusu, entegrasyon
//
// Global degiskenler burada tanimlanir (editor.h'de extern olarak bildirilir).
// Ana dongu kullanici tuslarini okur ve ilgili editor fonksiyonunu cagirir.

#include <stdio.h>
#include <string.h>
#include <ncurses.h>
#include "editor.h"

// --- Global veri yapilari (PDF'e gore textbuffer, head, tail, free global) ---
struct node textbuffer[MAX_LINES];

int head = -1;        // ilk gecerli satirin index'i
int tail = -1;        // son gecerli satirin index'i
int freeIndex = 0;    // textbuffer[] sonundaki ilk bos yer
int editCounter = 0;  // 10 insert/delete sonrasi otomatik GC icin sayac

char currentFile[100] = "";   // o an acik dosyanin adi

int main(){
    int ch;
    int running = 1;
    char filename[100];

    initBuffer();   // textbuffer[]'i sifirla
    initUI();       // NCURSES ekranini baslat
    print();        // baslangic ekranini ciz

    while (running) {
        ch = getch();

        // --- E: dosya ac / duzenle ---
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

        // --- P: ekrani yenile ---
        else if (ch == 'P' || ch == 'p'){
            print();
        }

        // --- I: secili satirdan sonra yeni satir ekle ---
        else if (ch == 'I' || ch == 'i'){
            if (isLineSelected()) {
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

        // --- D: secili satiri sil ---
        else if (ch == 'D' || ch == 'd'){
            if (isLineSelected()) {
                deleteLine(cursorLine());
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

        // --- R: secili karakteri degistir ---
        else if (ch == 'R' || ch == 'r'){
            if (isLineSelected()) {
                replace(cursorLine(), cursorChar());
            } else {
                mvprintw(13, 0, "Select a line first (move cursor + ENTER)");
                clrtoeol();
                refresh();
            }
            print();
        }

        // --- S: dosyayi kaydet ---
        else if (ch == 'S' || ch == 's'){
            save();
            print();
        }

        // --- G: garbage collection ---
        else if (ch == 'G' || ch == 'g'){
            garbageCollection();
            editCounter = 0;
            print();
        }

        // --- Q: cikis ---
        else if (ch == 'Q' || ch == 'q'){
            running = 0;
        }

        // --- Ok tuslari: cursor'u ekranda hareket ettir ---
        else if (ch == KEY_UP || ch == KEY_DOWN ||
                 ch == KEY_LEFT || ch == KEY_RIGHT){
            moveCursor(ch);
        }

        // --- ENTER: cursor'un uzerindeki satiri sec ---
        // ncurses'te Enter, terminale gore '\n', '\r' veya KEY_ENTER gelebilir.
        else if (ch == '\n' || ch == '\r' || ch == KEY_ENTER){
            selectLine();
            print();
        }
    }

    closeUI();
    return 0;
}
