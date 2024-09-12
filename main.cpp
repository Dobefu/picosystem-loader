#include "main.hpp"
#include "picosystem.hpp"
#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <queue>
#include <stack>

using namespace picosystem;

namespace Tetris
{
#include "games/picosystem-tetris/tetris.cpp"
}

char battery_level;

uint8_t volume = 100;

states state = states::SPLASH;
games game_chosen = games::NONE;
char game_selected = 0;

void init_logo(buffer_t *logo)
{
  target(logo);

  const uint8_t *s = _picosystem_logo;

  for (char y = 0; y < 50; y++)
  {
    for (char x = 0; x < 82; x += 8)
    {
      for (char bit = 0; bit < 8; bit++)
      {
        if (*s & (0b10000000 >> bit))
        {
          pen(15, 15, 15);
          pixel(x + bit, y);
        }
        else
        {
          pen(0, 0, 0);
          pixel(x + bit, y);
        }
      }

      s++;
    }
  }

  target();
}

void splash()
{
  pen(0, 0, 0);
  clear();

  buffer_t *logo = buffer(82, 50);
  init_logo(logo);

  for (char i = 0; i < 145; i++)
  {
    pen(0, 0, 0);

    if (-51 + i >= 0)
    {
      frect(79, -51 + i, 82, 1);
    }

    blit(logo, 0, 0, 82, 50, 79, -50 + i, 82, 50);

    _flip();

    sleep(15);
  }

  sleep(100);
  voice_t voice_startup = voice(100, 50, 50, 400);
  play(voice_startup, 4000, 40, volume);
  sleep(100);
  play(voice_startup, 3000, 40, volume);

  free(logo);
}

void init_games()
{
  target(card);
  blend(COPY);

  for (char j = 0; j <= 8; j++)
  {
    pen(0, 0, 0, j);

    if (j == 8)
    {
      pen(11, 12, 15);
    }

    fcircle(16, 16, 16 - j);
    fcircle(31, 16, 16 - j);
    fcircle(16, 31, 16 - j);
    fcircle(31, 31, 16 - j);

    hline(16, j, 16);
    hline(16, 47 - j, 16);
    vline(j, 16, 16);
    vline(47 - j, 16, 16);
  }

  pen(11, 12, 15);
  frect(9, 16, 30, 16);
  frect(16, 9, 16, 30);

  for (char i = 0; i < num_games; i++)
  {
    target(games_available[i].card);

    pen(0, 0, 0);
    text(games_available[i].title, 41 - (((float)strlen(games_available[i].title) / 2) * 8), 61);
    pen(15, 15, 15);
    text(games_available[i].title, 40 - (((float)strlen(games_available[i].title) / 2) * 8), 60);
  }

  target();
}

void draw_card(game_t game, int8_t x, int8_t y, float scale)
{
  // Corners.
  blit(card, 0, 0, 16, 16, (60 + x) * scale, (40 + y) * scale, 16 * scale, 16 * scale);
  blit(card, 32, 0, 16, 16, (164 + x) * scale, (40 + y) * scale, 16 * scale, 16 * scale);
  blit(card, 0, 32, 16, 16, (60 + x) * scale, (184 + y) * scale, 16 * scale, 16 * scale);
  blit(card, 32, 32, 16, 16, (164 + x) * scale, (184 + y) * scale, 16 * scale, 16 * scale);

  // Sides.
  blit(card, 16, 0, 16, 16, (76 + x) * scale, (40 + y) * scale, 88 * scale, 16 * scale);
  blit(card, 16, 32, 16, 16, (76 + x) * scale, (184 + y) * scale, 88 * scale, 16 * scale);
  blit(card, 0, 16, 16, 16, (60 + x) * scale, (56 + y) * scale, 16 * scale, 128 * scale);
  blit(card, 32, 16, 16, 16, (164 + x) * scale, (56 + y) * scale, 16 * scale, 128 * scale);

  // Center.
  blit(card, 16, 16, 16, 16, (76 + x) * scale, (56 + y) * scale, 88 * scale, 128 * scale);

  blit(game.card, 0, 0, 60, 80, (60 + x) * scale, (40 + y) * scale, 120 * scale, 160 * scale);
}

void init()
{
  splash();
  sleep(1000);

  init_games();
}

void update(uint32_t tick)
{
  battery_level = battery();
  led(100 - battery_level, battery_level, 0);

  switch (game_chosen)
  {
  case games::TETRIS:
    Tetris::update(tick);
    return;
  }

  if (pressed(LEFT) || pressed(UP))
  {
    game_selected--;

    if (game_selected > num_games - 1)
    {
      game_selected = num_games - 1;
    }
  }

  if (pressed(RIGHT) || pressed(DOWN))
  {
    game_selected++;

    if (game_selected > num_games - 1)
    {
      game_selected = 0;
    }
  }

  if (pressed(A))
  {
    game_chosen = games_available[game_selected].id;

    switch (game_chosen)
    {
    case games::TETRIS:
      Tetris::init();
      return;
    }
  }
}

void draw(uint32_t tick)
{
  switch (game_chosen)
  {
  case games::TETRIS:
    Tetris::draw(tick);
    return;
  }

  pen(2, 8, 2);
  clear();

  buffer_t *select_text = buffer(78, 8);
  target(select_text);

  pen(0, 0, 0);
  text("SELECT A GAME", 1, 1);
  pen(15, 15, 15);
  text("SELECT A GAME", 0, 0);

  target();
  blit(select_text, 0, 0, 78, 8, 42, 16, 156, 16);

  draw_card(games_available[(char)(game_selected - 1) > num_games - 1 ? num_games - 1 : game_selected - 1], -48, 40, .75);
  draw_card(games_available[(char)(game_selected + 1) > num_games - 1 ? 0 : game_selected + 1], 124, 40, .75);
  draw_card(games_available[game_selected], 0, 0, 1);
}
