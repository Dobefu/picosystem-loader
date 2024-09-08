#include "main.hpp"
#include "picosystem.hpp"
#include <array>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <stack>

using namespace picosystem;

namespace Tetris
{
#include "games/picosystem-tetris/tetris.cpp"
}

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

void init()
{
  splash();
  sleep(1000);

  pen(2, 8, 2);
  clear();
}

void update(uint32_t tick)
{
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
