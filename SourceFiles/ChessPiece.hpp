#pragma once

#include "Board.hpp"
#include <SDL_image.h>
#include "PieceType.hpp"

class ChessPiece {
	public:
		int x;
		int y;
		bool isWhite;
		bool drawn = false;
		bool selected = false;
		bool moved = false;

		ChessPiece(int x, int y, pieceTypes::PieceType type) {
			this->x = x;
			this->y = y;
			this->type = type;
			this->texture = nullptr;
			this->isWhite = true;
		}

		ChessPiece(int x, int y, pieceTypes::PieceType type, bool isWhite) {
			this->x = x;
			this->y = y;
			this->type = type;
			this->texture = nullptr;
			this->isWhite = isWhite;
		}

		pieceTypes::PieceType getType() {
			return type;
		}

		void setType(pieceTypes::PieceType newType) {
			type = newType;
		}

		SDL_Texture* getTexture() {
			return texture;
		}
		void setTexture(SDL_Texture* newTexture) {
			texture = newTexture;
		}

private:
	SDL_Texture* texture;
	pieceTypes::PieceType type;
};