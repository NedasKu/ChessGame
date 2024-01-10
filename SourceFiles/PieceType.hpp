#pragma once

#include <string>

namespace pieceTypes {
	enum PieceType {
		None = 0,
		Pawn,
		Rook,
		Bishop,
		Knight,
		Queen,
		King
	};
	int getPieceValue(PieceType type);
	std::string getName(PieceType type);
}