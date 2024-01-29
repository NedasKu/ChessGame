#pragma once

#include "ChessPiece.hpp"
#include "Bishop.hpp"
#include "King.hpp"
#include "Knight.hpp"
#include "Rook.hpp"
#include "Queen.hpp"
#include "Pawn.hpp"
#include "Move.hpp"

#include <vector>
#include <iostream>
#include <math.h>

class Board {
public:
	ChessPiece* chessBoard[8][8];

	Board() {
		for (int i = 0; i < 8; i++) {
			for (int y = 0; y < 8; y++) {
				chessBoard[i][y] = new ChessPiece(i, y, pieceTypes::PieceType::None);
			}
		}
	}
	std::vector<Move*> movesMade;


	// Check Board states
	bool whiteInCheck = false;
	bool blackInCheck = false;
	bool isCopy = false;

	std::vector<ChessPiece*> piecesCheckingWhite;
	std::vector<ChessPiece*> piecesCheckingBlack;

	bool whiteKingCheck();

	bool blackKingCheck();

	void movePieceTo(ChessPiece* piece, int x, int y);
	Board* copy();

	void clear(int x, int y, ChessPiece* chessBoard[8][8]);

	std::vector<std::pair<int, int>> getMoves(ChessPiece* piece);

	bool whiteKingSaved(ChessPiece* piece, ChessPiece* potentialPosition);
	bool blackKingSaved(ChessPiece* piece, ChessPiece* potentialPosition);

	bool pruneMoves(ChessPiece* piece, int potentialX, int potentialY);

	std::vector<std::pair<int, int>> getPotentialMoves(ChessPiece* piece);

	bool checkKingStates();
	bool whiteLost();
	bool blackLost();
	bool draw(bool isWhiteTurn);
};