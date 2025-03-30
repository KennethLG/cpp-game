#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

class Vector2 {
public:
  int x;
  int y;

  Vector2(int x, int y) : x(x), y(y) {}
};

class Instance {
public:
  Vector2 position;
  virtual ~Instance() = default;

  Instance(int x, int y) : position(x, y) {}
};

class Player : public Instance {
public:
  std::string spriteName;

  Player(int x, int y, const std::string &spriteName)
      : Instance(x, y), spriteName(spriteName) {}
};

class IRenderer {
public:
  virtual ~IRenderer() = default;
  virtual bool loadTexture(const std::string &filePath) = 0;
  virtual void render() = 0;
};

class SDLRenderer : public IRenderer {
private:
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_Texture *texture = nullptr;

public:
  SDLRenderer(int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
      throw std::runtime_error("SDL could not initialize! SDL_Error: " +
                               std::string(SDL_GetError()));
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
      throw std::runtime_error("SDL_image could not initialize! IMG_Error: " +
                               std::string(IMG_GetError()));
    }

    window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, width, height,
                              SDL_WINDOW_SHOWN);
    if (!window) {
      throw std::runtime_error("Window creation failed! SDL_Error: " +
                               std::string(SDL_GetError()));
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
      throw std::runtime_error("Renderer creation failed! SDL_Error: " +
                               std::string(SDL_GetError()));
    }
  }

  bool loadTexture(const std::string &filePath) override {
    texture = IMG_LoadTexture(renderer, filePath.c_str());
    if (!texture) {
      std::cerr << "Failed to load image! IMG_Error: " << IMG_GetError()
                << std::endl;
      return false;
    }
    return true;
  }

  void render() override {
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
  }

  ~SDLRenderer() {
    if (texture)
      SDL_DestroyTexture(texture);
    if (renderer)
      SDL_DestroyRenderer(renderer);
    if (window)
      SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
  }
};

class IInputHandler {
public:
  virtual ~IInputHandler() = default;
  virtual bool processInput() = 0;
};

class SDLInputHandler : public IInputHandler {
public:
  bool processInput() override {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        return false;
      }
    }
    return true;
  }
};

class Game {
private:
  IInputHandler &inputHandler;
  IRenderer &renderer;
  std::vector<Instance*> instances;

public:
  Game(IInputHandler &i, IRenderer &r) : inputHandler(i), renderer(r) {}

  void run() {
    bool running = true;
    while (running) {
      running = inputHandler.processInput();
      renderer.render();
    }
  }

  void init() { 
    instances.emplace_back(new Player(100, 100, "player.png"));
  }
};

int main() {
  SDLInputHandler inputHandler;
  SDLRenderer renderer(SCREEN_WIDTH, SCREEN_HEIGHT);
  Game game(inputHandler, renderer);
  game.run();
  return 0;
}
