#pragma once

#include "ChessPiece.hpp"

class Bishop : public ChessPiece {
public:
	// Used for mock pieces needed for some logic
	Bishop(int x, int y, bool isWhite) : ChessPiece(x, y, pieceTypes::PieceType::Bishop) {
		this->isWhite = isWhite;
	}

	Bishop(int x, int y, SDL_Texture* texture, bool isWhite) : ChessPiece(x, y, pieceTypes::PieceType::Bishop) {
		setTexture(texture);
		this->isWhite = isWhite;
	}
	std::vector<std::pair<int, int>> getPotentialMoves(ChessPiece* chessBoard[8][8]);
};
