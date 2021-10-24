#pragma once

/*--------------------------------------------------------------------------*/

#include <cstdio>

/*--------------------------------------------------------------------------*/

static const int HEIGHT_WINDOW = 128;
static const int WIDTH_WINDOW = 128;

void DrawInTerminal(int* memory, size_t start_ind_video_mem, size_t size_mem);
