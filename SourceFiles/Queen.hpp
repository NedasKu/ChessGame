#pragma once

#include "ChessPiece.hpp"

class Queen : public ChessPiece {
public:
	Queen(int x, int y, SDL_Texture* texture, bool isWhite) : ChessPiece(x, y, pieceTypes::PieceType::Queen) {
		setTexture(texture);
		this->isWhite = isWhite;
	}
	std::vector<std::pair<int, int>> getPotentialMoves(ChessPiece* chessBoard[8][8]);
};
