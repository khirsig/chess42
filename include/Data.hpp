/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 19:27:45 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/06 09:51:38 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_HPP
# define DATA_HPP

# include <vector>
# include <cstdlib>
# include <iostream>
# include "../raylib-cpp/include/raylib-cpp.hpp"
# include "./BoardSquare.hpp"
# include "History.hpp"

# define SCREEN_WIDTH 600
# define SCREEN_HEIGHT 600

# define CHECK 1
# define MATE 2

# define KING_NORMAL 0
# define KING_SHORT_CASTLE 1
# define KING_LONG_CASTLE 2

struct Data {
	BoardSquare							square[8][8];
	raylib::Color						primaryColor = RAYWHITE;
	raylib::Color						secondaryColor = BLUE;
	std::vector<raylib::Texture2D>		tex;
	std::vector<History>				history;
	int									moveNbr = -1;
	ChessPiece							*grabbedPiece = nullptr;
	int									grabbedPiecePosX;
	int									grabbedPiecePosY;
	bool								PlayerHasCastled[2];
	int									kingPosX[2];
	int									kingPosY[2];
	bool								kingCheck[2] = { false, false };
	BoardSquare							*lastMove[2] = { nullptr, nullptr };
	int									turn = WHITE_P;
	int									waitAI = 0;
	int									currentDepth;
	int									depth;
};

bool	isMovePossible(Data &data, BoardSquare currentSquare[8][8], ChessPiece *piece, int pieceX, int pieceY, int xAdd, int yAdd, bool lookForCheck);
void	moveAI(Data &data, BoardSquare currentBoard[8][8], int player);

#endif