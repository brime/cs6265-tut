#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flag.h"

#define COUNT 16
#define ASSERT(cond) if(!(cond)) exit(100);

int getint() {
    char buf[16] = {0};
    ASSERT(read(0, buf, 15) >= 1);
    return atoi(buf);
}

typedef struct {
    char *buf;
    size_t size;
} note;

char* admin_ptr = NULL;

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);

    note notes[COUNT] = {0};
    unsigned int index = 0, size = 0;
    while(1) {
        printf("====== Option ======\n1. add note\n2. delete note\n3. edit note\n4. call admin\n");

        switch(getint()) {
            case 0:
                return 1;
            case 1: {
                printf("Index: ");
                index = getint();
                if(index >= COUNT)
                    return 1;
                printf("Size: ");
                size = getint();
                if(size >= sizeof(void *) * 128) {
                    return 2;
                }
                notes[index].buf = malloc(size);
                notes[index].size = size;
                if(!notes[index].buf) {
                    printf("Out of memory!\n");
                    return 3;
                }
                printf("Added!\n");
                break;
            }
            case 2: {
                printf("Index: ");
                index = getint();
                if(index >= COUNT)
                    return 1;
                free(notes[index].buf);
                notes[index].size = 0;
                printf("Deleted!\n");
                break;
            }
            case 3: {
                printf("Index: ");
                index = getint();
                if(index >= COUNT)
                    return 1;
                printf("Data: ");
                memset(notes[index].buf, 0, notes[index].size);
                read(0, notes[index].buf, notes[index].size);
                break;
            }
            case 4: {
                if (!admin_ptr) {
                    admin_ptr = malloc(0x20);
                    strcpy(admin_ptr, "Admin is not here.\n");
                }
                printf("Oh, %s", admin_ptr);
                if(admin_ptr && !strcmp(admin_ptr, "Admin is back!\n")) {
                    printf("Now you can get your key :)\n\n");
                    print_key("");
                    return 0;
                }
                break;
            }
            default:
                printf("Invalid option!\n");
        }
    }
}
