#pragma once

#include <optional>
#include <ostream>
#include <vector>


enum Player {
  X,
  O
};

struct Move {
  int y;
  int x;
  Player p;

  Move(int y, int x, Player p) : y(y), x(x), p(p) {}
};

// immutable board class
class Board {
  // member variables
  private:
  // board state encoded in a minimal integer: 16 bits should be enough
  // welp, that took way too long so heres a bit array of 18.
  bool state[9][2];

  public:
  // copy etc constructors not needed because only property is int
  Board(); // 0 board
  Board(const Board &, Move); // board with move made

  std::optional<Player> get(int y, int x) const;
  bool full() const;
  bool isTerminal() const;
  std::optional<Player> winner() const;

  // statics
  public:
  // converts the index notation used in input/output to coordinates
  static std::pair<int, int> indexToCoords(int);
  // converts coordinates to index notation used in io
  static int coordsToIndex(int, int);
};

std::ostream &operator<<(std::ostream &, const Board &);
std::ostream &operator<<(std::ostream &, const Player &);
std::ostream &operator<<(std::ostream &, const std::optional<Player> &);

