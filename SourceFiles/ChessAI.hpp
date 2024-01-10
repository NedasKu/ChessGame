#pragma once

#include "Board.hpp"

class ChessAI {
public:
	bool aiIsWhite = false;
	ChessAI(bool aiIsWhite) {
		this->aiIsWhite = aiIsWhite;
	}

	/*
	* Defining a structure of how a move would be made.
	* We start with a board state.
	* Each board state gets a rating based on how in favor it is for a certain color.
	* The further into the negative it is, the better the board looks for black
	* The higher it is into the positives the better it looks for white
	* Example:
	*	White is up a knight, the board state value could be 3.
	*	Black is up a pawn, the board state value could be -1.
	* With each board state getting a rating, we want to predict the best possible move
	* This is done by checking all of the possible moves that could be made, and doing the same for the opponents next move
	* This goes on based on the depth of analysis.
	*
	* For each of the possible moves, a new board state is created.
	* We want to assume that the opponent would play optimal moves. An example with AI being black.
	* The odds are even, with general board state being 0.
	* possible board states after making move: -1, -6,
	* But the the opponent for the -1 move has moves that make it 0, 1, -2
	* for the -6 move the possible states are -1, 3, 6
	* So we would assume the opponent would play the move 1 in case of -1
	* and play the move 6 in the case of -6,
	* meaning the the value of the move -6 is decided to be 6 (bad)
	* and the value of move -1 is decided to be 1 (bad, but not so bad)
	*
	* This process is repeated on each layer so we can determine the final value of the move,
	* We try to do a greedy algorithm to take the value of each to be the lowest
	*/



	int checkDepth = 10;

	const double evalPiecesModifier = 1;
	const double evalPiecesOnBackRankModifier = 0.4;
	const double checkModifier = 2;
	const double numPiecesCanMoveModifier = 0.6;
	const double pawnProgressionModifier = 0.05;

	int pawnProgressedFromStart(int y, bool isWhite) {
		if (isWhite) {
			return 6 - y;
		}
		else {
			return 1 + y;
		}
	}

