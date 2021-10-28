#pragma once

/*--------------------------------------------------------------------------*/

#include <cstdio>

/*--------------------------------------------------------------------------*/

#include "../../constants/constants.h"
#ifdef __linux__
    #include <SFML/Graphics.hpp>
#endif

/*--------------------------------------------------------------------------*/

#ifdef __linux__
    void DrawInTerminal(sf::RenderWindow* window, int* memory, size_t start_ind_video_mem, size_t size_mem);
#elif _WIN32
    void DrawInTerminal(int* memory, size_t start_ind_video_mem, size_t size_mem);
#endif

