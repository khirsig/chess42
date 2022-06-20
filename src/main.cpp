/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 19:25:28 by khirsig           #+#    #+#             */
/*   Updated: 2022/06/20 16:15:49 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Data.hpp"
#include <iostream>

bool	pawnMove(Data &data, int xAdd, int yAdd)
{
	if ((yAdd > 0 && data.grabbedPiece->getOwner() == WHITE_P) || (yAdd < 0 && data.grabbedPiece->getOwner() == BLACK_P))
		return (false);
	if (yAdd > 2 || yAdd < -2)
		return (false);
	if ((yAdd > 1 && data.grabbedPiecePosY != 1) || (yAdd < -1 && data.grabbedPiecePosY != 6))
			return (false);
	if (xAdd != 0 && yAdd == 0)
		return (false);
	if (xAdd > 1 || xAdd < -1)
		return (false);

	int	incr = 0;
	if (yAdd > 0)
		incr = 1;
	else if (yAdd < 0)
		incr = -1;
	int i = incr;

	while ((incr == 1 && i <= yAdd) || (incr == -1 && i >= yAdd))
	{
		if (data.square[data.grabbedPiecePosY + i][data.grabbedPiecePosX + xAdd].piece != nullptr && xAdd == 0)
			return (false);
		if (data.square[data.grabbedPiecePosY + i][data.grabbedPiecePosX + xAdd].piece == nullptr && xAdd != 0)
			return (false);
		i += incr;
	}
	return (true);
}

bool	isMovePossible(Data &data, int xAdd, int yAdd)
{
	if (xAdd == 0 && yAdd == 0)
		return (false);
	switch (data.grabbedPiece->getType()) {
		case PAWN :
				if (pawnMove(data, xAdd, yAdd))
					return (true);
			break;
	}
	return (false);
}

void	placePiece(Data &data)
{
	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && data.grabbedPiece != nullptr)
	{
		int size = SCREEN_WIDTH / 8;
		int x = GetMouseX();
		int y = GetMouseY();

		x /= size;
		y /= size;
		if (isMovePossible(data, x - data.grabbedPiecePosX, y - data.grabbedPiecePosY))
		{
			data.square[data.grabbedPiecePosY][data.grabbedPiecePosX].piece = nullptr;
				data.square[y][x].piece = data.grabbedPiece;
		}
		data.grabbedPiece->setGrabbed(false);
		data.grabbedPiece = nullptr;
		data.grabbedPiecePosX = -1;
		data.grabbedPiecePosY = -1;
	}

}
void	grabPiece(Data &data)
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && data.grabbedPiece == nullptr)
	{
		int size = SCREEN_WIDTH / 8;
		int x = GetMouseX();
		int y = GetMouseY();

		x /= size;
		y /= size;
		if (data.square[y][x].piece != nullptr)
		{
			data.grabbedPiece = data.square[y][x].piece;
			data.square[y][x].piece->setGrabbed(true);
			data.grabbedPiecePosX = x;
			data.grabbedPiecePosY = y;
		}
	}
}

void	initPieces(Data &data)
{
	for (int x = 0; x < 8; ++x)
	{
		for (int y = 0; y < 8; ++y)
		{
			data.square[x][y].piece = nullptr;
		}
	}
	for (int i = 0; i < 8; ++i)
	{
		data.square[6][i].piece = new ChessPiece(WHITE_P, PAWN);
		data.square[1][i].piece = new ChessPiece(BLACK_P, PAWN);
		if (i == 1 || i == 6)
		{
			data.square[7][i].piece = new ChessPiece(WHITE_P, KNIGHT);
			data.square[0][i].piece = new ChessPiece(BLACK_P, KNIGHT);
		}
		if (i == 2 || i == 5)
		{
			data.square[7][i].piece = new ChessPiece(WHITE_P, BISHOP);
			data.square[0][i].piece = new ChessPiece(BLACK_P, BISHOP);
		}
		if (i == 0 || i == 7)
		{
			data.square[7][i].piece = new ChessPiece(WHITE_P, ROOK);
			data.square[0][i].piece = new ChessPiece(BLACK_P, ROOK);
		}
		if (i == 3)
		{
			data.square[7][i].piece = new ChessPiece(WHITE_P, QUEEN);
			data.square[0][i].piece = new ChessPiece(BLACK_P, QUEEN);
		}
		if (i == 4)
		{
			data.square[7][i].piece = new ChessPiece(WHITE_P, KING);
			data.square[0][i].piece = new ChessPiece(BLACK_P, KING);
		}
	}
}

std::string	getTexPath(int i)
{
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
	for (int i = 0; i < 12; ++i)
	{
		data.tex.push_back(raylib::Texture(getTexPath(i)));
	}
}

void	drawPiece(Data &data)
{
	int size = SCREEN_WIDTH / 8;

	for (int x = 0; x < 8; ++x)
	{
		for (int y = 0; y < 8; ++y)
		{
			if (data.square[y][x].piece != nullptr)
			{
				raylib::Vector2 pos;
				float			scale;

				int	pieceType = data.square[y][x].piece->getType();
				int pieceOwner = data.square[y][x].piece->getOwner();
				if (data.square[y][x].piece->getGrabbed())
				{
					pos.x = GetMouseX() - size / 2;
					pos.y = GetMouseY() - size / 2;
				}
				else
				{
					pos.x = x * size;
					pos.y = y * size;
				}
				scale = (float)size / 1280;
				switch (pieceType) {
					case PAWN :
						if (pieceOwner == WHITE_P)
							DrawTextureEx(data.tex[0], pos, 0, scale, WHITE);
						else
							DrawTextureEx(data.tex[1], pos, 0, scale, WHITE);
						break ;
					case BISHOP :
						if (pieceOwner == WHITE_P)
							DrawTextureEx(data.tex[2], pos, 0, scale, WHITE);
						else
							DrawTextureEx(data.tex[3], pos, 0, scale, WHITE);
						break ;
					case KNIGHT :
						if (pieceOwner == WHITE_P)
							DrawTextureEx(data.tex[4], pos, 0, scale, WHITE);
						else
							DrawTextureEx(data.tex[5], pos, 0, scale, WHITE);
						break ;
					case ROOK :
						if (pieceOwner == WHITE_P)
							DrawTextureEx(data.tex[6], pos, 0, scale, WHITE);
						else
							DrawTextureEx(data.tex[7], pos, 0, scale, WHITE);
						break ;
					case QUEEN :
						if (pieceOwner == WHITE_P)
							DrawTextureEx(data.tex[8], pos, 0, scale, WHITE);
						else
							DrawTextureEx(data.tex[9], pos, 0, scale, WHITE);
						break ;
					case KING :
						if (pieceOwner == WHITE_P)
							DrawTextureEx(data.tex[10], pos, 0, scale, WHITE);
						else
							DrawTextureEx(data.tex[11], pos, 0, scale, WHITE);
						break ;
				}
			}
		}
	}
}

void	drawBoard(Data &data)
{
	int size = SCREEN_WIDTH / 8;

	for (int x = 0; x < 8; ++x)
	{
		for (int y = 0; y < 8; ++y)
		{
			if (y % 2 == 0)
			{
				if (x % 2 == 0)
					DrawRectangle(size * x, size * y, size, size, data.primaryColor);
				else
					DrawRectangle(size * x, size * y, size, size, data.secondaryColor);
			}
			else
			{
				if (x % 2 == 0)
					DrawRectangle(size * x, size * y, size, size, data.secondaryColor);
				else
					DrawRectangle(size * x, size * y, size, size, data.primaryColor);
			}
		}
	}
}

int	main()
{
	Data data;

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess");
	SetTargetFPS(120);
	initTex(data);
	initPieces(data);
	while (!WindowShouldClose())
	{
		BeginDrawing();
		// ACTIONS
		grabPiece(data);
		placePiece(data);
		// DRAWING
		ClearBackground(RAYWHITE);
		drawBoard(data);
		drawPiece(data);
		EndDrawing();
	}
	CloseWindow();
}
