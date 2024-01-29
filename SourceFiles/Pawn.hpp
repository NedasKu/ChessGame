#pragma once

#include "ChessPiece.hpp"

class Pawn : public ChessPiece {
public:
	bool hasMoved = false;

	Pawn(int x, int y, SDL_Texture* texture, bool isWhite) : ChessPiece(x, y, pieceTypes::PieceType::Pawn) {
		setTexture(texture);
		this->isWhite = isWhite;
	}

	std::vector<std::pair<int, int>> getPotentialMoves(ChessPiece* chessBoard[8][8]);
};
