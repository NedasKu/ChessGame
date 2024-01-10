#pragma once

// SDL imports
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

// System imports
#include <stdio.h>
#include <iostream>
#include <filesystem>

// Game imports
#include "Board.hpp"
#include "ChessPiece.hpp"
#include "ChessAI.hpp"

//Screen dimension constants

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

const int HEIGHT_PADDING = 50;
const int EXTENDED_SCREEN_HEIGHT = SCREEN_HEIGHT + 2 * HEIGHT_PADDING;

const int WIDTH_PADDING = 0;
const int EXTENDED_SCREEN_WIDTH = SCREEN_HEIGHT + 2 * WIDTH_PADDING;

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
const int TILE_HEIGHT = SCREEN_HEIGHT / 8;
const int TILE_WIDTH = SCREEN_WIDTH / 8;

bool selected = false;
int selectedPieceX = -1;
int selectedPieceY = -1;

std::filesystem::path resourceDir = std::filesystem::current_path() / "ResourceFiles";
std::filesystem::path imagesDir = resourceDir / "Images";
std::filesystem::path soundsDir = resourceDir / "Sounds";
//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* renderer = NULL;
bool isWhiteTurn = true;
bool playerIsWhite = true;
const int DEFAULT_SOUND_FREQ = 48000;
const int AUDIO_CHANNELS = 1;

SDL_Texture* whitePawnImg;
SDL_Texture* whiteRookImg;
SDL_Texture* whiteKingImg;
SDL_Texture* whiteQueenImg;
SDL_Texture* whiteBishopImg;
SDL_Texture* whiteKnightImg;
SDL_Texture* blackPawnImg;
SDL_Texture* blackRookImg;
SDL_Texture* blackKingImg;
SDL_Texture* blackQueenImg;
SDL_Texture* blackBishopImg;
SDL_Texture* blackKnightImg;
SDL_Texture* potentialMoveImg;

Mix_Music* captureSound;
Mix_Music* castleSound;
Mix_Music* checkSound;
Mix_Music* moveSound;
Mix_Music* promoteSound;


int initSDL() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}
	return 0;
}

int createWindow(SDL_Window*& window) {
	window = SDL_CreateWindow("Chess Game", 100, 100, SCREEN_WIDTH, EXTENDED_SCREEN_HEIGHT, 0);
	if (window == nullptr) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}
	return 0;
}

class RGB {
public:
	int r;
	int g;
	int b;
	RGB(int r, int g, int b) {
		this->r = r;
		this->g = g;
		this->b = b;
	}

	RGB() {
		this->r = 0;
		this->g = 0;
		this->b = 0;
	}
};

RGB backgroundColor = RGB(0, 0, 0);

void cleanUp(SDL_Window* window) {
	//Destroy window
	SDL_DestroyWindow(window);
	//Quit SDL subsystems
	SDL_Quit();
}

void drawRect(int x, int y, int w, int h, SDL_Renderer*& renderer, RGB rect_colors) {
	// Redraw the rectangle;
	SDL_Rect rect;
	rect.x = x;
	rect.y = y + HEIGHT_PADDING;
	rect.h = h;
	rect.w = w;
	SDL_SetRenderDrawColor(renderer, rect_colors.r, rect_colors.g, rect_colors.b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &rect);
}

void drawPiece(SDL_Renderer*& renderer, ChessPiece* piece, Board* board) {
	SDL_Rect rect;
	rect.x = piece->x * TILE_WIDTH;
	rect.y = HEIGHT_PADDING + piece->y * TILE_HEIGHT;
	rect.h = TILE_WIDTH;
	rect.w = TILE_HEIGHT;
	SDL_RenderCopy(renderer, piece->getTexture(), nullptr, &rect);
}

void playSound() {
	//Mix_OpenAudio(DEFAULT_SOUND_FREQ, AUDIO_F32SYS, AUDIO_CHANNELS);
}

