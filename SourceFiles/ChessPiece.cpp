#include "ChessPiece.hpp"

ChessPiece::ChessPiece(int x, int y, pieceTypes::PieceType type) {
	this->x = x;
	this->y = y;
	this->type = type;
	this->texture = nullptr;
	this->isWhite = true;
}

ChessPiece::ChessPiece(int x, int y, pieceTypes::PieceType type, bool isWhite) {
	this->x = x;
	this->y = y;
	this->type = type;
	this->texture = nullptr;
	this->isWhite = isWhite;
}

pieceTypes::PieceType ChessPiece::getType() {
	return type;
}

void ChessPiece::setType(pieceTypes::PieceType newType) {
	type = newType;
}

SDL_Texture* ChessPiece::getTexture() {
	return texture;
}
void ChessPiece::setTexture(SDL_Texture* newTexture) {
	texture = newTexture;
}