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
void DrawInTerminal(int* memory, size_t start_ind_video_mem, size_t size_mem) {
    sf::RenderWindow window(sf::VideoMode(200, 200), "Hello From SFML");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Magenta);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear();
        window.draw(shape);
        window.display();

    }

    return;
}
#endif