void highLightPiecesAndMoves(SDL_Renderer*& renderer, ChessPiece* piece, Board* board) {
	RGB orange = RGB(255, 165, 0);
	RGB red = RGB(240, 0, 0);
	if (piece->selected && piece->isWhite == isWhiteTurn) {
		auto potentialMoves = board->getMoves(piece);
		for (auto it : potentialMoves) {
			if (board->chessBoard[it.first][it.second]->getType() != pieceTypes::PieceType::None) {
				drawRect(it.first * TILE_WIDTH, it.second * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, renderer, orange);
			}
			else {
				SDL_Rect rect;
				rect.x = it.first * TILE_WIDTH;
				rect.y = HEIGHT_PADDING + it.second * TILE_HEIGHT;
				rect.h = TILE_WIDTH;
				rect.w = TILE_HEIGHT;
				SDL_RenderCopy(renderer, potentialMoveImg, nullptr, &rect);
			}
		}
	}

	if (piece->getType() == pieceTypes::PieceType::King) {
		if (static_cast<King*>(piece)->isWhite && board->whiteInCheck) {
			drawRect(piece->x * TILE_WIDTH, piece->y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, renderer, red);
		}

		if (!static_cast<King*>(piece)->isWhite && board->blackInCheck) {
			drawRect(piece->x * TILE_WIDTH, piece->y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, renderer, red);
		}
	}
}



void drawChessBoard(SDL_Renderer*& renderer, Board* chessBoard) {
	RGB black = RGB(40, 40, 40);
	RGB white = RGB(211, 211, 211);

	bool drawWhite = true;
	// Draw board in layers, 
	// 1st layer, initial checkered board, 
	// 2nd layer, any highlighting of board
	// 3rd layer, all of the pieces

	// draw base
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (drawWhite) {
				drawRect(x * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, renderer, white);
			}
			else {
				drawRect(x * TILE_WIDTH, y * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT, renderer, black);
			}
			drawWhite = !drawWhite;
		}
		drawWhite = !drawWhite;
	}

	// draw highlighting
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			highLightPiecesAndMoves(renderer, chessBoard->chessBoard[x][y], chessBoard);
		}
	}

	// draw pieces
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (chessBoard->chessBoard[x][y]->getType() != pieceTypes::PieceType::None) {
				drawPiece(renderer, chessBoard->chessBoard[x][y], chessBoard);
			}
		}
	}
}


void drawBackground(SDL_Renderer* renderer) {
	// Redraw the rectangle;
	SDL_Rect rect;

	rect.x = 0;
	rect.y = 0;
	rect.h = EXTENDED_SCREEN_HEIGHT;
	rect.w = EXTENDED_SCREEN_WIDTH;
	SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &rect);
}

bool processMove(int selectedX, int selectedY, Board* board) {
	auto chessBoard = board->chessBoard;
	bool soundPlayed = false;
	// There should be 3 scenarios, 
	// 1. clicked on a piece that has valid moves, select it
	// 2. clicked on a valid move for a selected piece, move it
	//  2.1 special move - castling, need to move 2 pieces
	//	2.2 special move - promotion, need to change pawn
	// 3. clicked on a non-valid move for selected piece, de-select it
	// Scenario when a new piece is selected
	if (!selected) {
		// check if the turn matches the piece
		if (chessBoard[selectedX][selectedY]->isWhite == isWhiteTurn) {
			// check has valid moves
			auto validMoves = board->getMoves(chessBoard[selectedX][selectedY]);
			if (validMoves.size() > 0) {
				selected = true;
				selectedPieceX = selectedX;
				selectedPieceY = selectedY;
				chessBoard[selectedX][selectedY]->selected = true;
			}
		}
	}
	else {
		// Two scenarios if the piece is selected:
		// move it if we selected a new move that is valid for the piece
		// deselect it otherwise.
		auto validMoves = board->getMoves(chessBoard[selectedPieceX][selectedPieceY]);
		bool shouldMove = false;
		for (auto move : validMoves) {
			if (move.first == selectedX && move.second == selectedY) {
				shouldMove = true;
				break;
			}
		}
		// move piece into the new spot, reset selection status for it
		if (shouldMove) {
			// change pawn to queen
			if (chessBoard[selectedPieceX][selectedPieceY]->getType() == pieceTypes::PieceType::Pawn) {
				if (selectedY == 7 || selectedY == 0) {
					// TODO: implement selection of piece, instead of queening automatically
					chessBoard[selectedPieceX][selectedPieceY]->setType(pieceTypes::PieceType::Queen);
					if (chessBoard[selectedPieceX][selectedPieceY]->isWhite) {
						chessBoard[selectedPieceX][selectedPieceY]->setTexture(whiteQueenImg);
					}
					else {
						chessBoard[selectedPieceX][selectedPieceY]->setTexture(blackQueenImg);
					}
					Mix_PlayMusic(promoteSound, 1);
					soundPlayed = true;
				}
			}

			board->movePieceTo(chessBoard[selectedPieceX][selectedPieceY], selectedX, selectedY);
			if (!soundPlayed) {
				Mix_PlayMusic(moveSound, 1);
			}
			// change the turn
			isWhiteTurn = !isWhiteTurn;
			return true;
		}
		// clear status of selection should be perfomed regardless
		chessBoard[selectedPieceX][selectedPieceY]->selected = false;
		selectedPieceX = -1;
		selectedPieceY = -1;
		selected = false;
	}
	return false;
}


