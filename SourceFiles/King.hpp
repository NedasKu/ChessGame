#pragma once

#include "ChessPiece.hpp"
#include <SDL_image.h>
#include <vector>

class King : public ChessPiece {
public:
	King(int x, int y, SDL_Texture* texture, bool isWhite) : ChessPiece(x, y, pieceTypes::PieceType::King) {
		setTexture(texture);
		this->isWhite = isWhite;
	}

	std::vector<std::pair<int, int>> getPotentialMoves(ChessPiece* chessBoard[8][8]) {
		std::vector <std::pair<int, int>> potentialMoves;
		potentialMoves.push_back(std::make_pair(x, y + 1));
		potentialMoves.push_back(std::make_pair(x, y - 1));
		potentialMoves.push_back(std::make_pair(x-1, y));
		potentialMoves.push_back(std::make_pair(x-1, y + 1));
		potentialMoves.push_back(std::make_pair(x-1, y - 1));
		potentialMoves.push_back(std::make_pair(x+1, y ));
		potentialMoves.push_back(std::make_pair(x+1, y - 1));
		potentialMoves.push_back(std::make_pair(x+1, y + 1));

		// castling moves, if haven't moved, and rook in the corner hasn't moved, you can castle
		if (!moved) {
			auto potentialLeftRook = chessBoard[0][y];
			if (potentialLeftRook->getType() == pieceTypes::PieceType::Rook && potentialLeftRook->isWhite == isWhite && !potentialLeftRook->moved) {
				potentialMoves.push_back(std::make_pair(x - 2, y));
			}

			auto potentialRightRook = chessBoard[7][y];
			if (potentialRightRook->getType() == pieceTypes::PieceType::Rook && potentialRightRook->isWhite == isWhite && !potentialRightRook->moved) {
				potentialMoves.push_back(std::make_pair(x + 2, y));
			}
		}

		return potentialMoves;
	}

};