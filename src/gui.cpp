#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>
#include <unordered_map>

#include "critical_scenarios.h"
#include "gui.h"
#include "is_it_fns.h"
#include "movement.h"
#include "side_notes.h"
#include "variables.h"

const int BOARD_SIZE = 800;
const int SIDEBAR_WIDTH = 350;
const int WINDOW_WIDTH = BOARD_SIZE + SIDEBAR_WIDTH;
const int WINDOW_HEIGHT = BOARD_SIZE;
const int GRID_SIZE = BOARD_SIZE / 8;

struct Button {
  SDL_Rect rect;
  const char *label;
  void (*action)();
};

static Button sidebar_buttons[] = {
    {{BOARD_SIZE + 50, 150, 250, 50}, "UNDO", undo},
    {{BOARD_SIZE + 50, 220, 250, 50}, "REDO", redo},
    {{BOARD_SIZE + 50, 290, 250, 50}, "SAVE", save_game},
    {{BOARD_SIZE + 50, 360, 250, 50}, "LOAD", load_game}};

static SDL_Window *window = nullptr;
static SDL_Renderer *renderer = nullptr;
static TTF_Font *global_font = nullptr;

static bool running = false;
static bool is_fullscreen = false;
static int selected_row = -1;
static int selected_col = -1;
static std::unordered_map<char, SDL_Texture *> piece_textures;
static const char *active_sidenote = nullptr;

static bool is_promoting = false;
static char pending_c1, pending_c2;
static int pending_r1, pending_r2;

static void execute_move_and_evaluate(char c1, int r1, char c2, int r2,
                                      char prom_piece) {
  if (apply_gui_move(current.board, c1, r1, c2, r2, prom_piece)) {
    if (checkmate(current.board)) {
      if (current.white_win)
        active_sidenote = "CHECKMATE! White Wins!";
      else if (current.black_win)
        active_sidenote = "CHECKMATE! Black Wins!";
    } else if (stalemate(current.board)) {
      active_sidenote = "STALEMATE! It's a draw.";
    } else if (check(current.board, current.moves_played)) {
      active_sidenote = "CHECK!";
    } else {
      active_sidenote = get_random_sidenote();
    }
  }
}

static void load_textures() {
  std::unordered_map<char, std::string> texture_files = {
      {'p', "assets/wp.png"}, {'r', "assets/wr.png"}, {'n', "assets/wn.png"},
      {'b', "assets/wb.png"}, {'q', "assets/wq.png"}, {'k', "assets/wk.png"},
      {'P', "assets/bp.png"}, {'R', "assets/br.png"}, {'N', "assets/bn.png"},
      {'B', "assets/bb.png"}, {'Q', "assets/bq.png"}, {'K', "assets/bk.png"}};

  for (const auto &pair : texture_files) {
    SDL_Texture *tex = IMG_LoadTexture(renderer, pair.second.c_str());
    if (!tex)
      std::cerr << "Failed to load: " << pair.second << "\n";
    piece_textures[pair.first] = tex;
  }
}

static void handle_click(int x, int y) {
  if (x < BOARD_SIZE) {
    if (current.white_win || current.black_win || stalemate(current.board))
      return;
    if (is_promoting) {
      int box_x = 200, box_y = 350, box_w = 400, box_h = 100;
      if (x >= box_x && x <= box_x + box_w && y >= box_y &&
          y <= box_y + box_h) {
        char choices[] = {'q', 'r', 'b', 'n'};
        int idx = (x - box_x) / 100;
        execute_move_and_evaluate(pending_c1, pending_r1, pending_c2,
                                  pending_r2, choices[idx]);
        is_promoting = false;
        selected_row = -1;
        selected_col = -1;
      }
      return;
    }

    int clicked_col = x / GRID_SIZE;
    int clicked_row = y / GRID_SIZE;

    if (selected_row == -1) {
      char piece = current.board[clicked_row][clicked_col];
      if (piece != '.' && piece != '-') {
        selected_row = clicked_row;
        selected_col = clicked_col;
      }
    }
    else {
      char c1 = selected_col + 'A';
      int r1 = 8 - selected_row;
      char c2 = clicked_col + 'A';
      int r2 = 8 - clicked_row;

      if (is_valid_move(current.board, c1, r1, c2, r2, 0)) {
        char moving_piece = current.board[selected_row][selected_col];
        int target_row = 8 - r2;

        bool will_promote = (moving_piece == 'p' && target_row == 0) ||
                            (moving_piece == 'P' && target_row == 7);

        if (will_promote) {
          is_promoting = true;
          pending_c1 = c1;
          pending_r1 = r1;
          pending_c2 = c2;
          pending_r2 = r2;
          return;
        } else {
          execute_move_and_evaluate(c1, r1, c2, r2, '\0');
        }
      }

      selected_row = -1;
      selected_col = -1;
    }
  } else {
    for (auto &btn : sidebar_buttons) {
      if (x >= btn.rect.x && x < btn.rect.x + btn.rect.w && y >= btn.rect.y &&
          y < btn.rect.y + btn.rect.h) {
        if (is_promoting) {
          is_promoting = false;
          selected_row = -1;
          selected_col = -1;
        }
        btn.action();
        if (current.white_win || current.black_win)
          active_sidenote = "CHECKMATE!";
        else if (stalemate(current.board))
          active_sidenote = "STALEMATE!";
        else
          active_sidenote = get_random_sidenote();
        break;
      }
    }
  }
}