	int evalPieces(Board* chessBoard, bool isWhite) {
		int sum = 0;
		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++) {
				if (chessBoard->chessBoard[x][y]->isWhite == isWhite) {
					if (chessBoard->chessBoard[x][y]->getType() == pieceTypes::Pawn) {

						sum += pieceTypes::getPieceValue(chessBoard->chessBoard[x][y]->getType()) + pawnProgressedFromStart(y, isWhite) * pawnProgressionModifier;
					}
					else {
						sum += pieceTypes::getPieceValue(chessBoard->chessBoard[x][y]->getType());
					}
				}
			}
		}
		return sum;
	}

	int evalPiecesOnBackRank(Board* chessBoard, bool isWhite) {
		int sum = 0;
		int backrank = isWhite ? 7 : 0;
		for (int x = 0; x < 8; x++) {
			if ((chessBoard->chessBoard[x][backrank]->isWhite == isWhite)
					&& chessBoard->chessBoard[x][backrank]->getType() != pieceTypes::None
					&& chessBoard->chessBoard[x][backrank]->getType() != pieceTypes::King
					&& chessBoard->chessBoard[x][backrank]->getType() != pieceTypes::Rook
				) {
				sum ++;
			}
		}
		return sum;
	}

	int numPiecesCanMove(Board* chessBoard, bool isWhite) {
		int sum = 0;
		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++) {
				if (chessBoard->chessBoard[x][y]->isWhite == isWhite) {
					if (chessBoard->getMoves(chessBoard->chessBoard[x][y]).size() > 0) {
						sum ++;
					}
				}
			}
		}
		return sum;
	}

	double getBoardValue(Board* chessBoard) {
		double value = 0;
		value += (evalPieces(chessBoard, true) - evalPieces(chessBoard, false)) * evalPiecesModifier;

		value -= evalPiecesOnBackRank(chessBoard, true) * evalPiecesOnBackRankModifier;
		value += evalPiecesOnBackRank(chessBoard, false) * evalPiecesOnBackRankModifier;

		value += (numPiecesCanMove(chessBoard, true) - numPiecesCanMove(chessBoard, false)) * numPiecesCanMoveModifier;

		if (chessBoard->whiteLost()) {
			value -= 1000;
		}

		if (chessBoard->blackLost()) {
			value += 1000;
		}

		if (chessBoard->whiteInCheck) {
			value -= 2 * checkModifier;
		}

		if (chessBoard->blackInCheck) {
			value += 2 * checkModifier;
		}


		return value;
	}

	bool playAIMove(Board* chessBoard) {
		std::vector<std::pair<Board*, Move*>> potentialAIMoves;

		// First run for All of the AI moves
		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++) {
				auto piece = chessBoard->chessBoard[x][y];
				if (piece->isWhite == aiIsWhite && piece->getType() != pieceTypes::PieceType::None) {
					auto specificPieceMoves = chessBoard->getMoves(piece);
					for (auto it : specificPieceMoves) {
						Board* copyBoard = chessBoard->copy();
						copyBoard->movePieceTo(copyBoard->chessBoard[piece->x][piece->y], it.first, it.second);
						double moveValue = getBoardValue(copyBoard);
						Move* move = new Move(piece, it.first, it.second, moveValue);
						potentialAIMoves.push_back(std::make_pair(copyBoard, move));
					}
				}
			}
		}
		// Adjust all Board moves based on potential responses from the opponent
		bool shouldCheckAIMove = !aiIsWhite;
		for (int i = 0; i < checkDepth; i ++) {
			for (auto move : potentialAIMoves) {
				auto copiedChessBoard = move.first->copy();
				for (int x = 0; x < 8; x++) {
					for (int y = 0; y < 8; y++) {
						auto piece = copiedChessBoard->chessBoard[x][y];
						if (piece->isWhite == shouldCheckAIMove && piece->getType() != pieceTypes::PieceType::None) {
							auto specificPieceMoves = move.first->getMoves(piece);
							for (auto pieceMove : specificPieceMoves) {
								Board* copyBoard = move.first->copy();
								copyBoard->movePieceTo(copyBoard->chessBoard[piece->x][piece->y], pieceMove.first, pieceMove.second);
								double moveValue = getBoardValue(copyBoard);
								if (aiIsWhite) {
									if (moveValue < move.second->value) {
										move.first = copyBoard;
										move.second->value = moveValue;
									}
								}
								else {
									if (moveValue > move.second->value) {
										move.first = copyBoard;
										move.second->value = moveValue;
									}
								}
							}
						}
					}
				}
			}
			shouldCheckAIMove = !shouldCheckAIMove;
		}


		Move* bestMove = nullptr;
		double bestMoveValue = -1000;
		for (auto it : potentialAIMoves) {
			if (bestMoveValue == -1000) {
				bestMove = it.second;
				bestMoveValue = it.second->value;
				continue;
			}

			if (aiIsWhite) {
				if (bestMoveValue < it.second->value) {
					bestMove = it.second;
					bestMoveValue = it.second->value;
				}
			}
			else {
				if (bestMoveValue > it.second->value) {
					bestMove = it.second;
					bestMoveValue = it.second->value;
				}
			}
		}

		if (bestMove == nullptr) {
			return false;
		}

		// If there are many moves with the same best value, select one at random
		std::vector<Move*> bestMoves;
		for (auto it : potentialAIMoves) {
			if (it.second->value == bestMoveValue) {
				bestMoves.push_back(it.second);
			}
		}
		int randMove = (rand() % bestMoves.size());
		chessBoard->movePieceTo(bestMoves[randMove]->pieceToMove, bestMoves[randMove]->xToMove, bestMoves[randMove]->yToMove);
		return true;
	}
};
