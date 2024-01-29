#pragma once

#include "ChessPiece.hpp"

class Rook : public ChessPiece {
public:
	// Used for mock pieces needed for some logic
	Rook(int x, int y, bool isWhite) : ChessPiece(x, y, pieceTypes::PieceType::Rook) {
		this->isWhite = isWhite;
	}

	// Used for initialization with textures
	Rook(int x, int y, SDL_Texture* texture, bool isWhite) : ChessPiece(x, y, pieceTypes::PieceType::Rook) {
		setTexture(texture);
		this->isWhite = isWhite;
	}

	std::vector<std::pair<int, int>> getPotentialMoves(ChessPiece* chessBoard[8][8]);
};
