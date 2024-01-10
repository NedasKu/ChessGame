#pragma once

#include "ChessPiece.hpp"
#include <SDL_image.h>
#include <vector>


class Knight : public ChessPiece {
public:
	Knight(int x, int y, SDL_Texture* texture, bool isWhite) : ChessPiece(x, y, pieceTypes::PieceType::Knight) {
		setTexture(texture);
		this->isWhite = isWhite;
	}

	std::vector<std::pair<int, int>> getPotentialMoves(ChessPiece* chessBoard[8][8]) {
		std::vector <std::pair<int, int>> potentialMoves;
		potentialMoves.push_back(std::make_pair(x+1, y+2));
		potentialMoves.push_back(std::make_pair(x+1, y-2));
		potentialMoves.push_back(std::make_pair(x-1, y+2));
		potentialMoves.push_back(std::make_pair(x-1, y-2));
		potentialMoves.push_back(std::make_pair(x+2, y-1));
		potentialMoves.push_back(std::make_pair(x+2, y+1));
		potentialMoves.push_back(std::make_pair(x-2, y+1));
		potentialMoves.push_back(std::make_pair(x-2, y-1));

		return potentialMoves;
	}
};