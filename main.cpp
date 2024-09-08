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

  for (char j = 0; j <= 8; j++)
  {
    pen(2 + j, 8, 2 + j);

    if (j == 8)
    {
      pen(11, 12, 15);
    }

    fcircle(16, 16, 16 - j);
    fcircle(104, 16, 16 - j);
    fcircle(16, 144, 16 - j);
    fcircle(104, 144, 16 - j);

    hline(16, j, 88);
    hline(16, 160 - j, 88);
    vline(j, 16, 128);
    vline(120 - j, 16, 128);
  }

  pen(11, 12, 15);
  frect(9, 16, 103, 128);
  frect(16, 9, 88, 143);

  for (char i = 0; i < num_games; i++)
  {
    target(games_available[i].card);

    pen(15, 15, 15);
    text(games_available[i].title, 40 - (((float)strlen(games_available[i].title) / 2) * 8), 60);
  }

  target();
}

void init()
{
  splash();
  sleep(1000);

  pen(2, 8, 2);
  clear();

  init_games();

  blit(card, 0, 0, 120, 160, 60, 40, 120, 160);
  blit(games_available[0].card, 0, 0, 60, 80, 60, 40, 120, 160);
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

  if (pressed(A))
  {
    game_chosen = games::TETRIS;
    Tetris::init();
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
}
