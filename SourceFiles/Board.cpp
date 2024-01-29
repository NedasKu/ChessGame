#pragma once

#include "Board.hpp"

bool Board::whiteKingCheck() {
	ChessPiece* whiteKing = nullptr;
	auto board = chessBoard;
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			auto piece = board[x][y];
			if (piece->getType() == pieceTypes::PieceType::King) {
				if (piece->isWhite) {
					whiteKing = piece;
				}
			}
		}
	}
	int whiteKingCoordX = whiteKing->x;
	int whiteKingCoordY = whiteKing->y;
	whiteInCheck = false;
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (board[x][y]->isWhite) {
				continue;
			}
			auto moves = getMoves(board[x][y]);
			for (auto& it : moves) {
				if (it.first == whiteKingCoordX && it.second == whiteKingCoordY) {
					whiteInCheck = true;
				}
			}
		}
	}

	return whiteInCheck;
}

bool Board::blackKingCheck() {
	ChessPiece* blackKing = nullptr;

	auto board = chessBoard;
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			auto piece = board[x][y];
			if (piece->getType() == pieceTypes::PieceType::King) {
				if (!piece->isWhite) {
					blackKing = piece;
				}
			}
		}
	}

	int blackKingCoordX = blackKing->x;
	int blackKingCoordY = blackKing->y;
	blackInCheck = false;
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (!board[x][y]->isWhite) {
				continue;
			}
			auto moves = getMoves(board[x][y]);
			for (auto& it : moves) {
				if (it.first == blackKingCoordX && it.second == blackKingCoordY) {
					blackInCheck = true;
				}
			}
		}
	}

	return blackInCheck;
}

void Board::movePieceTo(ChessPiece* piece, int x, int y) {
	movesMade.emplace_back(new Move(piece, x, y, 0));
	int prevX = piece->x;
	int prevY = piece->y;
	piece->selected = false;
	piece->moved = true;
	piece->x = x;
	piece->y = y;
	chessBoard[x][y] = piece;
	clear(prevX, prevY, chessBoard);
	// Special move for castling
	if (piece->getType() == pieceTypes::PieceType::King) {
		if (abs(prevX - x) > 1) {
			if (prevX < x) {
				ChessPiece* potentialRightRook = chessBoard[7][prevY];
				movePieceTo(potentialRightRook, prevX + 1, prevY);
			}
			else {
				ChessPiece* potentialLeftRook = chessBoard[0][prevY];
				movePieceTo(potentialLeftRook, prevX - 1, prevY);
			}
		}
	}
}

Board* Board::copy() {
	auto copy = new Board();
	for (int i = 0; i < 8; i++) {
		for (int y = 0; y < 8; y++) {
			copy->chessBoard[i][y] = new ChessPiece(i, y, chessBoard[i][y]->getType(), chessBoard[i][y]->isWhite);
		}
	}
	copy->isCopy = true;
	return copy;
}

void Board::clear(int x, int y, ChessPiece* chessBoard[8][8]) {
	chessBoard[x][y] = new ChessPiece(x, y, pieceTypes::PieceType::None);
}

std::vector<std::pair<int, int>> Board::getMoves(ChessPiece* piece) {
	auto potentialMoves = getPotentialMoves(piece);
	for (int i = 0; i < potentialMoves.size(); i++) {
		if (!pruneMoves(piece, potentialMoves[i].first, potentialMoves[i].second)) {
			potentialMoves.erase(potentialMoves.begin() + i);
			i--;
		}
	}

	return potentialMoves;
}

bool Board::whiteKingSaved(ChessPiece* piece, ChessPiece* potentialPosition) {
	Board* newBoard = copy();

	newBoard->movePieceTo(newBoard->chessBoard[piece->x][piece->y], potentialPosition->x, potentialPosition->y);
	return !newBoard->whiteKingCheck();
}

bool Board::blackKingSaved(ChessPiece* piece, ChessPiece* potentialPosition) {
	Board* newBoard = copy();

	newBoard->movePieceTo(newBoard->chessBoard[piece->x][piece->y], potentialPosition->x, potentialPosition->y);
	return !newBoard->blackKingCheck();
}


