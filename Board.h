// ============================ Board.h ============================

#pragma once
#include "Pieces.h"

using namespace std;

// ---- Promotion choice ----
enum PromoPiece { PROMO_Q = 1, PROMO_R, PROMO_B, PROMO_N };

class Board
{
private:
	Piece* grid[ROWS][COLS];   // 8x8 board

	int whiteScore;
	int blackScore;

	int enPassantRow;
	int enPassantCol;

	// Private helpers
	bool isSquareAttacked(int row, int col, Color byColor);

public:
	Board();
	~Board();

	// Move execution
	bool movePiece(int cRow, int cCol, int nRow, int nCol,
		Color currentPlayer, PromoPiece promo = PROMO_Q);

	// Game state queries
	bool isInCheck(Color c);
	bool isCheckmate(Color c);
	bool isStalemate(Color c);
	bool onlyKingsLeft();

	// Valid move highlighting
	void getValidMoves(int row, int col, bool validMoves[ROWS][COLS]);

	// Display
	void displayBoard(int cursorRow, int cursorCol,
		int selRow, int selCol,
		bool validMoves[ROWS][COLS]) const;

	// Accessors
	Piece* getPiece(int row, int col) const;
	int    getWhiteScore() const;
	int    getBlackScore() const;
};
