#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "net.h"

void browser_run() {
    printf("Simple Browser\n");
    char url[256];
    printf("Enter URL: ");
    scanf("%s", url);

    printf("Loading %s...\n", url);
    net_send(url);
    net_receive();
}
