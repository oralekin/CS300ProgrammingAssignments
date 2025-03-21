#include "Board.h"

using namespace std;

// simple recursive integer exponentiation
// TODO: make this use the squaring algorithm
int power(int base, unsigned int exponent) {
  if (exponent == 0) return 1;
  else return 3 * power(base, exponent - 1);
}

Board::Board() {}

Board::Board(const Board &prev, Move m) {

  // copy previous
  for (int i = 0; i < 9; i++) for (int j = 0; j < 2; j++)
    state[i][j] = prev.state[i][j];


  if (get(m.y, m.x).has_value()) throw "invalid move!";
  // we know that this spot is empty.
  state[3 * m.y + m.x][0] = true;
  state[3 * m.y + m.x][1] = m.p;
}

optional<Player> Board::get(int y, int x) const {
  // int idx = (y * 3) + x;
  // int code = (state / power(3, idx)) % 3;
  int code = state[3 * y + x][0] * (state[3 * y + x][1] + 1);

  switch (code) {
  case 0:
    return {};
  case 1:
    return Player::X;
  case 2:
    return Player::O;
  default:
    throw "some kinda runtime error!?";
  }
}

bool Board::full() const {
  return
    get(0, 0) && // do corners first because theyre likely to be untaken
    get(0, 2) &&
    get(2, 0) &&
    get(2, 2) &&
    get(0, 1) &&
    get(1, 0) &&
    get(1, 2) &&
    get(2, 1) &&
    get(1, 1); // do middle last because its likely to be taken
}

bool Board::isTerminal() const {
  return winner() || full();
}

optional<Player> Board::winner() const {
  // wins containing middle square:
  optional<Player> winner = get(1, 1);
  if (winner.has_value()) {
    // diagonals
    if (get(0, 0) == winner && winner == get(2, 2)) return winner;
    if (get(2, 0) == winner && winner == get(0, 2)) return winner;

    // vertical, horizontal
    if (get(0, 1) == winner && winner == get(2, 1)) return winner;
    if (get(1, 0) == winner && winner == get(1, 2)) return winner;
  }

  // try top left corner wins:
  winner = get(0, 0);
  if (winner.has_value()) {
    // vertical
    if (get(1, 0) == winner && winner == get(2, 0)) return winner;
    // horizontal 
    if (get(0, 1) == winner && winner == get(0, 2)) return winner;
  }

  // bottom right
  winner = get(2, 2);
  if (winner.has_value()) {
    // vertical
    if (get(1, 2) == winner && winner == get(0, 2)) return winner;
    // horizontal 
    if (get(2, 1) == winner && winner == get(2, 0)) return winner;
  }

  return {};
}

std::pair<int, int> Board::indexToCoords(int idx) {
  return { (idx - 1) / 3, (idx - 1) % 3 };
}

int Board::coordsToIndex(int y, int x) {
  return (y * 3) + x + 1;
}

ostream &operator<<(ostream &os, const Board &b) {
  const string sep_H = "---+---+---";

  os << " " << b.get(0, 0) << " | " << b.get(0, 1) << " | " << b.get(0, 2) << " ";
  os << endl << sep_H << endl;
  os << " " << b.get(1, 0) << " | " << b.get(1, 1) << " | " << b.get(1, 2) << " ";
  os << endl << sep_H << endl;
  os << " " << b.get(2, 0) << " | " << b.get(2, 1) << " | " << b.get(2, 2) << " ";

  return os;
}

ostream &operator<<(ostream &os, const Player &p) {
  switch (p) {
  case Player::X:
    os << "X";
    break;
  case Player::O:
    os << "O";
    break;
  }

  return os;
}

ostream &operator<<(ostream &os, const optional<Player> &p) {
  if (!p.has_value()) os << " ";
  else os << *p;

  return os;
}