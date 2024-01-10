#include "PieceType.hpp"

int pieceTypes::getPieceValue(PieceType type) {
	switch (type) {
	case None:
		return 0;
	case Pawn:
		return 1;
	case Rook:
		return 5;
	case Bishop:
		return 3;
	case Knight:
		return 3;
	case Queen:
		return 9;
	case King:
		return 0;
	default:
		return 0;
	}
}
std::string pieceTypes::getName(PieceType type) {
	switch (type) {
	case None:
		return "";
	case Pawn:
		return "Pawn";
	case Rook:
		return "Rook";
	case Bishop:
		return "Bishop";
	case Knight:
		return "Knight";
	case Queen:
		return "Queen";
	case King:
		return "King";
	default:
		return "";
	}
}
