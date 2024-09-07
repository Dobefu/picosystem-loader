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

games game_chosen = games::NONE;

void init()
{
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
