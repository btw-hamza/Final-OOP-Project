Chess Game – OOP Final Project

A fully functional console-based Chess Game in C++ developed as the final project for the Object Oriented Programming (OOP) Lab course at FAST National University of Computer and Emerging Sciences.

Group Members
| Roll Number | Name                 |
| ----------- | -------------------- |
| 25F-0702    | Muhammad Hamza Nasir |
| 25F-0589    | Hayan Tariq          |
| 25F-0807    | Muhammad Ali Shahid  |

Course Information

Course: Object Oriented Programming Lab
Instructor: Wania Fatima
Semester: Spring 2026

Introduction

Chess is one of the oldest and most strategic board games in the world.

This project implements a complete console-based Chess Game in C++ supporting:

Legal move validation
Check and checkmate detection
Stalemate detection
Castling
En passant
Pawn promotion
Material score system
ANSI colored console interface

The project was designed to demonstrate strong understanding of:

Encapsulation
Inheritance
Polymorphism
Composition
Dynamic Memory Allocation (DMA)
The codebase is modular, clean, and extensible.

Project Structure

├── Pieces.h
├── Pieces.cpp
├── Board.h
├── Board.cpp
├── Main.cpp
└── README.md

File Responsibilities
| File                    | Description                                      |
| ----------------------- | ------------------------------------------------ |
| `Pieces.h / Pieces.cpp` | Base class and all 6 derived chess piece classes |
| `Board.h / Board.cpp`   | Board management, move execution, game state     |
| `Main.cpp`              | Game loop, UI rendering, and input handling      |

OOP Concepts Demonstrated
| Concept                   | Implementation                                                               |
| ------------------------- | ---------------------------------------------------------------------------- |
| Encapsulation             | Private/protected members with controlled access through getters/setters     |
| Inheritance               | `Pawn`, `Rook`, `Knight`, `Bishop`, `Queen`, and `King` inherit from `Piece` |
| Polymorphism              | Runtime dispatch using virtual `isValidMove()`                               |
| Composition               | `Board` contains and manages all chess pieces                                |
| Abstract Class            | `Piece` class contains pure virtual functions                                |
| Dynamic Memory Allocation | Pieces created with `new` and destroyed using `delete`                       |
| Enums                     | Used for colors and promotion choices                                        |

Encapsulation

The project protects internal state using private/protected members.
Example
protected:
    Color color;
    char symbol;
    bool moved;
Access is only provided through controlled public methods:
getColor()
getSymbol()
hasMoved()
setMoved()
The Board class also hides its internal state such as:
Grid
Scores
En passant tracking
Board state management

Inheritance

All chess pieces inherit from the abstract Piece base class.
class Pawn : public Piece
class Rook : public Piece
class Knight : public Piece
class Bishop : public Piece
class Queen : public Piece
class King : public Piece
Each derived class implements its own movement logic through:
isValidMove()

Polymorphism

The project heavily uses runtime polymorphism.
The board stores all pieces as:
Piece* grid[8][8];
At runtime, each pointer may point to:
Pawn
Rook
Knight
Bishop
Queen
King
Example:
p->isValidMove(...)
The correct derived function is automatically called through virtual dispatch.

Composition

The Board class HAS-A collection of chess pieces.
Piece* grid[ROWS][COLS];
The board:
Creates pieces
Owns pieces
Manages pieces
Deletes pieces
This demonstrates strong composition and ownership.

Dynamic Memory Allocation

All pieces are allocated dynamically:
grid[6][i] = new Pawn(WHITE);
grid[0][4] = new King(BLACK);
Captured pieces are deleted immediately:
delete grid[row][col];
The destructor cleans up all memory to avoid leaks.

Features Implemented
| Feature                   | Description                          |
| ------------------------- | ------------------------------------ |
| Legal Move Validation   | Each piece validates its own moves     |
| Check Detection         | Detects attacks on the king            |
| Checkmate Detection     | Simulates all possible escape moves    |
| Stalemate Detection     | Detects draw when no legal moves exist |
| Castling                | Full king-side and queen-side castling |
| En Passant              | Special pawn capture implemented       |
| Pawn Promotion          | Promote to Queen/Rook/Bishop/Knight    |
| Score System            | Piece values and promotion bonuses     |
| Valid Move Highlighting | Shows safe legal moves                 |
| Resign Option           | Player can resign anytime              |
| ANSI Colored UI         | Colored console interface              |

Console UI Features
The game uses ANSI escape codes for a better console experience.
| Color     | Purpose     |
| --------- | ----------- |
| Blue   | Cursor         |
| Green  | Selected piece |
| Yellow | Valid moves    |
| Red    | Check warning  |

How to Run

Compile
g++ Main.cpp Board.cpp Pieces.cpp -o chess
Run
./chess
Key Learning Outcomes

Through this project we learned:

Real-world implementation of OOP principles
Runtime polymorphism in large systems
Dynamic memory management
Complex game logic handling
State simulation and rollback systems
Modular software design in C++

Conclusion:

This project successfully demonstrates all major Object-Oriented Programming concepts through a complete playable chess game.
The system includes:
Clean architecture
Modular design
Full chess mechanics
Dynamic memory handling
Advanced rule validation
The result is a polished semester-level C++ project that combines both strong programming fundamentals and practical software engineering concepts.

Suggested Future Improvements:

GUI version using SFML or SDL
AI opponent using Minimax Algorithm
Save/Load game system
Multiplayer support
Move history and replay system
Undo/Redo functionality

License

This project was developed for academic purposes as part of the OOP Lab course at FAST National University of Computer and Emerging Sciences.
