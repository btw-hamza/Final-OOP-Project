// ============================ Pieces.h ============================

#pragma once
#include <iostream>
#include <cmath>
using namespace std;

const int ROWS = 8;
const int COLS = 8;

// Color enum (OOP: enum used instead of raw int/bool)
enum Color
{
	WHITE,
	BLACK
};

// ============================ Base Class : Piece ============================
class Piece
{
protected:
	Color color;
	char  symbol;
	bool  moved;

public:
	Piece(Color c, char s);

	// Pure virtual - Polymorphism
	virtual bool isValidMove(
		int cRow, int cCol,
		int nRow, int nCol,
		Piece* board[ROWS][COLS],
		int enPassantRow,
		int enPassantCol) = 0;

	Color getColor()    const;
	char  getSymbol()   const;
	bool  hasMoved()    const;
	void  setMoved(bool m);

	virtual ~Piece();  // virtual destructor - required for polymorphic delete
};

// ============================ Derived Classes ============================

class Pawn : public Piece
{
public:
	Pawn(Color c);
	bool isValidMove(int cRow, int cCol, int nRow, int nCol,
		Piece* board[ROWS][COLS], int enPassantRow, int enPassantCol);
	~Pawn() {}
};

class Rook : public Piece
{
public:
	Rook(Color c);
	bool isValidMove(int cRow, int cCol, int nRow, int nCol,
		Piece* board[ROWS][COLS], int enPassantRow, int enPassantCol);
	~Rook() {}
};

class Knight : public Piece
{
public:
	Knight(Color c);
	bool isValidMove(int cRow, int cCol, int nRow, int nCol,
		Piece* board[ROWS][COLS], int enPassantRow, int enPassantCol);
	~Knight() {}
};

class Bishop : public Piece
{
public:
	Bishop(Color c);
	bool isValidMove(int cRow, int cCol, int nRow, int nCol,
		Piece* board[ROWS][COLS], int enPassantRow, int enPassantCol);
	~Bishop() {}
};

class Queen : public Piece
{
public:
	Queen(Color c);
	bool isValidMove(int cRow, int cCol, int nRow, int nCol,
		Piece* board[ROWS][COLS], int enPassantRow, int enPassantCol);
	~Queen() {}
};

class King : public Piece
{
public:
	King(Color c);
	bool isValidMove(int cRow, int cCol, int nRow, int nCol,
		Piece* board[ROWS][COLS], int enPassantRow, int enPassantCol);
	~King() {}
};
