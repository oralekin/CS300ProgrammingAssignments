// Ekin Oral oralekin@sabanciuniv.edu
// 29421

#include <iostream>
#include <ostream>
#include <vector>


// i wrote all of this for the latest g++, but coderunner seems to have an older
// version, so polyfill needed
// polyfill macros from https://stackoverflow.com/a/50348105
#ifdef __cpp_lib_experimental_optional
#include <experimental/optional>
namespace cxx17 {
  using std::experimental::optional;
}
#elif __cpp_lib_optional
#include <optional>
namespace cxx17 {
  using std::optional;
}
#else
#include <boost/optional.hpp>
namespace cxx17 {
  using boost::optional;
}
#endif




// Board.h
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

  cxx17::optional<Player> get(int y, int x) const;
  bool full() const;
  bool isTerminal() const;
  cxx17::optional<Player> winner() const;

  // statics
  public:
  // converts the index notation used in input/output to coordinates
  static std::pair<int, int> indexToCoords(int);
  // converts coordinates to index notation used in io
  static int coordsToIndex(int, int);
};

std::ostream &operator<<(std::ostream &, const Board &);
std::ostream &operator<<(std::ostream &, const Player &);
std::ostream &operator<<(std::ostream &, const cxx17::optional<Player> &);


// Board.cpp
using namespace std;

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

