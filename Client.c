#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <ncursesw/curses.h>

#include "Client.h"

void client() {
    endwin();
    initscr();
    clear();
    printw("연결할 주소를 입력하세요 (예: 127.0.0.1)\n");
    scanw("%s", ip);
    printw("연결할 포트를 입력하세요 (예: 3456)\n");
    scanw("%s", port);
    endwin();

    // TODO: add some error checks

    client_sock = socket(AF_INET, SOCK_STREAM, 0);

    bzero((void*)&client_addr, sizeof(client_addr));
    hp = gethostbyname(ip);
    bcopy(hp->h_addr, (struct sockaddr*)&client_addr.sin_addr, hp->h_length);
    client_addr.sin_port = htons(atoi(port));
    client_addr.sin_family = AF_INET;

    connect(client_sock, (struct sockaddr*)&client_addr, sizeof(client_addr));
    
    f = fdopen(client_sock, "r+");

    // do something with f

    fclose(f);
    close(client_sock);
}