Board* initNewBoard(SDL_Renderer* renderer) {
	whitePawnImg = IMG_LoadTexture(renderer, (imagesDir / "pawn-w.png").string().c_str());
	whiteRookImg = IMG_LoadTexture(renderer, (imagesDir / "rook-w.png").string().c_str());
	whiteKingImg = IMG_LoadTexture(renderer, (imagesDir / "king-w.png").string().c_str());
	whiteQueenImg = IMG_LoadTexture(renderer, (imagesDir / "queen-w.png").string().c_str());
	whiteBishopImg = IMG_LoadTexture(renderer, (imagesDir / "bishop-w.png").string().c_str());
	whiteKnightImg = IMG_LoadTexture(renderer, (imagesDir / "knigth-w.png").string().c_str());
	blackPawnImg = IMG_LoadTexture(renderer, (imagesDir / "pawn-b.png").string().c_str());
	blackRookImg = IMG_LoadTexture(renderer, (imagesDir / "rook-b.png").string().c_str());
	blackKingImg = IMG_LoadTexture(renderer, (imagesDir / "king-b.png").string().c_str());
	blackQueenImg = IMG_LoadTexture(renderer, (imagesDir / "queen-b.png").string().c_str());
	blackBishopImg = IMG_LoadTexture(renderer, (imagesDir / "bishop-b.png").string().c_str());
	blackKnightImg = IMG_LoadTexture(renderer, (imagesDir / "knigth-b.png").string().c_str());
	potentialMoveImg = IMG_LoadTexture(renderer, (imagesDir / "potential-move.png").string().c_str());


	captureSound = Mix_LoadMUS((soundsDir / "capture.mp3").string().c_str());
	castleSound = Mix_LoadMUS((soundsDir / "castle.mp3").string().c_str());;
	checkSound = Mix_LoadMUS((soundsDir / "move-check.mp3").string().c_str());;
	moveSound = Mix_LoadMUS((soundsDir / "move-self.mp3").string().c_str());;
	promoteSound = Mix_LoadMUS((soundsDir / "promote.mp3").string().c_str());;


	Board* chessBoard = new Board();

	for (int i = 0; i < 8; i++) {
		chessBoard->chessBoard[i][1] = new Pawn(i, 1, blackPawnImg, false);
	}

	for (int i = 0; i < 8; i++) {
		chessBoard->chessBoard[i][6] = new Pawn(i, 6, whitePawnImg, true);
	}

	chessBoard->chessBoard[0][0] = new Rook(0, 0, blackRookImg, false);
	chessBoard->chessBoard[1][0] = new Knight(1, 0, blackKnightImg, false);
	chessBoard->chessBoard[2][0] = new Bishop(2, 0, blackBishopImg, false);
	chessBoard->chessBoard[3][0] = new Queen(3, 0, blackQueenImg, false);
	chessBoard->chessBoard[4][0] = new King(4, 0, blackKingImg, false);
	chessBoard->chessBoard[5][0] = new Bishop(5, 0, blackBishopImg, false);
	chessBoard->chessBoard[6][0] = new Knight(6, 0, blackKnightImg, false);
	chessBoard->chessBoard[7][0] = new Rook(7, 0, blackRookImg, false);

	chessBoard->chessBoard[0][7] = new Rook(0, 7, whiteRookImg, true);
	chessBoard->chessBoard[1][7] = new Knight(1, 7, whiteKnightImg, true);
	chessBoard->chessBoard[2][7] = new Bishop(2, 7, whiteBishopImg, true);
	chessBoard->chessBoard[3][7] = new Queen(3, 7, whiteQueenImg, true);
	chessBoard->chessBoard[4][7] = new King(4, 7, whiteKingImg, true);
	chessBoard->chessBoard[5][7] = new Bishop(5, 7, whiteBishopImg, true);
	chessBoard->chessBoard[6][7] = new Knight(6, 7, whiteKnightImg, true);
	chessBoard->chessBoard[7][7] = new Rook(7, 7, whiteRookImg, true);

	return chessBoard;
}


