#pragma once

#include "ChessPiece.hpp"
#include <SDL_image.h>
#include <vector>

class Queen : public ChessPiece {
public:
	Queen(int x, int y, SDL_Texture* texture, bool isWhite) : ChessPiece(x, y, pieceTypes::PieceType::Queen) {
		setTexture(texture);
		this->isWhite = isWhite;
	}

	std::vector<std::pair<int, int>> getPotentialMoves(ChessPiece* chessBoard[8][8]) {
		std::vector <std::pair<int, int>> potentialMoves;
		// Steal moves from rook and bishop
		auto fakeRook = new Rook(x, y, isWhite);
		auto potentialRookMoves = fakeRook->getPotentialMoves(chessBoard);
		potentialMoves.insert(potentialMoves.end(), potentialRookMoves.begin(), potentialRookMoves.end());

		auto fakeBishop = new Bishop(x, y, isWhite);
		auto potentialBishopMoves = fakeBishop->getPotentialMoves(chessBoard);
		potentialMoves.insert(potentialMoves.end(), potentialBishopMoves.begin(), potentialBishopMoves.end());

		return potentialMoves;
	}
};