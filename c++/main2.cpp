#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <random>
using namespace std;

enum alpha
{
    GAME,
    OVER
};

bool isover(SDL_Rect& button, int mousex, int mousey)
{
    if (mousex < button.x || mousex > button.x + button.w)
    {
        return false;
    }
    if (mousey < button.y || mousey > button.y + button.h)
    {
        return false;
    }
    return true;
}

int main()
{
    bool start = false;
    int mousex = 0;
    int mousey = 0;
    int ww = 800;
    int wh = 600;
    alpha state = GAME;
    // random
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(6, 10);
    cout << dis(gen) << endl;
    int frames = 60;
    int fps = 1000 / 60;
    int xvelo = dis(gen);
    int yvelo = -dis(gen);
    SDL_Window* window = SDL_CreateWindow("hallo hannes", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Rect button = {ww / 2 - 50, wh / 2 - 50, 100, 100};

    SDL_GetWindowSize(window, &ww, &wh);
    SDL_Rect rect = {0, wh / 2 - 100, 25, 200};
    SDL_Rect rect2 = {ww - 25, wh / 2 - 100, 25, 200};
    SDL_Rect ball = {ww / 2 - 25, wh / 2 - 25, 50, 50};

    TTF_Init();
    TTF_Font* font = TTF_OpenFont("assets/font.ttf", 42);
    SDL_Color color = {0, 0, 0};

    bool quit = false;
    SDL_Event e;
    while (!quit)
    {
        SDL_GetMouseState(&mousex, &mousey);
        while (SDL_PollEvent(&e))
        {
            if (isover(button, mousex, mousey))
            {
                if (!start)
                {
                    if (e.type == SDL_MOUSEBUTTONDOWN)
                    {
                        start = true;
                        SDL_Delay(100);
                    }
                }
            }

            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_w:
                        rect.y += -50;
                        break;
                    case SDLK_s:
                        rect.y += 50;
                        break;
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                    case SDLK_UP:
                        rect2.y += -50;
                        break;
                    case SDLK_DOWN:
                        rect2.y += 50;
                        break;
                }
            }
        }
        if (start)
        {
            ball.x += xvelo;
            ball.y += yvelo;
        }
        // gameover
        switch (state)
        {
            case GAME:
                break;
            case OVER:
                SDL_Delay(100);
                SDL_DestroyWindow(window);
                SDL_DestroyRenderer(renderer);
                SDL_Quit();

                return 1;
                break;
        } // comm
        SDL_GetWindowSize(window, &ww, &wh);
        rect2.x = ww - rect.w;

        if (ball.x < 0 || ball.x - ball.w > ww)
        {
            state = OVER;
        }

        // ball rules
        // window rules
        if (rect.y + rect.h > wh)
        {
            rect.y -= 25;
        }
        if (rect2.y + rect2.h > wh)
        {
            rect2.y -= 25;
        }
        if (rect.y < 0)
        {
            rect.y += 25;
        }
        if (rect2.y < 0)
        {
            rect2.y += 25;
        }
        // ka
        if (ball.y + ball.h > wh)
        {
            yvelo = -yvelo;
        }
        if (ball.y <= 0)
        {
            yvelo = -yvelo;
        }

        if (SDL_HasIntersection(&rect, &ball))
        {
            xvelo = dis(gen);
            yvelo = dis(gen);
            xvelo = xvelo;
        }
        if (SDL_HasIntersection(&rect2, &ball))
        {
            xvelo = dis(gen);
            yvelo = dis(gen);
            xvelo = -xvelo;
            yvelo = -yvelo;
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderFillRect(renderer, &rect2);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);
        SDL_RenderFillRect(renderer, &ball);
        if (!start)
        {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
            SDL_RenderFillRect(renderer, &button);
        }

        // Render "by alex" at the bottom middle
        SDL_Surface* textsur = TTF_RenderText_Solid(font, "by alex", color);
        SDL_Texture* textex = SDL_CreateTextureFromSurface(renderer, textsur);
        SDL_Rect textRect;
        SDL_QueryTexture(textex, NULL, NULL, &textRect.w, &textRect.h);
        textRect.x = (ww - textRect.w) / 2;
        textRect.y = wh - textRect.h - 10; // 10 pixels above the bottom
        SDL_FreeSurface(textsur);
        SDL_RenderCopy(renderer, textex, NULL, &textRect);
        SDL_DestroyTexture(textex);

        SDL_RenderPresent(renderer);

        SDL_Delay(fps);
    }

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}

