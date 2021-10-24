#include "graphics.h"

void DrawInTerminal(int* memory, size_t start_ind_video_mem, size_t size_mem) {
    for (size_t ii = start_ind_video_mem + 1; ii < size_mem; ++ii) {
        if (memory[ii] != 0) {
            printf("*");
        }
        else {
            printf(" ");
        }
        if (ii % WIDTH_WINDOW == 0) {
            printf("\n");
        }
    }
}
