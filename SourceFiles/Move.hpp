#pragma once

#include "Board.hpp"

class Move {
public:
	ChessPiece* pieceToMove;
	int xToMove;
	int yToMove;
	double value;
	Move(ChessPiece* pieceToMove, int xToMove, int yToMove, double value) {
		this->pieceToMove = pieceToMove;
		this->xToMove = xToMove;
		this->yToMove = yToMove;
		this->value = value;
	}

	std::string toString() {
		return pieceTypes::getName(pieceToMove->getType()) + " to " + std::to_string(xToMove) + "," + std::to_string(yToMove);
	}
};
