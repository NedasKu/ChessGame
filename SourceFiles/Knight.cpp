#include "Knight.hpp"

std::vector<std::pair<int, int>> Knight::getPotentialMoves(ChessPiece* chessBoard[8][8]) {
	std::vector <std::pair<int, int>> potentialMoves;
	potentialMoves.push_back(std::make_pair(x + 1, y + 2));
	potentialMoves.push_back(std::make_pair(x + 1, y - 2));
	potentialMoves.push_back(std::make_pair(x - 1, y + 2));
	potentialMoves.push_back(std::make_pair(x - 1, y - 2));
	potentialMoves.push_back(std::make_pair(x + 2, y - 1));
	potentialMoves.push_back(std::make_pair(x + 2, y + 1));
	potentialMoves.push_back(std::make_pair(x - 2, y + 1));
	potentialMoves.push_back(std::make_pair(x - 2, y - 1));

	return potentialMoves;
}
