/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 19:27:45 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/25 08:23:43 by khirsig          ###   ########.fr       */
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


# define SCREEN_WIDTH 1200
# define SCREEN_HEIGHT 1200

# define CHECK 1
# define MATE 2

# define KING_NORMAL 0
# define KING_SHORT_CASTLE 1
# define KING_LONG_CASTLE 2

static bool exitBarDepth = false;

struct Data {
	Board								chessBoard;
	raylib::Color						primaryColor = RAYWHITE;
	raylib::Color						secondaryColor = BLUE;
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
	int									barTurn = WHITE_P;
};

bool	isMovePossible(Board &chessBoard, int pieceX, int pieceY, int xAdd, int yAdd, bool lookForCheck);
void	toggleCheckBothPlayers(Board &chessBoard);
bool	lookForCheckmate(Board &chessBoard);
bool	possibleMoveCheck(Board &chessBoard, int pieceX, int pieceY, int targetX, int targetY);

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