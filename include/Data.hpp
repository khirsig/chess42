/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 19:27:45 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/29 14:57:31 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_HPP
# define DATA_HPP

# include <vector>
# include <thread>
# include <future>
# include <cstdlib>
# include <iostream>
# include <chrono>
# include <mutex>
# include <sstream>
# include <iomanip>
# include "../extern/raylib-cpp/include/raylib-cpp.hpp"
# include "./Board.hpp"
# include "./engine.hpp"
# include "../config.hpp"

# define CHECK 1
# define MATE 2

# define KING_NORMAL 0
# define KING_SHORT_CASTLE 1
# define KING_LONG_CASTLE 2

static bool exitBarDepth = false;

struct Data {
	Board								chessBoard;
	raylib::Color						primaryColor = PRIMARY_COLOR;
	raylib::Color						secondaryColor = SECONDARY_COLOR;
	std::vector<raylib::Texture2D>		tex;
	int									moveNbr = -1;
	ChessPiece							*grabbedPiece = nullptr;
	int									grabbedPiecePosX;
	int									grabbedPiecePosY;
	bool								PlayerHasCastled[2];
	BoardSquare							*lastMove[2] = { nullptr, nullptr };
	int									turn = WHITE_P;
	int									waitAI = 0;
	int									currentDepth;
	int									depth;
	bool								aiThinking = false;
	std::future<Move>					aiThreadMove;
	std::future<Move>					barThreadMove;
	std::future<float>					evaluation;
	Board								*barBoard = nullptr;
	int									currentBarDepth = 5;
	int									nextBarDepth = 6;
	float								barEval;
};

void	placePiece(Data &data, int player);
void	grabPiece(Data &data);

void	drawBoard(Data &data);
void	drawAllPieces(Data &data);
void	drawDebugInfo(Data &data);

void	initTex(Data &data);
void	initPieces(Data &data);

bool	isMovePossible(Board &chessBoard, int pieceX, int pieceY, int xAdd, int yAdd, bool lookForCheck);
void	toggleCheckBothPlayers(Board &chessBoard);
bool	lookForCheckmate(Board &chessBoard);
bool	possibleMoveCheck(Board &chessBoard, int pieceX, int pieceY, int targetX, int targetY);
bool	lookForCheck(Board &chessBoard, int player);
bool	possibleMoveCheck(Board &chessBoard, int pieceX, int pieceY, int targetX, int targetY);

float	getPieceSquareValue(ChessPiece *piece, int x, int y, bool endgame);
float	getOppositeSquare(int i);

ChessPiece	*movePiece(Board &chessBoard, int pieceX, int pieceY, int targetX, int targetY);
void		revertMovePiece(Board &chessBoard, int pieceX, int pieceY, int targetX, int targetY, ChessPiece *deletedPiece, int player);

Board	*copyBoard(Board &chessBoard);

void	getBestAIMove(Data &data, Board &chessBoard, int player);
void	executeAIMove(Data &data, Board &chessBoard, int player);
Move	lookForMoves(Board *chessBoard, int player, int depth, bool relative);

void	drawEvalBar(Data &data, Board &chessBoard);
void	runEvalBar(Data &data, Board &chessBoard);
float	calculateBoard(Board &chessBoard, int player, bool relative);

#endif