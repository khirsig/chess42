/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 19:27:45 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/19 15:36:42 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_HPP
# define DATA_HPP

# include <vector>
# include <cstdlib>
# include <iostream>
# include "../extern/raylib-cpp/include/raylib-cpp.hpp"
# include "./Board.hpp"
# include "History.hpp"
# include "./engine.hpp"


# define SCREEN_WIDTH 1200
# define SCREEN_HEIGHT 1200

# define CHECK 1
# define MATE 2

# define KING_NORMAL 0
# define KING_SHORT_CASTLE 1
# define KING_LONG_CASTLE 2

struct Data {
	Board								chessBoard;
	raylib::Color						primaryColor = RAYWHITE;
	raylib::Color						secondaryColor = DARKGREEN;
	std::vector<raylib::Texture2D>		tex;
	std::vector<History>				history;
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
};

bool	isMovePossible(Board &chessBoard, int pieceX, int pieceY, int xAdd, int yAdd, bool lookForCheck);
void	moveAI(Data &data, Board &chessBoard, int player);
void	toggleCheckBothPlayers(Board &chessBoard);
bool	lookForCheckmate(Board &chessBoard);
bool	possibleMoveCheck(Board &chessBoard, int pieceX, int pieceY, int targetX, int targetY);
float	calculateBoard(Board &chessBoard, int player, float factor);


#endif