/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 19:25:28 by khirsig           #+#    #+#             */
/*   Updated: 2022/06/07 00:00:39 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Data.hpp"
#include <iostream>

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
		ChessPiece temp;
		temp.owner = WHITE_P;
		temp.type = PAWN;
		data.whitePiece.push_back(temp);
		data.square[6][i].piece = &data.whitePiece.back();
		temp.owner = BLACK_P;
		temp.type = PAWN;
		data.blackPiece.push_back(temp);
		data.square[1][i].piece = &data.blackPiece.back();
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
	for (int i = 0; i < 2; ++i)
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

				pos.x = x * size;
				pos.y = y * size;
				scale = (float)size / 1280;
				if (data.square[y][x].piece->type == PAWN)
				{
					if (data.square[y][x].piece->owner == WHITE_P)
					{
						DrawTextureEx(data.tex[0], pos, 0, scale, WHITE);
						std::cout << "WHITE: " << x << "   " << y << "   " << data.square[y][x].piece->owner << "   " << data.square[y][x].piece->type << std::endl;
					}
					else
					{
						DrawTextureEx(data.tex[1], pos, 0, scale, WHITE);
						std::cout << "BLACK: " << x << "   " << y << "   " << data.square[y][x].piece->owner << "   " << data.square[y][x].piece->type << std::endl;
					}
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
	SetTargetFPS(30);
	initTex(data);
	initPieces(data);
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(RAYWHITE);
		drawBoard(data);
		drawPiece(data);
		EndDrawing();
	}
	CloseWindow();
}
