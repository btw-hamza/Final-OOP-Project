// ============================ Pieces.cpp ============================

#include "Pieces.h"
#include <cmath>
using namespace std;

// ======================== Base Piece ========================

Piece::Piece(Color c, char s)
{
	color = c;
	symbol = s;
	moved = false;
}

Color Piece::getColor()    const { return color; }
char  Piece::getSymbol()   const { return symbol; }
bool  Piece::hasMoved()    const { return moved; }
void  Piece::setMoved(bool m) { moved = m; }
Piece::~Piece() {}

// ======================== Pawn ========================
// Moves forward 1 (or 2 from start), captures diagonally,
// supports en passant capture.

Pawn::Pawn(Color c) : Piece(c, c == WHITE ? 'P' : 'p') {}

bool Pawn::isValidMove(int cRow, int cCol, int nRow, int nCol,
	Piece* board[ROWS][COLS], int enPassantRow, int enPassantCol)
{
	if (cRow == nRow && cCol == nCol) return false;

	int dir = (color == WHITE) ? -1 : 1;  // WHITE moves up, BLACK down

	// One step forward into empty square
	if (nCol == cCol &&
		nRow == cRow + dir &&
		board[nRow][nCol] == nullptr)
		return true;

	// Two steps forward from starting rank (both squares must be empty)
	if (!moved &&
		nCol == cCol &&
		nRow == cRow + 2 * dir &&
		board[cRow + dir][cCol] == nullptr &&
		board[nRow][nCol] == nullptr)
		return true;

	// Diagonal capture of enemy piece
	if (abs(nCol - cCol) == 1 &&
		nRow == cRow + dir &&
		board[nRow][nCol] != nullptr &&
		board[nRow][nCol]->getColor() != color)
		return true;

	// En passant capture
	// The target square must be diagonally forward and match the ep square
	if (abs(nCol - cCol) == 1 &&
		nRow == cRow + dir &&
		nRow == enPassantRow &&
		nCol == enPassantCol)
		return true;

	return false;
}

// ======================== Rook ========================
// Moves any number of squares along rank or file; path must be clear.

Rook::Rook(Color c) : Piece(c, c == WHITE ? 'R' : 'r') {}

bool Rook::isValidMove(int cRow, int cCol, int nRow, int nCol,
	Piece* board[ROWS][COLS], int enPassantRow, int enPassantCol)
{
	if (cRow == nRow && cCol == nCol) return false;

	if (board[nRow][nCol] != nullptr &&
		board[nRow][nCol]->getColor() == color)
		return false;

	if (cRow != nRow && cCol != nCol) return false;

	if (cRow == nRow)
	{
		int step = (nCol > cCol) ? 1 : -1;
		for (int i = cCol + step; i != nCol; i += step)
			if (board[cRow][i] != nullptr) return false;
	}
	else
	{
		int step = (nRow > cRow) ? 1 : -1;
		for (int i = cRow + step; i != nRow; i += step)
			if (board[i][cCol] != nullptr) return false;
	}

	return true;
}

// ======================== Knight ========================
// L-shaped jump; ignores pieces in between.

Knight::Knight(Color c) : Piece(c, c == WHITE ? 'N' : 'n') {}

bool Knight::isValidMove(int cRow, int cCol, int nRow, int nCol,
	Piece* board[ROWS][COLS], int enPassantRow, int enPassantCol)
{
	if (cRow == nRow && cCol == nCol) return false;

	if (board[nRow][nCol] != nullptr &&
		board[nRow][nCol]->getColor() == color)
		return false;

	int r = abs(nRow - cRow);
	int c = abs(nCol - cCol);

	return (r == 2 && c == 1) || (r == 1 && c == 2);
}

// ======================== Bishop ========================
// Diagonal movement; path must be clear.

Bishop::Bishop(Color c) : Piece(c, c == WHITE ? 'B' : 'b') {}

bool Bishop::isValidMove(int cRow, int cCol, int nRow, int nCol,
	Piece* board[ROWS][COLS], int enPassantRow, int enPassantCol)
{
	if (cRow == nRow && cCol == nCol) return false;

	if (board[nRow][nCol] != nullptr &&
		board[nRow][nCol]->getColor() == color)
		return false;

	if (abs(nRow - cRow) != abs(nCol - cCol)) return false;

	int rowStep = (nRow > cRow) ? 1 : -1;
	int colStep = (nCol > cCol) ? 1 : -1;

	int r = cRow + rowStep;
	int c = cCol + colStep;

	while (r != nRow && c != nCol)
	{
		if (board[r][c] != nullptr) return false;
		r += rowStep;
		c += colStep;
	}

	return true;
}

// ======================== Queen ========================
// Combines rook + bishop movement (reuses their path helpers).

Queen::Queen(Color c) : Piece(c, c == WHITE ? 'Q' : 'q') {}

bool Queen::isValidMove(int cRow, int cCol, int nRow, int nCol,
	Piece* board[ROWS][COLS], int enPassantRow, int enPassantCol)
{
	Rook   r(color);
	Bishop b(color);

	return r.isValidMove(cRow, cCol, nRow, nCol, board, enPassantRow, enPassantCol) ||
		b.isValidMove(cRow, cCol, nRow, nCol, board, enPassantRow, enPassantCol);
}

// ======================== King ========================
// One square any direction; also signals castling intent (2 squares).

King::King(Color c) : Piece(c, c == WHITE ? 'K' : 'k') {}

bool King::isValidMove(int cRow, int cCol, int nRow, int nCol,
	Piece* board[ROWS][COLS], int enPassantRow, int enPassantCol)
{
	if (cRow == nRow && cCol == nCol) return false;

	if (board[nRow][nCol] != nullptr &&
		board[nRow][nCol]->getColor() == color)
		return false;

	int r = abs(nRow - cRow);
	int c = abs(nCol - cCol);

	// Normal one-square move
	if (r <= 1 && c <= 1) return true;

	// Castling intent (full legality checked in Board::movePiece)
	if (!moved && r == 0 && c == 2) return true;

	return false;
}
