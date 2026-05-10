// ============================ Board.cpp ============================

#include "Board.h"
#include <iostream>
#include <cctype>
using namespace std;

// ======================== Constructor ========================

Board::Board()
{
	whiteScore = 0;
	blackScore = 0;
	enPassantRow = -1;
	enPassantCol = -1;

	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++)
			grid[i][j] = nullptr;

	for (int i = 0; i < COLS; i++)
	{
		grid[1][i] = new Pawn(BLACK);
		grid[6][i] = new Pawn(WHITE);
	}

	grid[0][0] = new Rook(BLACK);
	grid[0][1] = new Knight(BLACK);
	grid[0][2] = new Bishop(BLACK);
	grid[0][3] = new Queen(BLACK);
	grid[0][4] = new King(BLACK);
	grid[0][5] = new Bishop(BLACK);
	grid[0][6] = new Knight(BLACK);
	grid[0][7] = new Rook(BLACK);

	grid[7][0] = new Rook(WHITE);
	grid[7][1] = new Knight(WHITE);
	grid[7][2] = new Bishop(WHITE);
	grid[7][3] = new Queen(WHITE);
	grid[7][4] = new King(WHITE);
	grid[7][5] = new Bishop(WHITE);
	grid[7][6] = new Knight(WHITE);
	grid[7][7] = new Rook(WHITE);
}

// ======================== Destructor ========================

Board::~Board()
{
	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++)
		{
			delete grid[i][j];
			grid[i][j] = nullptr;
		}
}

// ======================== Accessors ========================

Piece* Board::getPiece(int row, int col) const { return grid[row][col]; }
int    Board::getWhiteScore()            const { return whiteScore; }
int    Board::getBlackScore()            const { return blackScore; }

// ======================== pieceValue (local helper) ========================

static int pieceValue(char sym)
{
	switch (toupper(sym))
	{
	case 'P': return 1;
	case 'N': return 3;
	case 'B': return 3;
	case 'R': return 5;
	case 'Q': return 9;
	default:  return 0;
	}
}

// ======================== isSquareAttacked ========================

bool Board::isSquareAttacked(int row, int col, Color byColor)
{
	for (int r = 0; r < ROWS; r++)
		for (int c = 0; c < COLS; c++)
		{
			Piece* p = grid[r][c];
			if (p == nullptr || p->getColor() != byColor) continue;
			if (p->isValidMove(r, c, row, col, grid,
				enPassantRow, enPassantCol))
				return true;
		}
	return false;
}

// ======================== isInCheck ========================

bool Board::isInCheck(Color c)
{
	int kr = -1, kc = -1;

	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++)
			if (grid[i][j] != nullptr &&
				grid[i][j]->getColor() == c &&
				toupper(grid[i][j]->getSymbol()) == 'K')
			{
				kr = i; kc = j;
			}

	if (kr == -1) return false;

	Color opp = (c == WHITE) ? BLACK : WHITE;
	return isSquareAttacked(kr, kc, opp);
}

// ======================== movePiece ========================

