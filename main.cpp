// ============================ Main.cpp ============================

#include "Board.h"
#include <conio.h>
#include <string>
using namespace std;

// ---- Key codes ----
const int KEY_SPECIAL = 224;
const int KEY_UP = 72;
const int KEY_DOWN = 80;
const int KEY_LEFT = 75;
const int KEY_RIGHT = 77;
const int KEY_ENTER = 13;
const int KEY_ESC = 27;

// ---- Promotion prompt ----
PromoPiece askPromotion()
{
	cout << "\n  PAWN PROMOTION - Choose:";
	cout << "  1=Queen  2=Rook  3=Bishop  4=Knight : ";
	cout.flush();
	while (true)
	{
		int k = _getch();
		if (k == '1') return PROMO_Q;
		if (k == '2') return PROMO_R;
		if (k == '3') return PROMO_B;
		if (k == '4') return PROMO_N;
	}
}

// ---- Check if this move will trigger promotion ----
bool isPawnPromotion(Board& b, int fr, int fc, int tr)
{
	Piece* p = b.getPiece(fr, fc);
	if (p == nullptr) return false;
	if (toupper(p->getSymbol()) != 'P') return false;
	return (tr == 0 || tr == 7);
}

// ---- Clear selection ----
void clearSel(int& sr, int& sc, bool& sel, bool vm[ROWS][COLS])
{
	sel = false;
	sr = sc = -1;
	for (int r = 0; r < ROWS; r++)
		for (int c = 0; c < COLS; c++)
			vm[r][c] = false;
}

// ============================================================
// main
// ============================================================
int main()
{
	// ---- Player names ----
	string player1, player2;

	cout << "=====================================" << endl;
	cout << "       WELCOME TO CHESS MASTER       " << endl;
	cout << "=====================================" << endl;
	cout << "Enter Player 1 Name (White): ";
	getline(cin, player1);
	cout << "Enter Player 2 Name (Black): ";
	getline(cin, player2);

	Board b;
	Color currentPlayer = WHITE;

	int  cursorRow = 7, cursorCol = 0;
	int  selRow = -1, selCol = -1;
	bool pieceSelected = false;

	bool validMoves[ROWS][COLS] = { false };

	// Clear screen once, hide cursor
	cout << "\033[2J\033[H";
	cout << "\033[?25l";
	cout.flush();

	// ============================================================
	// GAME LOOP
	// ============================================================
	while (true)
	{
		// ---- Redraw from top (no clear = no flicker) ----
		cout << "\033[H";

		// Header
		cout << "  +-------------------------------------------------+\n";
		cout << "  |              C H E S S  M A S T E R             |\n";
		cout << "  |  Arrows: move cursor    Enter: select / move    |\n";
		cout << "  |  ESC: deselect          R: resign               |\n";
		cout << "  +-------------------------------------------------+\n\n";

		// Scores
		int diff = b.getWhiteScore() - b.getBlackScore();
		cout << "  Score - White: " << b.getWhiteScore()
			<< "   Black: " << b.getBlackScore();
		if (diff > 0) cout << "   (" << player1 << " leads +" << diff << ")";
		else if (diff < 0) cout << "   (" << player2 << " leads +" << -diff << ")";
		else               cout << "   (even)   ";
		cout << "     \n\n";

		// Turn indicator
		string& curName = (currentPlayer == WHITE) ? player1 : player2;
		cout << "  " << curName
			<< (currentPlayer == WHITE ? " (White)" : " (Black)")
			<< "'s Turn                         \n";

		// Check warning
		if (b.isInCheck(currentPlayer))
			cout << "  \033[1;31m>>> " << curName
			<< " is in CHECK! <<<\033[0m          \n";
		else
			cout << "                                             \n";

		// Board
		b.displayBoard(cursorRow, cursorCol, selRow, selCol, validMoves);

		cout.flush();

		// ---- End conditions ----
		if (b.isCheckmate(currentPlayer))
		{
			system("cls");
			b.displayBoard(-1, -1, -1, -1, validMoves);
			cout << "\n  " << curName << " is in CHECKMATE!\n";
			string& winner = (currentPlayer == WHITE) ? player2 : player1;
			cout << "  " << winner << " WINS!\n\n";
			break;
		}
		if (b.isStalemate(currentPlayer))
		{
			system("cls");
			b.displayBoard(-1, -1, -1, -1, validMoves);
			cout << "\n  STALEMATE!  It's a Draw!\n\n";
			break;
		}
		if (b.onlyKingsLeft())
		{
			system("cls");
			b.displayBoard(-1, -1, -1, -1, validMoves);
			cout << "\n  Only Kings remain.  DRAW!\n\n";
			break;
		}

		// ---- Input (blocking - simple and clean) ----
		int key = _getch();

		// Arrow keys
		if (key == KEY_SPECIAL)
		{
			key = _getch();
			if (key == KEY_UP && cursorRow > 0) cursorRow--;
			if (key == KEY_DOWN && cursorRow < 7) cursorRow++;
			if (key == KEY_LEFT && cursorCol > 0) cursorCol--;
			if (key == KEY_RIGHT && cursorCol < 7) cursorCol++;
		}

		// Enter - select or move
		else if (key == KEY_ENTER)
		{
			if (!pieceSelected)
			{
				Piece* p = b.getPiece(cursorRow, cursorCol);
				if (p != nullptr && p->getColor() == currentPlayer)
				{
					selRow = cursorRow;
					selCol = cursorCol;
					pieceSelected = true;
					b.getValidMoves(selRow, selCol, validMoves);
				}
			}
			else
			{
				if (validMoves[cursorRow][cursorCol])
				{
					PromoPiece promo = PROMO_Q;
					if (isPawnPromotion(b, selRow, selCol, cursorRow))
						promo = askPromotion();

					if (b.movePiece(selRow, selCol,
						cursorRow, cursorCol,
						currentPlayer, promo))
					{
						currentPlayer = (currentPlayer == WHITE)
							? BLACK : WHITE;
					}
				}
				clearSel(selRow, selCol, pieceSelected, validMoves);
			}
		}

		// ESC - deselect
		else if (key == KEY_ESC)
		{
			clearSel(selRow, selCol, pieceSelected, validMoves);
		}

		// R - resign
		else if (toupper(key) == 'R')
		{
			system("cls");
			cout << "\n  " << curName << " Resigned!\n";
			string& winner = (currentPlayer == WHITE) ? player2 : player1;
			cout << "  " << winner << " WINS!\n\n";
			break;
		}
	}

	// Restore cursor
	cout << "\033[?25h\033[0m";
	cout << "  Press any key to exit...\n";
	_getch();

	return 0;
}