cxx17::optional<Player> Board::get(int y, int x) const {
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

cxx17::optional<Player> Board::winner() const {
  // wins containing middle square:
  cxx17::optional<Player> winner = get(1, 1);
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

ostream &operator<<(ostream &os, const cxx17::optional<Player> &p) {
  if (!p.has_value()) os << " ";
  else os << *p;

  return os;
}

// DecisionTree.cpp

#include <climits>

using namespace std;

// in >c++17 this czn be constexpr 
Player inverse(const Player &p) {
  if (p == Player::X) return Player::O;
  else if (p == Player::O) return Player::X;
  else throw "unknown error!";
}

class DecisionTree {
  struct Node {
    Board board;
    Move move;

    int value;
    Node *children[9];

    Node(Board b, Move m) :board(b), move(m), value(0), children() {}

    // generates every subtree recursively.
    void generateAll() {
      for (int y = 0; y < 3; y++)
        for (int x = 0; x < 3; x++)
          if (!board.get(y, x)) {
            children[3 * y + x] = new Node(
              Board(board, Move(y, x, inverse(move.p))),
              Move(y, x, inverse(move.p))
            );
            children[3 * y + x]->generateAll();
          }
    }

    ~Node() {
      // delete all children if this node is deleted
      for (int i = 0; i < 9; i++) {
        Node *&current = children[i];
        if (current) delete current;
        current = nullptr;
      }
    }


    // directly implementing the pseudocode
    int evaluate_helper(int &visits, int a, int b) const {
      // this node was visited! increment visits counter
      visits++;

      if (board.isTerminal()) {
        auto w = board.winner();
        if (w) switch (*w) {
        case Player::X:
          return 10;
        case Player::O:
          return -10;
        default: // shouldnt happen
          return 0;
        } else return 0;
      } else {
        if (move.p == inverse(Player::X)) { // next move is maximizing player
          int maxEval = INT_MIN;
          for (int i = 0; i < 9; i++)
            if (children[i]) {
              int eval = children[i]->evaluate(visits, a, b);
              maxEval = max(maxEval, eval);
              a = max(a, eval);
              if (b <= a) break;
            }
          return maxEval;
        } else { // next move is minimizing
          int minEval = INT_MAX;
          for (int i = 0; i < 9; i++)
            if (children[i]) {
              int eval = children[i]->evaluate(visits, a, b);
              minEval = min(minEval, eval);
              b = min(b, eval);
              if (b <= a) break;
            }
          return minEval;
        }
      }
    }

    int evaluate(int &visits, int a, int b) {
      value = evaluate_helper(visits, a, b);
      return value;
    }
  };

  Node *root;

  public:
  DecisionTree(Board b, Move m) : root(new Node(b, m)) {}
  // use invalid move as placeholder
  // last move is inverse of who will play next
  DecisionTree(Board b, Player toPlay) : DecisionTree(b, Move(3, 3, inverse(toPlay))) {}
  // empty board initializer
  // use player O because next=first move is X
  DecisionTree() : DecisionTree(Board(), Player::X) {}


  pair<Move, int> best_minmax() const {
    // make sure tree is fully generated.
    root->generateAll();

    Node *chosen = nullptr;

    // start from one because I keep getting off by one errors
    // might be because the reference implementation considers the root "visited"
    // I don't think that's correct but I'll start with 1 to match expected
    int visits = 1;

    // loop all children of root, pick the first that gives best outcome, 
    for (int i = 0; i < 9; i++) if (root->children[i]) {
      int a = INT_MIN;
      int b = INT_MAX;
      root->children[i]->evaluate(visits, a, b);
      // previous move was O = im maximizing
      if (root->move.p == Player::O) {
        if (!chosen || (// short circuit if not chosen one yet
          (chosen->value < root->children[i]->value)
          )) chosen = root->children[i];
      } else { // minimizing
        if (!chosen || (// short circuit if not chosen one yet
          (chosen->value > root->children[i]->value)
          )) chosen = root->children[i];
      }
    }

    return { chosen->move, visits };
  }

  ~DecisionTree() {
    delete root;
  }

};

// main.cpp

using namespace std;


const string MOVES_BOARD =
" 1 | 2 | 3 \n"
"---+---+---\n"
" 4 | 5 | 6 \n"
"---+---+---\n"
" 7 | 8 | 9 \n";

int getPlayerMove(const Board &position) {

  bool valid = false;
  int response;
  while (!valid) {
    cout << "Your move (1-9): ";
    cin >> response;

    // innocent until guilty
    valid = true;
    if (response > 9 || response < 1) {
      cout << "Selection out of range. Try again." << endl;
      valid = false;
      continue;
    }

    pair<int, int> ret = Board::indexToCoords(response);
    if (position.get(ret.first, ret.second).has_value()) {
      cout << "That position is already taken. Try again." << endl;
      valid = false;
    }
  }

  return response;
}

void announceWinner(cxx17::optional<Player> winner, Player pc) {
  if (!winner.has_value()) cout << "It's a draw!" << endl;
  else if (winner.value() == pc) cout << "Congratulations! You win!" << endl;
  else cout << "The AI wins!" << endl;
  return;
}

int main() {
  cout <<
    "Do you want to play as X or O? (X goes first): ";
  Player pc = Player::X;
  Player npc = Player::O;

  string player_choice;
  cin >> player_choice;
  if (player_choice == "O") swap(pc, npc);

  cout << "Welcome to Tic Tac Toe!" << endl
    << "You are playing as " << pc << "." << endl
    << "The AI is playing as " << npc << "." << endl;

  Board current;
  bool aiTurn = npc == Player::X; // ai starts if it's turn

  // show prompt for cell indices
  cout << "Enter a number from 1-9 to make your move:" << endl << endl;
  cout << MOVES_BOARD << endl;

  // do game until the program is done.
  while (true) {
    if (aiTurn) { // ai move

      auto tree = DecisionTree(current, npc);

      cout << "AI is making a move..." << endl;
      auto chosen = tree.best_minmax();
      cout << "AI chose position "
        << Board::coordsToIndex(chosen.first.y, chosen.first.x)
        << "." << endl
        << "Nodes explored: " << chosen.second << endl;

      current = Board(move(current), chosen.first);
    } else { // player move
      // get player move.
      auto player_input = Board::indexToCoords(getPlayerMove(current));

      // update current board
      Move player_move(player_input.first, player_input.second, pc);
      current = Board(move(current), player_move);

    }

    cout << endl << current << endl << endl;
    auto w = current.winner();
    if (w || current.full()) {
      announceWinner(w, pc);
      return 0;
    }

    // next turn other player
    aiTurn = !aiTurn;
  }

  return 0;
}