#include <iostream>
#include "Board.h"
#include "DecisionTree.cpp"


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

void announceWinner(optional<Player> winner, Player pc) {
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
      cout << "AI chose position " << Board::coordsToIndex(chosen.first.y, chosen.first.x) << "." << endl
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