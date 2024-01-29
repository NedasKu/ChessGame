#include "Rook.hpp"

std::vector<std::pair<int, int>> Rook::getPotentialMoves(ChessPiece* chessBoard[8][8]) {
	std::vector <std::pair<int, int>> potentialMoves;

	for (int i = y + 1; i < 8; i++) {
		if (chessBoard[x][i]->getType() != pieceTypes::PieceType::None) {
			if (chessBoard[x][i]->isWhite != isWhite) {
				potentialMoves.push_back(std::make_pair(x, i));
			}
			break;
		}
		potentialMoves.push_back(std::make_pair(x, i));
	}

	for (int i = y - 1; i >= 0; i--) {
		if (chessBoard[x][i]->getType() != pieceTypes::PieceType::None) {
			if (chessBoard[x][i]->isWhite != isWhite) {
				potentialMoves.push_back(std::make_pair(x, i));
			}
			break;
		}
		potentialMoves.push_back(std::make_pair(x, i));
	}


	for (int i = x - 1; i >= 0; i--) {
		if (chessBoard[i][y]->getType() != pieceTypes::PieceType::None) {
			if (chessBoard[i][y]->isWhite != isWhite) {
				potentialMoves.push_back(std::make_pair(i, y));
			}
			break;
		}
		potentialMoves.push_back(std::make_pair(i, y));
	}

	for (int i = x + 1; i < 8; i++) {
		if (chessBoard[i][y]->getType() != pieceTypes::PieceType::None) {
			if (chessBoard[i][y]->isWhite != isWhite) {
				potentialMoves.push_back(std::make_pair(i, y));
			}
			break;
		}
		potentialMoves.push_back(std::make_pair(i, y));
	}

	return potentialMoves;
}