static void render_frame() {
  SDL_SetRenderDrawColor(renderer, 128, 120, 117, 255);
  SDL_RenderClear(renderer);

  for (int r = 0; r < 8; r++) {
    for (int c = 0; c < 8; c++) {
      SDL_Rect rect = {c * GRID_SIZE, r * GRID_SIZE, GRID_SIZE, GRID_SIZE};

      if ((r + c) % 2 == 0)
        SDL_SetRenderDrawColor(renderer, 240, 217, 181, 255);
      else
        SDL_SetRenderDrawColor(renderer, 100, 150, 200, 255);
      SDL_RenderFillRect(renderer, &rect);

      if (r == selected_row && c == selected_col) {
        SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
        SDL_RenderFillRect(renderer, &rect);
      }

      if (selected_row != -1) {
        char c1 = selected_col + 'A';
        int r1 = 8 - selected_row;
        char c2 = c + 'A';
        int r2 = 8 - r;

        if (is_valid_move(current.board, c1, r1, c2, r2, 0)) {
          SDL_SetRenderDrawColor(renderer, 100, 200, 100, 200);
          SDL_Rect dot_rect = {c * GRID_SIZE + (GRID_SIZE / 2) - 15,
                               r * GRID_SIZE + (GRID_SIZE / 2) - 15, 30, 30};
          SDL_RenderFillRect(renderer, &dot_rect);
        }
      }

      char piece = current.board[r][c];
      if (piece_textures.count(piece))
        SDL_RenderCopy(renderer, piece_textures[piece], NULL, &rect);
    }
  }

  for (const auto &btn : sidebar_buttons) {
    SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
    SDL_RenderFillRect(renderer, &btn.rect);

    if (global_font) {
      SDL_Color white = {255, 255, 255, 255};
      SDL_Surface *surf = TTF_RenderText_Blended(global_font, btn.label, white);
      if (surf) {
        SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_Rect textRect = {btn.rect.x + (btn.rect.w - surf->w) / 2,
                             btn.rect.y + (btn.rect.h - surf->h) / 2, surf->w,
                             surf->h};
        SDL_RenderCopy(renderer, tex, NULL, &textRect);
        SDL_FreeSurface(surf);
        SDL_DestroyTexture(tex);
      }
    }
  }

  if (active_sidenote && global_font) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *surf = TTF_RenderText_Blended_Wrapped(
        global_font, active_sidenote, white, SIDEBAR_WIDTH - 100);

    if (surf) {
      SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, surf);
      SDL_Rect textRect = {BOARD_SIZE + (SIDEBAR_WIDTH - surf->w) / 2, 50,
                           surf->w, surf->h};
      SDL_RenderCopy(renderer, tex, NULL, &textRect);
      SDL_FreeSurface(surf);
      SDL_DestroyTexture(tex);
    }
  }

  for (int i = 0; i < current.white_eaten_count; i++) {
    char piece = current.white_eaten[i];
    if (piece_textures.count(piece)) {
      int x_pos = BOARD_SIZE + 25 + (i % 6) * 50;
      int y_pos = 480 + (i / 6) * 50;
      SDL_Rect dest = {x_pos, y_pos, 40, 40};
      SDL_RenderCopy(renderer, piece_textures[piece], NULL, &dest);
    }
  }

  for (int i = 0; i < current.black_eaten_count; i++) {
    char piece = current.black_eaten[i];
    if (piece_textures.count(piece)) {
      int x_pos = BOARD_SIZE + 25 + (i % 6) * 50;
      int y_pos = 600 + (i / 6) * 50;
      SDL_Rect dest = {x_pos, y_pos, 40, 40};
      SDL_RenderCopy(renderer, piece_textures[piece], NULL, &dest);
    }
  }

  if (is_promoting) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_Rect dim_overlay = {0, 0, BOARD_SIZE, BOARD_SIZE};
    SDL_RenderFillRect(renderer, &dim_overlay);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

    // Draw the Popup Box
    int box_x = 200, box_y = 350, box_w = 400, box_h = 100;
    SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
    SDL_Rect menu_rect = {box_x, box_y, box_w, box_h};
    SDL_RenderFillRect(renderer, &menu_rect);
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderDrawRect(renderer, &menu_rect);

    char pieces[] = {'q', 'r', 'b', 'n'};
    bool is_white = (turn(current.moves_played) == 0);

    for (int i = 0; i < 4; i++) {
      char p = is_white ? tolower(pieces[i]) : toupper(pieces[i]);
      SDL_Rect dest = {box_x + 10 + (i * 100), box_y + 10, 80, 80};

      if (i > 0) {
        SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
        SDL_RenderDrawLine(renderer, box_x + (i * 100), box_y + 10,
                           box_x + (i * 100), box_y + 90);
      }

      if (piece_textures.count(p)) {
        SDL_RenderCopy(renderer, piece_textures[p], NULL, &dest);
      }
    }
  }

  SDL_RenderPresent(renderer);
}

int init_gui() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0 ||
      !(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) || TTF_Init() == -1)
    return -1;
  window =
      SDL_CreateWindow("Chess Engine", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!window || !renderer)
    return -1;
  SDL_RenderSetLogicalSize(renderer, WINDOW_WIDTH, WINDOW_HEIGHT);
  global_font = TTF_OpenFont("assets/font.ttf", 24);
  load_textures();
  active_sidenote = get_random_sidenote();
  return 0;
}

void run_gui_loop() {
  running = true;
  SDL_Event event;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT)
        running = false;
      else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_F11) {
        is_fullscreen = !is_fullscreen;
        SDL_SetWindowFullscreen(
            window, is_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
      } else if (event.type == SDL_MOUSEBUTTONDOWN &&
                 event.button.button == SDL_BUTTON_LEFT) {
        handle_click(event.button.x, event.button.y);
      }
    }
    render_frame();
  }
}

void cleanup_gui() {
  for (auto &pair : piece_textures)
    SDL_DestroyTexture(pair.second);
  piece_textures.clear();
  if (global_font)
    TTF_CloseFont(global_font);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
}