bool Board::movePiece(int cRow, int cCol, int nRow, int nCol,
	Color currentPlayer, PromoPiece promo)
{
	if (grid[cRow][cCol] == nullptr)                         return false;
	if (grid[cRow][cCol]->getColor() != currentPlayer)       return false;
	if (!grid[cRow][cCol]->isValidMove(cRow, cCol, nRow, nCol,
		grid, enPassantRow, enPassantCol))                   return false;
	if (grid[nRow][nCol] != nullptr &&
		grid[nRow][nCol]->getColor() == currentPlayer)       return false;

	Color opp = (currentPlayer == WHITE) ? BLACK : WHITE;

	// ---- Castling ----
	bool isCastling = (toupper(grid[cRow][cCol]->getSymbol()) == 'K' &&
		abs(nCol - cCol) == 2);

	if (isCastling)
	{
		bool kingSide = (nCol > cCol);
		int  rookCol = kingSide ? 7 : 0;
		int  rookDest = kingSide ? 5 : 3;
		int  step = kingSide ? 1 : -1;

		if (grid[cRow][rookCol] == nullptr ||
			toupper(grid[cRow][rookCol]->getSymbol()) != 'R' ||
			grid[cRow][rookCol]->hasMoved())
			return false;

		for (int c = cCol + step; c != rookCol; c += step)
			if (grid[cRow][c] != nullptr) return false;

		if (isInCheck(currentPlayer)) return false;

		if (isSquareAttacked(cRow, cCol + step, opp)) return false;

		grid[cRow][rookDest] = grid[cRow][rookCol];
		grid[cRow][rookCol] = nullptr;
		grid[cRow][rookDest]->setMoved(true);
	}

	// ---- En passant capture ----
	bool isEnPassant = (toupper(grid[cRow][cCol]->getSymbol()) == 'P' &&
		nRow == enPassantRow && nCol == enPassantCol &&
		abs(nCol - cCol) == 1);

	Piece* epCaptured = nullptr;
	if (isEnPassant)
	{
		epCaptured = grid[cRow][nCol];
		grid[cRow][nCol] = nullptr;
	}

	// ---- Execute move (captured piece saved, not deleted yet) ----
	Piece* captured = grid[nRow][nCol];
	bool   hadMoved = grid[cRow][cCol]->hasMoved();

	grid[nRow][nCol] = grid[cRow][cCol];
	grid[cRow][cCol] = nullptr;
	grid[nRow][nCol]->setMoved(true);

	// ---- Update en passant target ----
	int prevEpRow = enPassantRow;
	int prevEpCol = enPassantCol;
	enPassantRow = -1;
	enPassantCol = -1;

	if (toupper(grid[nRow][nCol]->getSymbol()) == 'P' &&
		abs(nRow - cRow) == 2)
	{
		enPassantRow = (cRow + nRow) / 2;
		enPassantCol = nCol;
	}

	// ---- Self-check test: rollback if illegal ----
	if (isInCheck(currentPlayer))
	{
		grid[cRow][cCol] = grid[nRow][nCol];
		grid[nRow][nCol] = captured;
		grid[cRow][cCol]->setMoved(hadMoved);

		if (isEnPassant && epCaptured != nullptr)
			grid[cRow][nCol] = epCaptured;

		if (isCastling)
		{
			bool kingSide = (nCol > cCol);
			int  rookDest = kingSide ? 5 : 3;
			int  rookOrig = kingSide ? 7 : 0;
			grid[cRow][rookOrig] = grid[cRow][rookDest];
			grid[cRow][rookDest] = nullptr;
			grid[cRow][rookOrig]->setMoved(false);
		}

		enPassantRow = prevEpRow;
		enPassantCol = prevEpCol;
		return false;
	}

	// ---- Move legal: update score, delete captured pieces ----
	if (captured != nullptr)
	{
		if (currentPlayer == WHITE) whiteScore += pieceValue(captured->getSymbol());
		else                        blackScore += pieceValue(captured->getSymbol());
		delete captured;
	}
	if (epCaptured != nullptr)
	{
		if (currentPlayer == WHITE) whiteScore += pieceValue(epCaptured->getSymbol());
		else                        blackScore += pieceValue(epCaptured->getSymbol());
		delete epCaptured;
	}

	// ---- Pawn promotion ----
	if (toupper(grid[nRow][nCol]->getSymbol()) == 'P' &&
		(nRow == 0 || nRow == 7))
	{
		Color c = grid[nRow][nCol]->getColor();
		delete grid[nRow][nCol];

		// Promotion bonus points
		int promoBonus = 0;

		switch (promo)
		{
		case PROMO_R:
			grid[nRow][nCol] = new Rook(c);
			promoBonus = 4;
			break;
		case PROMO_B:
			grid[nRow][nCol] = new Bishop(c);
			promoBonus = 2;
			break;
		case PROMO_N:
			grid[nRow][nCol] = new Knight(c);
			promoBonus = 2;
			break;
		default:      // PROMO_Q
			grid[nRow][nCol] = new Queen(c);
			promoBonus = 8;
			break;
		}

		// Award bonus to the player whose pawn was promoted
		if (currentPlayer == WHITE) whiteScore += promoBonus;
		else                        blackScore += promoBonus;
	}

	return true;
}

// ======================== isCheckmate ========================

bool Board::isCheckmate(Color c)
{
	if (!isInCheck(c)) return false;

	for (int sr = 0; sr < ROWS; sr++)
		for (int sc = 0; sc < COLS; sc++)
		{
			Piece* p = grid[sr][sc];
			if (p == nullptr || p->getColor() != c) continue;

			for (int dr = 0; dr < ROWS; dr++)
				for (int dc = 0; dc < COLS; dc++)
				{
					if (!p->isValidMove(sr, sc, dr, dc, grid,
						enPassantRow, enPassantCol)) continue;
					if (grid[dr][dc] != nullptr &&
						grid[dr][dc]->getColor() == c) continue;

					Piece* savedDest = grid[dr][dc];
					grid[dr][dc] = grid[sr][sc];
					grid[sr][sc] = nullptr;

					bool stillCheck = isInCheck(c);

					grid[sr][sc] = grid[dr][dc];
					grid[dr][dc] = savedDest;

					if (!stillCheck) return false;
				}
		}

	return true;
}

// ======================== isStalemate ========================

bool Board::isStalemate(Color c)
{
	if (isInCheck(c)) return false;

	for (int sr = 0; sr < ROWS; sr++)
		for (int sc = 0; sc < COLS; sc++)
		{
			Piece* p = grid[sr][sc];
			if (p == nullptr || p->getColor() != c) continue;

			for (int dr = 0; dr < ROWS; dr++)
				for (int dc = 0; dc < COLS; dc++)
				{
					if (!p->isValidMove(sr, sc, dr, dc, grid,
						enPassantRow, enPassantCol)) continue;
					if (grid[dr][dc] != nullptr &&
						grid[dr][dc]->getColor() == c) continue;

					Piece* savedDest = grid[dr][dc];
					grid[dr][dc] = grid[sr][sc];
					grid[sr][sc] = nullptr;

					bool stillCheck = isInCheck(c);

					grid[sr][sc] = grid[dr][dc];
					grid[dr][dc] = savedDest;

					if (!stillCheck) return false;
				}
		}

	return true;
}

