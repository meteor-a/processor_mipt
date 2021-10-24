#include "graphics.h"


#ifdef _WIN32
void DrawInTerminal(int* memory, size_t start_ind_video_mem, size_t size_mem) {
    printf("|");
    for (int ii = 0; ii < WIDTH_WINDOW; ++ii) {
        printf("-");
    }
    printf("|");
    printf("\n");

    for (int h_count = 0; h_count < HEIGHT_WINDOW; ++h_count) {
        printf("|");
        for (int w_count = 0; w_count < WIDTH_WINDOW; ++w_count) {
            if (memory[start_ind_video_mem + h_count * WIDTH_WINDOW + w_count] != 0) {
                printf("*");
            }
            else {
                printf(" ");
            }
        }
        printf("|");
        printf("\n");
    }

    printf("|");
    for (int ii = 0; ii < WIDTH_WINDOW; ++ii) {
        printf("-");
    }
    printf("|");
}

#elif __linux__

void DrawInTerminal(sf::RenderWindow window, int* memory, size_t start_ind_video_mem, size_t size_mem) {
    window(sf::VideoMode(HEIGHT_WINDOW, WIDTH_WINDOW), "CPU GRAPHICS");
    
    sf::Texture bufferTexture;
    sf::Sprite  buffferSprite;
    sf::Image   img;
    img.create(WIDTH_WINDOW, HEIGHT_WINDOW, sf::Color::White);

    for (int w_count = 0; w_count < WIDTH_WINDOW; ++w_count) {
        for (int h_count = 0; h_count < HEIGHT_WINDOW; ++h_count) {
            if(memory[start_ind_video_mem + w_count * WIDTH_WINDOW + h_count] != 0) {
                img.setPixel(w_count, h_count, sf::Color::Green);
            } else {
                img.setPixel(w_count, h_count, sf::Color::Red);
            }
        }
    }    

    bufferTexture.loadFromImage(img);
    buffferSprite.setTexture(bufferTexture);
    
    window.clear();    
    window.draw(buffferSprite);
    window.display();

    return;
}

#endif