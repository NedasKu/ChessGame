#incude "Bishop.hpp"

std::vector<std::pair<int, int>> Bishop::getPotentialMoves(ChessPiece* chessBoard[8][8]) {
	std::vector <std::pair<int, int>> potentialMoves;

	// stopper mechanism for each diagonal if encountered a piece
	bool stopper1 = false;
	bool stopper2 = false;
	bool stopper3 = false;
	bool stopper4 = false;

	for (int i = 1; i < 8; i++) {
		if (!stopper1) {
			// out of bounds check first, as other checks need to check board pieces
			if (x + i > 7 || y + i > 7) {
				stopper1 = true;
			}
			else {
				if (chessBoard[x + i][y + i]->getType() == pieceTypes::PieceType::None) {
					potentialMoves.push_back(std::make_pair(x + i, y + i));
				}
				else {
					if (chessBoard[x + i][y + i]->isWhite != isWhite) {
						potentialMoves.push_back(std::make_pair(x + i, y + i));
					}
					stopper1 = true;
				}
			}
		}

		if (!stopper2) {
			if (x - i < 0 || y + i > 7) {
				stopper2 = true;
			}
			else {
				if (chessBoard[x - i][y + i]->getType() == pieceTypes::PieceType::None) {
					potentialMoves.push_back(std::make_pair(x - i, y + i));
				}
				else {
					if (chessBoard[x - i][y + i]->isWhite != isWhite) {
						potentialMoves.push_back(std::make_pair(x - i, y + i));
					}
					stopper2 = true;
				}
			}

		}

		if (!stopper3) {
			if (x + i > 7 || y - i < 0) {
				stopper3 = true;
			}
			else {
				if (chessBoard[x + i][y - i]->getType() == pieceTypes::PieceType::None) {
					potentialMoves.push_back(std::make_pair(x + i, y - i));
				}
				else {
					if (chessBoard[x + i][y - i]->isWhite != isWhite) {
						potentialMoves.push_back(std::make_pair(x + i, y - i));
					}
					stopper3 = true;
				}
			}
		}

		if (!stopper4) {
			if (x - i < 0 || y - i < 0) {
				stopper4 = true;
			}
			else {
				if (chessBoard[x - i][y - i]->getType() == pieceTypes::PieceType::None) {
					potentialMoves.push_back(std::make_pair(x - i, y - i));
				}
				else {
					if (chessBoard[x - i][y - i]->isWhite != isWhite) {
						potentialMoves.push_back(std::make_pair(x - i, y - i));
					}
					stopper4 = true;
				}
			}
		}
	}

	return potentialMoves;
}