// ======================== onlyKingsLeft ========================

bool Board::onlyKingsLeft()
{
	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++)
			if (grid[i][j] != nullptr &&
				toupper(grid[i][j]->getSymbol()) != 'K')
				return false;
	return true;
}

// ======================== getValidMoves ========================

void Board::getValidMoves(int row, int col, bool validMoves[ROWS][COLS])
{
	for (int i = 0; i < ROWS; i++)
		for (int j = 0; j < COLS; j++)
			validMoves[i][j] = false;

	Piece* p = grid[row][col];
	if (p == nullptr) return;

	Color c = p->getColor();

	for (int dr = 0; dr < ROWS; dr++)
		for (int dc = 0; dc < COLS; dc++)
		{
			if (!p->isValidMove(row, col, dr, dc, grid,
				enPassantRow, enPassantCol)) continue;
			if (grid[dr][dc] != nullptr &&
				grid[dr][dc]->getColor() == c) continue;

			bool epCapture = (toupper(p->getSymbol()) == 'P' &&
				dr == enPassantRow && dc == enPassantCol &&
				abs(dc - col) == 1);

			Piece* epPawn = nullptr;
			if (epCapture)
			{
				epPawn = grid[row][dc];
				grid[row][dc] = nullptr;
			}

			Piece* savedDest = grid[dr][dc];
			grid[dr][dc] = grid[row][col];
			grid[row][col] = nullptr;

			bool legal = !isInCheck(c);

			grid[row][col] = grid[dr][dc];
			grid[dr][dc] = savedDest;
			if (epCapture)
				grid[row][dc] = epPawn;

			if (legal) validMoves[dr][dc] = true;
		}

	// ---- Castling squares ----
	if (toupper(p->getSymbol()) == 'K' && !p->hasMoved() &&
		!isInCheck(c))
	{
		Color opp = (c == WHITE) ? BLACK : WHITE;
		int   br = (c == WHITE) ? 7 : 0;

		if (row == br &&
			grid[row][5] == nullptr && grid[row][6] == nullptr &&
			grid[row][7] != nullptr &&
			toupper(grid[row][7]->getSymbol()) == 'R' &&
			!grid[row][7]->hasMoved() &&
			!isSquareAttacked(row, 5, opp) &&
			!isSquareAttacked(row, 6, opp))
			validMoves[row][6] = true;

		if (row == br &&
			grid[row][1] == nullptr &&
			grid[row][2] == nullptr &&
			grid[row][3] == nullptr &&
			grid[row][0] != nullptr &&
			toupper(grid[row][0]->getSymbol()) == 'R' &&
			!grid[row][0]->hasMoved() &&
			!isSquareAttacked(row, 3, opp) &&
			!isSquareAttacked(row, 2, opp))
			validMoves[row][2] = true;
	}
}

// ======================== displayBoard ========================

void Board::displayBoard(int cursorRow, int cursorCol,
	int selRow, int selCol,
	bool validMoves[ROWS][COLS]) const
{
	cout << endl;
	cout << "    a   b   c   d   e   f   g   h" << endl;
	cout << "  +---+---+---+---+---+---+---+---+" << endl;

	for (int i = 0; i < ROWS; i++)
	{
		cout << (8 - i) << " |";

		for (int j = 0; j < COLS; j++)
		{
			Piece* piece = grid[i][j];
			char   sym = piece ? piece->getSymbol() : ' ';

			bool isCursor = (i == cursorRow && j == cursorCol);
			bool isSel = (i == selRow && j == selCol);
			bool isValid = validMoves[i][j];

			if (isCursor)
				cout << "\033[44m\033[1;97m " << sym << " \033[0m|";
			else if (isSel)
				cout << "\033[42m\033[1;97m " << sym << " \033[0m|";
			else if (isValid)
			{
				if (piece == nullptr)
					cout << "\033[43m \033[1;33m.\033[43m \033[0m|";
				else
					cout << "\033[43m\033[1;30m " << sym << " \033[0m|";
			}
			else if (piece == nullptr)
				cout << "   |";
			else if (piece->getColor() == WHITE)
				cout << "\033[47m\033[1;30m " << sym << " \033[0m|";
			else
				cout << "\033[100m\033[1;97m " << sym << " \033[0m|";
		}

		cout << " " << (8 - i) << endl;
		cout << "  +---+---+---+---+---+---+---+---+" << endl;
	}

	cout << "    a   b   c   d   e   f   g   h" << endl;
}
