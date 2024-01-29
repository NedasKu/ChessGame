#pragma once

#include <SDL_image.h>
#include "PieceType.hpp"
#include <vector>

class ChessPiece {
public:
	int x;
	int y;
	bool isWhite;
	bool drawn = false;
	bool selected = false;
	bool moved = false;

	ChessPiece(int x, int y, pieceTypes::PieceType type);
	ChessPiece(int x, int y, pieceTypes::PieceType type, bool isWhite);

	pieceTypes::PieceType getType();
	void setType(pieceTypes::PieceType newType);
	SDL_Texture* getTexture();
	void setTexture(SDL_Texture* newTexture);

private:
	SDL_Texture* texture;
	pieceTypes::PieceType type;
};