int main(int argc, char* args[])
{
	srand(time(0));

	// The window we'll be rendering to
	SDL_Window* window = nullptr;

	// The surface contained by the window
	if (initSDL() != 0) {
		return -1;
	}
	Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 640);


	if (createWindow(window) != 0) {
		return -1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

	// Clear window
	SDL_RenderClear(renderer);
	SDL_Event e;
	auto chessBoard = initNewBoard(renderer);

	ChessAI* chessAi = new ChessAI(!playerIsWhite);
	ChessAI* chessAi2 = new ChessAI(playerIsWhite);

	// Create the board
	bool quit = false;
	bool moveMade = false;
	while (quit == false) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				if ((playerIsWhite == isWhiteTurn) && e.button.button == SDL_BUTTON_LEFT) {
					int selectedX = e.button.x / TILE_WIDTH;
					int selectedY = (e.button.y - HEIGHT_PADDING) / TILE_HEIGHT;
					moveMade = processMove(selectedX, selectedY, chessBoard);
				}
			}
			if (e.type == SDL_QUIT) {
				quit = true;
			};
		}

		if (!playerIsWhite == isWhiteTurn && moveMade == false) {
			chessAi->playAIMove(chessBoard);
			moveMade = true;
			Mix_PlayMusic(moveSound, 1);
			isWhiteTurn = !isWhiteTurn;
		}
		else {
			//chessAi2->playAIMove(chessBoard);
			//moveMade = true;
			//Mix_PlayMusic(moveSound, 1);
			//isWhiteTurn = !isWhiteTurn;
		}

		// Clear previous render
		SDL_RenderClear(renderer);
		drawBackground(renderer);
		drawChessBoard(renderer, chessBoard);

		// Flip render onto the screen
		SDL_RenderPresent(renderer);
		SDL_Delay(SCREEN_TICKS_PER_FRAME);

		if (moveMade) {
			// if move was made, check king states
			if (chessBoard->checkKingStates()) {
				Mix_PlayMusic(checkSound, 1);
			}

			// check for conclusions of the game;
			if (chessBoard->whiteLost()) {
				std::cout << "Black wins" << std::endl;
				SDL_Delay(50000);
				return 0;
				// display black won window
			}

			if (chessBoard->blackLost()) {
				std::cout << "White wins" << std::endl;
				SDL_Delay(50000);
				return 0;
				// display white won window
			}

			if (chessBoard->draw(isWhiteTurn)) {
				std::cout << "Game ended in a draw" << std::endl;
				SDL_Delay(50000);
				return 0;
				// display draw state window
			}
		}
		if (moveMade) {
			std::cout << "moves made: " << std::endl;
			for (auto it : chessBoard->movesMade) {
				std::cout << it->toString() << std::endl;
			}
		}

		moveMade = false;
	}

	SDL_DestroyRenderer(renderer);
	cleanUp(window);

	return 0;
}