bool Board::pruneMoves(ChessPiece* piece, int potentialX, int potentialY) {
	// TODO: check what causes this to trigger
	// Not a real move, this is staying in place
	if (piece->x == potentialX && piece->y == potentialY) {
		return false;
	}
	// Don't move out of the board
	if (potentialX > 7 || potentialY > 7 || potentialX < 0 || potentialY < 0) {
		return false;
	}

	ChessPiece* potentialPosition = chessBoard[potentialX][potentialY];
	if (potentialPosition->getType() != pieceTypes::PieceType::None) {
		if (piece->isWhite == potentialPosition->isWhite) {
			return false;
		}
	}

	if (piece->isWhite && !isCopy) {
		if (!whiteKingSaved(piece, potentialPosition)) {
			return false;
		}
	}

	if (!piece->isWhite && !isCopy) {
		if (!blackKingSaved(piece, potentialPosition)) {
			return false;
		}
	}

	// special case for the king when castling, don't allow a castling if path is obstructed by a check
	if (piece->getType() == pieceTypes::PieceType::King) {

	}


	return true;
}

std::vector<std::pair<int, int>> Board::getPotentialMoves(ChessPiece* piece) {
	switch (piece->getType()) {
	case pieceTypes::PieceType::Bishop:
		return static_cast<Bishop*>(piece)->getPotentialMoves(chessBoard);
	case pieceTypes::PieceType::Knight:
		return static_cast<Knight*>(piece)->getPotentialMoves(chessBoard);
	case pieceTypes::PieceType::King:
		return static_cast<King*>(piece)->getPotentialMoves(chessBoard);
	case pieceTypes::PieceType::Rook:
		return static_cast<Rook*>(piece)->getPotentialMoves(chessBoard);
	case pieceTypes::PieceType::Queen:
		return static_cast<Queen*>(piece)->getPotentialMoves(chessBoard);
	case pieceTypes::PieceType::Pawn:
		return static_cast<Pawn*>(piece)->getPotentialMoves(chessBoard);
	case pieceTypes::PieceType::None:
		return std::vector<std::pair<int, int>>();
	default:
		std::cout << "Type error" << std::endl;
		return std::vector<std::pair<int, int>>();
	}
}

// Check the board after the moves for if any king has just been checked
bool Board::checkKingStates() {
	bool thereIsCheck = false;
	thereIsCheck |= whiteKingCheck();
	thereIsCheck |= blackKingCheck();
	for (auto it : piecesCheckingBlack) {
		std::cout << "black king checked by - " << it->x << " " << it->y << std::endl;
	}
	for (auto it : piecesCheckingWhite) {
		std::cout << "white king checked by - " << it->x << " " << it->y << std::endl;
	}
	return thereIsCheck;
}


bool Board::whiteLost() {
	if (!whiteInCheck) {
		return false;
	}

	std::vector<std::pair<int, int>> possibleMoves;
	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 7; y++) {
			if (chessBoard[x][y]->isWhite) {
				auto possibleMovesForPiece = Board::getMoves(chessBoard[x][y]);
				possibleMoves.insert(possibleMoves.end(), possibleMovesForPiece.begin(), possibleMovesForPiece.end());
			}
		}
	}

	return possibleMoves.size() == 0;
}

bool Board::blackLost() {
	if (!blackInCheck) {
		return false;
	}

	std::vector<std::pair<int, int>> possibleMoves;
	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 7; y++) {
			if (!chessBoard[x][y]->isWhite) {
				auto possibleMovesForPiece = Board::getMoves(chessBoard[x][y]);
				possibleMoves.insert(possibleMoves.end(), possibleMovesForPiece.begin(), possibleMovesForPiece.end());
			}
		}
	}

	return possibleMoves.size() == 0;
}

bool Board::draw(bool isWhiteTurn) {
	if (blackInCheck || whiteInCheck) {
		return false;
	}

	std::vector<std::pair<int, int>> possibleMoves;
	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 7; y++) {
			if (!chessBoard[x][y]->isWhite == isWhiteTurn) {
				auto possibleMovesForPiece = Board::getMoves(chessBoard[x][y]);
				possibleMoves.insert(possibleMoves.end(), possibleMovesForPiece.begin(), possibleMovesForPiece.end());
			}
		}
	}
	return possibleMoves.size() == 0;
}
