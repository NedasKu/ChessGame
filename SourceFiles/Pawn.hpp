#pragma once

#include "ChessPiece.hpp"
#include <SDL_image.h>
#include <vector>

class Pawn : public ChessPiece {
public:
	bool hasMoved = false;

	Pawn(int x, int y, SDL_Texture* texture, bool isWhite) : ChessPiece(x, y, pieceTypes::PieceType::Pawn) {
		setTexture(texture);
		this->isWhite = isWhite;
	}
	
	std::vector<std::pair<int, int>> getPotentialMoves(ChessPiece* chessBoard[8][8]) {
		std::vector <std::pair<int, int>> potentialMoves;
		int direction = isWhite ? -1 : 1;
		
		// Movement
		if (chessBoard[x][y + direction]->getType() == pieceTypes::PieceType::None) {
			potentialMoves.push_back(std::make_pair(x, y  + direction));
			if (!moved) {
				if (chessBoard[x][y + 2* direction]->getType() == pieceTypes::PieceType::None) {
					potentialMoves.push_back(std::make_pair(x, y + 2* direction));
				}
			}
		}

		// Capturing
		if (x > 0) {
			if (chessBoard[x - 1][y + direction]->getType() != pieceTypes::PieceType::None && chessBoard[x - 1][y + direction]->isWhite != isWhite) {
				potentialMoves.push_back(std::make_pair(x-1, y + direction));
			}
		}

		if (x < 7) {
			if (chessBoard[x + 1][y + direction]->getType() != pieceTypes::PieceType::None && chessBoard[x + 1][y + direction]->isWhite != isWhite) {
				potentialMoves.push_back(std::make_pair(x + 1, y + direction));
			}
		}


		return potentialMoves;
	}

};