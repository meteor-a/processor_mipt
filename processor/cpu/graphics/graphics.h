#pragma once

/*--------------------------------------------------------------------------*/

#include <cstdio>

/*--------------------------------------------------------------------------*/

#ifdef __linux__
    #include <SFML/Graphics.hpp>
#endif

/*--------------------------------------------------------------------------*/

static const int HEIGHT_WINDOW = 256;
static const int WIDTH_WINDOW  = 256;

#ifdef __linux__
    void DrawInTerminal(sf::RenderWindow* window, int* memory, size_t start_ind_video_mem, size_t size_mem);
#elif _WIN32
    void DrawInTerminal(int* memory, size_t start_ind_video_mem, size_t size_mem);
#endif

