#include "picosystem.hpp"

using namespace picosystem;

#ifndef _PICOSYSTEM_LOADER_H
#define _PICOSYSTEM_LOADER_H

enum states
{
  SPLASH,
  PLAYING,
};

enum games
{
  NONE,
  TETRIS,
};

struct game_t
{
  games id;
  const char *title;
  buffer_t *card = buffer(60, 80);
};

buffer_t *card = buffer(48, 48);

const char num_games = 6;
game_t games_available[num_games] = {
    {
        games::TETRIS,
        "Tetris",
    },
    {
        games::TETRIS,
        "Tetris 2",
    },
    {
        games::TETRIS,
        "Tetris 3",
    },
    {
        games::TETRIS,
        "Tetris 4",
    },
    {
        games::TETRIS,
        "Tetris 5",
    },
    {
        games::TETRIS,
        "Tetris 6",
    },
};

#endif
