/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 19:25:28 by khirsig           #+#    #+#             */
/*   Updated: 2022/06/24 10:41:37 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Data.hpp"

int	kingMove(Data &data, ChessPiece *piece, int pieceX, int pieceY, int xAdd, int yAdd)
{
	if (abs(xAdd) > 0 && yAdd == 0
		&& data.square[pieceY][pieceX + xAdd].piece != nullptr
		&& data.square[pieceY][pieceX + xAdd].piece->getType() == ROOK
		&& !data.square[pieceY][pieceX + xAdd].piece->getHasMoved()
		&& !piece->getHasMoved())
	{
		int incr;
		if (xAdd > 0)
			incr = 1;
		else
			incr = -1;
		int x = incr;
		while ((incr == 1 && x <= xAdd) || (incr == -1 && x >= xAdd))
		{
			if (data.square[pieceY][pieceX + x].piece != nullptr && x != xAdd)
				return (-1);
			if (data.square[pieceY][pieceX + x].piece != nullptr && x == xAdd)
			{
				if (pieceX + x == 0)
					return (KING_LONG_CASTLE);
				else if (pieceX + x == 7)
					return (KING_SHORT_CASTLE);
				return (-1);
			}
			x += incr;
		}
	}
	if (abs(xAdd) > 1 || abs(yAdd) > 1)
		return (-1);
	if (abs(xAdd) != 1 && abs(yAdd) != 1)
		return (-1);
	return (KING_NORMAL);
}

bool	rookMove(Data &data, int pieceX, int pieceY, int xAdd, int yAdd)
{
	if (xAdd != 0 && yAdd != 0)
		return (false);

	int	xIncr = 0;
	if (xAdd > 0)
		xIncr = 1;
	else if (xAdd < 0)
		xIncr = -1;
	int	yIncr = 0;
	if (yAdd > 0)
		yIncr = 1;
	else if (yAdd < 0)
		yIncr = -1;
	int x = xIncr;
	int y = yIncr;

	while (((yIncr == 1 && y <= yAdd) || (yIncr == -1 && y >= yAdd) || (yIncr == 0)) && ((xIncr == 1 && x <= xAdd) || (xIncr == -1 && x >= xAdd) || (xIncr == 0)))
	{
		if (data.square[pieceY + y][pieceX + x].piece != nullptr && (x != xAdd || y != yAdd))
			return (false);

		if ((xIncr == 1 && x <= xAdd) || (xIncr == -1 && x >= xAdd))
			x += xIncr;
		if ((yIncr == 1 && y <= yAdd) || (yIncr == -1 && y >= yAdd))
			y += yIncr;
	}
	return (true);
}

bool	knightMove(Data &data, int xAdd, int yAdd)
{
	if (!((abs(xAdd) == 2 && abs(yAdd) == 1) || (abs(xAdd) == 1 && abs(yAdd) == 2)))
		return (false);
	return (true);
}

bool	bishopMove(Data &data, int pieceX, int pieceY, int xAdd, int yAdd)
{
	if (xAdd == 0 || yAdd == 0)
		return (false);
	if (abs(xAdd) != abs(yAdd))
		return (false);

	int	xIncr = 0;
	if (xAdd > 0)
		xIncr = 1;
	else if (xAdd < 0)
		xIncr = -1;
	int	yIncr = 0;
	if (yAdd > 0)
		yIncr = 1;
	else if (yAdd < 0)
		yIncr = -1;
	int x = xIncr;
	int y = yIncr;

	while (((yIncr == 1 && y <= yAdd) || (yIncr == -1 && y >= yAdd) || (yIncr == 0)) && ((xIncr == 1 && x <= xAdd) || (xIncr == -1 && x >= xAdd) || (xIncr == 0)))
	{
		if (data.square[pieceY + y][pieceX + x].piece != nullptr && (x != xAdd || y != yAdd))
			return (false);

		if ((xIncr == 1 && x <= xAdd) || (xIncr == -1 && x >= xAdd))
			x += xIncr;
		if ((yIncr == 1 && y <= yAdd) || (yIncr == -1 && y >= yAdd))
			y += yIncr;
	}
	return (true);
}

bool	pawnMove(Data &data, ChessPiece *piece, int pieceX, int pieceY, int xAdd, int yAdd, bool lookForCheck)
{
	if ((yAdd > 0 && piece->getOwner() == WHITE_P) || (yAdd < 0 && piece->getOwner() == BLACK_P))
		return (false);
	if (yAdd > 2 || yAdd < -2)
		return (false);
	if ((yAdd > 1 && pieceY != 1) || (yAdd < -1 && pieceY != 6))
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
		if (data.square[pieceY + i][pieceX + xAdd].piece != nullptr && xAdd == 0)
			return (false);
		if (data.square[pieceY + i][pieceX + xAdd].piece == nullptr && xAdd != 0)
			return (false);
		i += incr;
	}
	if (abs(pieceX) == 0 && lookForCheck)
		return (false);
	return (true);
}

bool	isMovePossible(Data &data, ChessPiece *piece, int pieceX, int pieceY, int xAdd, int yAdd, bool lookForCheck)
{
	if (xAdd == 0 && yAdd == 0)
		return (false);
	switch (piece->getType()) {
		case PAWN :
			if (pawnMove(data, piece, pieceX, pieceY, xAdd, yAdd, lookForCheck))
				return (true);
			break ;
		case BISHOP :
			if (bishopMove(data, pieceX, pieceY, xAdd, yAdd))
				return (true);
			break ;
		case KNIGHT :
			if (knightMove(data, xAdd, yAdd))
				return (true);
			break;
		case ROOK :
			if (rookMove(data, pieceX, pieceY, xAdd, yAdd))
				return (true);
			break ;
		case QUEEN :
			if (bishopMove(data, pieceX, pieceY, xAdd, yAdd) || rookMove(data, pieceX, pieceY, xAdd, yAdd))
				return (true);
			break ;
		case KING :
			int ret = kingMove(data, piece, pieceX, pieceY, xAdd, yAdd);
			if (ret == KING_NORMAL)
				return (true);
			if (ret == KING_SHORT_CASTLE)
			{
				data.square[pieceY][pieceX + 1].piece = data.square[pieceY][pieceX + xAdd].piece;
				data.square[pieceY][pieceX + 1].piece->setHasMoved(true);
				data.square[pieceY][pieceX + xAdd].piece = nullptr;
				data.square[pieceY][pieceX + 2].piece = piece;
				data.square[pieceY][pieceX + 2].piece->setHasMoved(true);
				data.kingPosX[data.square[pieceY][pieceX + 2].piece->getOwner()] = pieceX + 2;
				data.square[pieceY][pieceX].piece = nullptr;
			}
			if (ret == KING_LONG_CASTLE)
			{
				data.square[pieceY][pieceX - 1].piece = data.square[pieceY][pieceX + xAdd].piece;
				data.square[pieceY][pieceX - 1].piece->setHasMoved(true);
				data.square[pieceY][pieceX + xAdd].piece = nullptr;
				data.square[pieceY][pieceX - 2].piece = piece;
				data.square[pieceY][pieceX - 2].piece->setHasMoved(true);
				data.kingPosX[data.square[pieceY][pieceX - 2].piece->getOwner()] = pieceX - 2;
				data.square[pieceY][pieceX].piece = nullptr;
			}
			return (false);
			break ;
	}
	return (false);
}

bool	lookForCheck(Data &data, int player)
{
	int otherPlayer;

	// Stores the other player than the owner of the player that could be in check.
	if (player == WHITE_P)
		otherPlayer = BLACK_P;
	else
		otherPlayer = WHITE_P;
	// Iterates through all squares to find all pieces.
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			// Only looks at pieces of the opposite player, because only they could give a check.
			if (data.square[y][x].piece && data.square[y][x].piece->getOwner() == otherPlayer)
			{
				// Looks if any of those pieces could make a theoretical move to the position of the king.
				// We need the piece that could give check and its position and also the x and y value it would need to make that move.
				// Last bool should be true because we're looking for a check, not moving.
				if (isMovePossible(data, data.square[y][x].piece, x, y, data.kingPosX[player] - x, data.kingPosY[player] - y, true))
					return (true);
			}
		}
	}
	// If no piece was able to move to the position of players king, return false.
	return (false);
}

void	placePiece(Data &data)
{
	// Checks if the left mouse button is released while we had a piece grabbed.
	if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && data.grabbedPiece != nullptr)
	{
		int size = SCREEN_WIDTH / 8;
		int x = GetMouseX();
		int y = GetMouseY();

		x /= size;
		y /= size;
		// Checks if the move from the position of the grabbed piece to the position of the mouse cursor is possible.
		// To do so it needs a pointer to the piece, the grabbed piece position and the x and y value that would need to be added to it to make that move.
		// Last bool should only be true if we're looking for a check.
		if (isMovePossible(data, data.grabbedPiece, data.grabbedPiecePosX, data.grabbedPiecePosY, x - data.grabbedPiecePosX, y - data.grabbedPiecePosY, false))
		{
			// If the move is theoretically possible, checks if the square our piece lands on does not have any piece of the same owner on it, or is empty.
			if ((data.square[y][x].piece != nullptr && data.square[data.grabbedPiecePosY][data.grabbedPiecePosX].piece->getOwner() != data.square[y][x].piece->getOwner()) || data.square[y][x].piece == nullptr)
			{
				// If this is also true, makes the move.
				data.square[data.grabbedPiecePosY][data.grabbedPiecePosX].piece = nullptr;
				// If there is an piece that would get overwritten, save the address just in case.
				ChessPiece *deletedPiece = nullptr;
				if (data.square[y][x].piece)
					deletedPiece = data.square[y][x].piece;
				data.square[y][x].piece = data.grabbedPiece;

				// Player stores the owner of the piece that made the move.
				int player = data.square[y][x].piece->getOwner();

				// If any king made a move, updates the new position.
				if (data.grabbedPiece->getType() == KING)
				{
					data.kingPosX[data.grabbedPiece->getOwner()] = x;
					data.kingPosY[data.grabbedPiece->getOwner()] = y;
				}
				// Now after the move has been made, checks if there is any check for the player that made the move.
				if (lookForCheck(data, player))
				{
					// If there is a check, the move will be reversed and the deleted piece will be put back in place.
					if (deletedPiece)
						data.square[y][x].piece = deletedPiece;
					else
						data.square[y][x].piece = nullptr;
					data.square[data.grabbedPiecePosY][data.grabbedPiecePosX].piece = data.grabbedPiece;
					// Also resets the position of the king if he was moved.
					if (data.grabbedPiece->getType() == KING)
					{
						data.kingPosX[data.grabbedPiece->getOwner()] = data.grabbedPiecePosX;
						data.kingPosY[data.grabbedPiece->getOwner()] = data.grabbedPiecePosY;
					}
				}
				else
				{
					// If the piece really moved, set the bool hasMoved to true.
					if (!data.grabbedPiece->getHasMoved())
						data.grabbedPiece->setHasMoved(true);
					// The square from - to the move happened are saved for different color in board drawing.
					data.lastMove[0] = &data.square[data.grabbedPiecePosY][data.grabbedPiecePosX];
					data.lastMove[1] = &data.square[y][x];
				}
			}
		}
		// In any case, if move made or not, the grabbed piece gets released and reset again.
		// This happens always because the mouse button was released.
		// It either made its move or it snaps back to original position.
		data.grabbedPiece->setGrabbed(false);
		data.grabbedPiece = nullptr;
		data.grabbedPiecePosX = -1;
		data.grabbedPiecePosY = -1;
		// After everything is over looks for checks on both players. If one king is in check sets the bool for board drawing.
		if (lookForCheck(data, WHITE_P))
			data.kingCheck[WHITE_P] = true;
		else
			data.kingCheck[WHITE_P] = false;
		if (lookForCheck(data, BLACK_P))
			data.kingCheck[BLACK_P] = true;
		else
			data.kingCheck[BLACK_P] = false;
	}

}

void	grabPiece(Data &data)
{
	// If the left mouse button is pressed we set our grabbedPiece pointer to the piece that is on the current field.
	// If there is no piece, nothing happens. If we already have one piece grabbed nothing happens.
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && data.grabbedPiece == nullptr)
	{
		int size = SCREEN_WIDTH / 8;
		int x = GetMouseX();
		int y = GetMouseY();

		x /= size;
		y /= size;
		// Sets the pointer and also saves the position of the grabbed piece.
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
	// First sets all chess piece pointers to nullptr.
	for (int x = 0; x < 8; ++x)
	{
		for (int y = 0; y < 8; ++y)
		{
			data.square[x][y].piece = nullptr;
		}
	}
	// Iterates through all the x fields and puts the right pieces for both players on the board.
	// We always know the right y value, so we only need one loop
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
			data.kingPosX[WHITE_P] = i;
			data.kingPosY[WHITE_P] = 7;
			data.square[0][i].piece = new ChessPiece(BLACK_P, KING);
			data.kingPosX[BLACK_P] = i;
			data.kingPosY[BLACK_P] = 0;
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

void	drawPiece(Data &data, int pieceType, int pieceOwner, raylib::Vector2 pos, float scale)
{
	// Looks for the correct piece type and owner and draws the right texture.
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

void	drawAllPieces(Data &data)
{
	int size = SCREEN_WIDTH / 8;

	// Iterating through all possible boardsquares.
	for (int x = 0; x < 8; ++x)
	{
		for (int y = 0; y < 8; ++y)
		{
			// Checking if there is an actual piece on the current square
			if (data.square[y][x].piece != nullptr)
			{
				raylib::Vector2 pos;
				float			scale;

				int	pieceType = data.square[y][x].piece->getType();
				int pieceOwner = data.square[y][x].piece->getOwner();
				pos.x = x * size;
				pos.y = y * size;
				scale = (float)size / 1280;
				// If it is not the grabbed piece, draw it with the correct texture.
				if (!data.square[y][x].piece->getGrabbed())
					drawPiece(data, pieceType, pieceOwner, pos, scale);
			}
		}
	}
	// After all other pieces have been drawn, draw the grabbed piece last.
	if (data.grabbedPiece)
	{
		raylib::Vector2	pos;
		float			scale;

		int	pieceType = data.grabbedPiece->getType();
		int pieceOwner = data.grabbedPiece->getOwner();

		pos.x = GetMouseX() - size / 2;
		pos.y = GetMouseY() - size / 2;
		scale = (float)size / 1280;
		drawPiece(data, pieceType, pieceOwner, pos, scale);
	}
}

void	drawBoard(Data &data)
{
	int size = SCREEN_WIDTH / 8;

	// Iterating through all possible boardsquares
	for (int x = 0; x < 8; ++x)
	{
		for (int y = 0; y < 8; ++y)
		{
			//Checking if King is in check and on the current looked at field. If yes, draw in red. Else draw in primary or secondary color.
			if ((y == data.kingPosY[WHITE_P] && x == data.kingPosX[WHITE_P] && data.kingCheck[WHITE_P])
				|| (y == data.kingPosY[BLACK_P] && x == data.kingPosX[BLACK_P] && data.kingCheck[BLACK_P]))
					DrawRectangle(size * x, size * y, size, size, RED);
			else if (y % 2 == 0)
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
			// If the current square's address matches with one where the last move was made paint orange/yellow square over it.
			if (&data.square[y][x] == data.lastMove[0] || &data.square[y][x] == data.lastMove[1])
			{
				raylib::Color lastMoveYellow(ORANGE);
				lastMoveYellow.a = 200;
				DrawRectangle(size * x, size * y, size, size, lastMoveYellow);
			}
		}
	}
}

int	main()
{
	Data data;

	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Chess 42");
	SetTargetFPS(120);
	initTex(data);
	initPieces(data);
	while (!WindowShouldClose())
	{
		BeginDrawing();
		// ACTIONS
		grabPiece(data);
		placePiece(data);
		// START DRAWING
		ClearBackground(RAYWHITE);
		drawBoard(data);
		drawAllPieces(data);
		// END DRAWING
		EndDrawing();
	}
	CloseWindow();
}
