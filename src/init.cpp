/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 10:29:22 by khirsig           #+#    #+#             */
/*   Updated: 2022/08/05 15:10:48 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Data.hpp"

void	initPieces(Data &data)
{
	// First sets all chess piece pointers to nullptr.
	for (int x = 0; x < 8; ++x)
	{
		for (int y = 0; y < 8; ++y)
		{
			data.chessBoard.square[x][y].piece = nullptr;
		}
	}
	// Iterates through all the x fields and puts the right pieces for both players on the board.
	// We always know the right y value, so we only need one loop
	for (int i = 0; i < 8; ++i)
	{
		data.chessBoard.square[6][i].piece = new ChessPiece(WHITE_P, PAWN, 100);
		data.chessBoard.square[1][i].piece = new ChessPiece(BLACK_P, PAWN, 100);
		if (i == 1 || i == 6)
		{
			data.chessBoard.square[7][i].piece = new ChessPiece(WHITE_P, KNIGHT, 320);
			data.chessBoard.square[0][i].piece = new ChessPiece(BLACK_P, KNIGHT, 320);
		}
		if (i == 2 || i == 5)
		{
			data.chessBoard.square[7][i].piece = new ChessPiece(WHITE_P, BISHOP, 330);
			data.chessBoard.square[0][i].piece = new ChessPiece(BLACK_P, BISHOP, 330);
		}
		if (i == 0 || i == 7)
		{
			data.chessBoard.square[7][i].piece = new ChessPiece(WHITE_P, ROOK, 500);
			data.chessBoard.square[0][i].piece = new ChessPiece(BLACK_P, ROOK, 500);
		}
		if (i == 3)
		{
			data.chessBoard.square[7][i].piece = new ChessPiece(WHITE_P, QUEEN, 900);
			data.chessBoard.square[0][i].piece = new ChessPiece(BLACK_P, QUEEN, 900);
		}
		if (i == 4)
		{
			data.chessBoard.square[7][i].piece = new ChessPiece(WHITE_P, KING, 20000);
			data.chessBoard.kingPosX[WHITE_P] = i;
			data.chessBoard.kingPosY[WHITE_P] = 7;
			data.chessBoard.square[0][i].piece = new ChessPiece(BLACK_P, KING, 20000);
			data.chessBoard.kingPosX[BLACK_P] = i;
			data.chessBoard.kingPosY[BLACK_P] = 0;
		}
	}
}

std::string	getTexPath(int i)
{
	// Puts together the correct path to the piece graphics.
	std::string texLink = "./resources/pieces/piece";

	if (i < 10)
	{
		texLink += "0";
		texLink += std::to_string(i);
		texLink += ".png";
	}
	else
	{
		texLink += std::to_string(i);
		texLink += ".png";
	}
	return (texLink);
}

void	initTex(Data &data)
{
	// Fills the texture vector with all the piece textures.
	for (int i = 0; i < 12; ++i)
	{
		data.tex.push_back(raylib::Texture(getTexPath(i)));
	}
}
