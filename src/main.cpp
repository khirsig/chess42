/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 19:25:28 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/23 04:48:03 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Data.hpp"

int	kingMove(Board &chessBoard, int pieceX, int pieceY, int xAdd, int yAdd)
{
	// The first case checks if we try to castle.
	// It checks if the piece on the target square a rook and if any of the involved pieces have already moved.
	// if (chessBoard.square[pieceY][pieceX + xAdd].piece != nullptr)
	if (abs(xAdd) > 0 && yAdd == 0
		&& chessBoard.castled[chessBoard.square[pieceY][pieceX].piece->getOwner()] == -1
		&& chessBoard.square[pieceY][pieceX + xAdd].piece != nullptr
		&& chessBoard.square[pieceY][pieceX + xAdd].piece->getType() == ROOK
		&& !chessBoard.square[pieceY][pieceX + xAdd].piece->getHasMoved()
		&& !chessBoard.square[pieceY][pieceX].piece->getHasMoved())
	{
		// std::cout << "ATTEMPTING CASTLE\n";
		int incr;
		if (xAdd > 0)
			incr = 1;
		else
			incr = -1;
		int x = incr;
		// The loop checks if we do not jump over pieces while castling
		while ((incr == 1 && x <= xAdd) || (incr == -1 && x >= xAdd))
		{
			if (chessBoard.square[pieceY][pieceX + x].piece != nullptr && x != xAdd)
				return (-1);
			if (chessBoard.square[pieceY][pieceX + x].piece != nullptr && x == xAdd)
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
	if (chessBoard.square[pieceY + yAdd][pieceX + xAdd].piece
		&& chessBoard.square[pieceY + yAdd][pieceX + xAdd].piece->getOwner()
		==  chessBoard.square[pieceY][pieceX].piece->getOwner())
		return (-1);
	// The last two cases check if the kings tries to move further than one field in his normal movement.
	if (abs(xAdd) > 1 || abs(yAdd) > 1)
		return (-1);
	if (abs(xAdd) != 1 && abs(yAdd) != 1)
		return (-1);
	return (KING_NORMAL);
}

bool	rookMove(Board &chessBoard, int owner, int pieceX, int pieceY, int xAdd, int yAdd)
{
	// The rook can only move on the straights. So if xAdd and yAdd are both != 0 returns false.
	if (xAdd != 0 && yAdd != 0)
		return (false);
	if (chessBoard.square[pieceY + yAdd][pieceX + xAdd].piece
		&& chessBoard.square[pieceY + yAdd][pieceX + xAdd].piece->getOwner()
		==  chessBoard.square[pieceY][pieceX].piece->getOwner())
		return (false);
	// if (chessBoard.square[pieceX + xAdd][pieceY + yAdd].piece && chessBoard.square[pieceX + xAdd][pieceY + yAdd].piece->getOwner() == owner)
	// 	return (false);

	// We set both the x increment value and the y increment value according if our xAdd and yAdd goal are positive or negative.
	// This way we can iterate through the loop later in correct directions at the same time.
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

	// This loop checks if the rook tries to jump over other pieces. If it tries to, return false.
	while (((yIncr == 1 && y <= yAdd) || (yIncr == -1 && y >= yAdd) || (yIncr == 0)) && ((xIncr == 1 && x <= xAdd) || (xIncr == -1 && x >= xAdd) || (xIncr == 0)))
	{
		if (chessBoard.square[pieceY + y][pieceX + x].piece != nullptr && (x != xAdd || y != yAdd))
			return (false);

		if ((xIncr == 1 && x <= xAdd) || (xIncr == -1 && x >= xAdd))
			x += xIncr;
		if ((yIncr == 1 && y <= yAdd) || (yIncr == -1 && y >= yAdd))
			y += yIncr;
	}
	return (true);
}

bool	knightMove(Board &chessBoard, int owner, int pieceX, int pieceY, int xAdd, int yAdd)
{
	if (!((abs(xAdd) == 2 && abs(yAdd) == 1) || (abs(xAdd) == 1 && abs(yAdd) == 2)))
		return (false);
	if (chessBoard.square[pieceY + yAdd][pieceX + xAdd].piece
		&& chessBoard.square[pieceY + yAdd][pieceX + xAdd].piece->getOwner()
		==  chessBoard.square[pieceY][pieceX].piece->getOwner())
		return (false);
	// if (chessBoard.square[pieceX + xAdd][pieceY + yAdd].piece && chessBoard.square[pieceX + xAdd][pieceY + yAdd].piece->getOwner() == owner)
	// 	return (false);
	return (true);
}

bool	bishopMove(Board &chessBoard, int owner, int pieceX, int pieceY, int xAdd, int yAdd)
{
	// Checks if the bishop moves only in diagonals, if not returns false
	if (chessBoard.square[pieceY + yAdd][pieceX + xAdd].piece
		&& chessBoard.square[pieceY + yAdd][pieceX + xAdd].piece->getOwner()
		==  chessBoard.square[pieceY][pieceX].piece->getOwner())
		return (false);
	if (xAdd == 0 || yAdd == 0)
		return (false);
	if (abs(xAdd) != abs(yAdd))
		return (false);
	// if (chessBoard.square[pieceX + xAdd][pieceY + yAdd].piece && chessBoard.square[pieceX + xAdd][pieceY + yAdd].piece->getOwner() == owner)
	// 	return (false);

	// We set both the x increment value and the y increment value according if our xAdd and yAdd goal are positive or negative
	// This way we can iterate through the loop later in correct directions at the same time.
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

	// The loop makes sure that the bishop does not jump over pieces. If it tries to, returns false.
	while (((yIncr == 1 && y <= yAdd) || (yIncr == -1 && y >= yAdd) || (yIncr == 0)) && ((xIncr == 1 && x <= xAdd) || (xIncr == -1 && x >= xAdd) || (xIncr == 0)))
	{
		if (chessBoard.square[pieceY + y][pieceX + x].piece != nullptr && (x != xAdd || y != yAdd))
			return (false);

		if ((xIncr == 1 && x <= xAdd) || (xIncr == -1 && x >= xAdd))
			x += xIncr;
		if ((yIncr == 1 && y <= yAdd) || (yIncr == -1 && y >= yAdd))
			y += yIncr;
	}
	return (true);
}

bool	pawnMove(Board &chessBoard, int pieceX, int pieceY, int xAdd, int yAdd, bool lookForCheck)
{
	// The Pawn can either move two fields if it has not been moved yet, or one field.
	// Taking pieces is a bit different with pawns so we need a special check for it.
	// If lookForCheck is true, we only look for the diagonal movement.
	if (abs(yAdd) > 2 || abs(xAdd) > 1)
		return (false);
	if (chessBoard.square[pieceY + yAdd][pieceX + xAdd].piece && abs(xAdd) != 1)
		return (false);
	if (chessBoard.square[pieceY][pieceX].piece->getOwner() == WHITE_P)
	{
		if (yAdd > 0)
			return (false);
		if (pieceY != 6 && abs(yAdd) > 1)
			return (false);
		if (yAdd == -1 && abs(xAdd) == 1 && chessBoard.square[pieceY + yAdd][pieceX + xAdd].piece && chessBoard.square[pieceY + yAdd][pieceX + xAdd].piece->getOwner() == BLACK_P)
			return (true);
	}
	else
	{
		if (yAdd < 0)
			return (false);
		if (pieceY != 1 && abs(yAdd) > 1)
			return (false);
		if (yAdd == 1 && abs(xAdd) == 1 && chessBoard.square[pieceY + yAdd][pieceX + xAdd].piece && chessBoard.square[pieceY + yAdd][pieceX + xAdd].piece->getOwner() == WHITE_P)
			return (true);
	}
	if (abs(yAdd) != 1 && abs(xAdd) >= 1)
		return (false);

	int	incr = 0;
	if (yAdd > 0)
		incr = 1;
	else if (yAdd < 0)
		incr = -1;
	int i = incr;

	while ((incr == 1 && i <= yAdd) || (incr == -1 && i >= yAdd))
	{
		if (chessBoard.square[pieceY + i][pieceX + xAdd].piece != nullptr && xAdd == 0)
			return (false);
		if (chessBoard.square[pieceY + i][pieceX + xAdd].piece == nullptr && xAdd != 0)
			return (false);
		i += incr;
	}
	if (abs(xAdd) == 0 && lookForCheck)
		return (false);
	return (true);
}

bool	isMovePossible(Board &chessBoard, int pieceX, int pieceY, int xAdd, int yAdd, bool lookForCheck)
{
	// If there is no range of a movement, the move is just not possible.
	if (xAdd == 0 && yAdd == 0)
		return (false);
	int	pieceOwner = chessBoard.square[pieceY][pieceX].piece->getOwner();
	// Since every piece has a different way of moving we need to find the right type first.
	// Every piece has its own move function that checks if the fields it wants to move through are possible.
	// The King has some special cases with castling.
	switch (chessBoard.square[pieceY][pieceX].piece->getType()) {
		case PAWN :
			if (pawnMove(chessBoard, pieceX, pieceY, xAdd, yAdd, lookForCheck))
				return (true);
			break ;
		case BISHOP :
			if (bishopMove(chessBoard, pieceOwner, pieceX, pieceY, xAdd, yAdd))
				return (true);
			break ;
		case KNIGHT :
			if (knightMove(chessBoard, pieceOwner, pieceX, pieceY, xAdd, yAdd))
				return (true);
			break;
		case ROOK :
			if (rookMove(chessBoard, pieceOwner, pieceX, pieceY, xAdd, yAdd))
				return (true);
			break ;
		case QUEEN :
			if (bishopMove(chessBoard, pieceOwner, pieceX, pieceY, xAdd, yAdd) || rookMove(chessBoard, pieceOwner, pieceX, pieceY, xAdd, yAdd))
				return (true);
			break ;
		case KING :
			int ret = kingMove(chessBoard, pieceX, pieceY, xAdd, yAdd);
			if (ret == KING_NORMAL)
				return (true);
			if (ret == KING_SHORT_CASTLE)
				return (true);
			if (ret == KING_LONG_CASTLE)
				return (true);
			return (false);
			break ;
	}
	return (false);
}

bool	lookForCheck(Board &chessBoard, int player)
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
			if (chessBoard.square[y][x].piece && chessBoard.square[y][x].piece->getOwner() == otherPlayer)
			{
				// Looks if any of those pieces could make a theoretical move to the position of the king.
				// We need the piece that could give check and its position and also the x and y value it would need to make that move.
				// Last bool should be true because we're looking for a check, not moving.
				if (isMovePossible(chessBoard, x, y,
					chessBoard.kingPosX[player] - x, chessBoard.kingPosY[player] - y, true))
					return (true);
			}
		}
	}
	// If no piece was able to move to the position of players king, return false.
	return (false);
}

bool	lookForCheckmate(Board &chessBoard)
{
	int	possibleMoves = 0;

	if (chessBoard.kingCheck[WHITE_P] || chessBoard.kingCheck[BLACK_P])
	{
		int player;
		if (chessBoard.kingCheck[WHITE_P])
			player = WHITE_P;
		else
			player = BLACK_P;
		for (int y = 0; y < 8; ++y)
		{
			for (int x = 0; x < 8; ++x)
			{
				for (int innerY = 0; innerY < 8; ++innerY)
				{
					for (int innerX = 0; innerX < 8; ++innerX)
					{
						if (chessBoard.square[y][x].piece && chessBoard.square[y][x].piece->getOwner() == player
							&& isMovePossible(chessBoard, x, y, innerX - x, innerY - y, false))
						{
							if (!possibleMoveCheck(chessBoard, x, y, innerX, innerY))
							{
								if ((chessBoard.square[innerY][innerX].piece && chessBoard.square[innerY][innerX].piece->getOwner() != player) || !chessBoard.square[innerY][innerX].piece)
									possibleMoves++;
							}
						}
					}
				}
			}
		}
	}
	if (possibleMoves >= 0)
		return (false);
	else
		return (true);
}

void	toggleCheckBothPlayers(Board &chessBoard)
{
		if (lookForCheck(chessBoard, WHITE_P))
			chessBoard.kingCheck[WHITE_P] = true;
		else
			chessBoard.kingCheck[WHITE_P] = false;
		if (lookForCheck(chessBoard, BLACK_P))
			chessBoard.kingCheck[BLACK_P] = true;
		else
			chessBoard.kingCheck[BLACK_P] = false;
}

void	placePiece(Data &data, int player)
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
		if (!data.chessBoard.checkmate && data.turn == data.grabbedPiece->getOwner() && isMovePossible(data.chessBoard, data.grabbedPiecePosX, data.grabbedPiecePosY, x - data.grabbedPiecePosX, y - data.grabbedPiecePosY, false))
		{
			// If the move is theoretically possible, checks if the square our piece lands on does not have any piece of the same owner on it, or is empty.
			if ((data.chessBoard.square[y][x].piece != nullptr
				&& data.chessBoard.square[data.grabbedPiecePosY][data.grabbedPiecePosX].piece->getOwner() != data.chessBoard.square[y][x].piece->getOwner())
				|| data.chessBoard.square[y][x].piece == nullptr || (data.chessBoard.square[data.grabbedPiecePosY][data.grabbedPiecePosX].piece->getType() == KING && (x == 7 || x == 0)))
			{
				ChessPiece *deletedPiece = movePiece(data.chessBoard, data.grabbedPiecePosX, data.grabbedPiecePosY, x, y);

				// Now after the move has been made, checks if there is any check for the player that made the move.
				if (lookForCheck(data.chessBoard, player))
					revertMovePiece(data.chessBoard, data.grabbedPiecePosX, data.grabbedPiecePosY, x, y, deletedPiece, player);
				else
				{
					// The square from - to the move happened are saved for different color in board drawing.
					if (data.chessBoard.castled[player] != data.chessBoard.moveTurn)
					{
						data.lastMove[0] = &data.chessBoard.square[data.grabbedPiecePosY][data.grabbedPiecePosX];
						data.lastMove[1] = &data.chessBoard.square[y][x];
					}
					else
					{
						if (data.chessBoard.kingPosX[player] == 6)
							data.lastMove[0] = &data.chessBoard.square[data.chessBoard.kingPosY[player]][data.chessBoard.kingPosX[player] - 1];
						else
							data.lastMove[0] = &data.chessBoard.square[data.chessBoard.kingPosY[player]][data.chessBoard.kingPosX[player] + 1];
						data.lastMove[1] = &data.chessBoard.square[data.chessBoard.kingPosY[player]][data.chessBoard.kingPosX[player]];
					}
					data.moveNbr++;

					int otherPlayer;
					if (player == WHITE_P)
						otherPlayer = BLACK_P;
					else
						otherPlayer = WHITE_P;
					data.turn = otherPlayer;
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
		toggleCheckBothPlayers(data.chessBoard);
		if (lookForCheckmate(data.chessBoard))
			data.chessBoard.checkmate = true;
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
		if (data.chessBoard.square[y][x].piece != nullptr)
		{
			data.grabbedPiece = data.chessBoard.square[y][x].piece;
			data.chessBoard.square[y][x].piece->setGrabbed(true);
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

	// Iterating through all possible Boards.
	for (int x = 7; x >= 0; --x)
	{
		for (int y = 7; y >= 0; --y)
		{
			// Checking if there is an actual piece on the current square
			if (data.chessBoard.square[y][x].piece != nullptr)
			{
				raylib::Vector2 pos;
				float			scale;

				int	pieceType = data.chessBoard.square[y][x].piece->getType();
				int pieceOwner = data.chessBoard.square[y][x].piece->getOwner();
				pos.x = x * size;
				pos.y = y * size;
				scale = (float)size / 1280;
				// If it is not the grabbed piece, draw it with the correct texture.
				if (!data.chessBoard.square[y][x].piece->getGrabbed())
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
		drawPiece(data, pieceType, pieceOwner, pos, scale * 1.10);
	}
}

bool	possibleMoveCheck(Board &chessBoard, int pieceX, int pieceY, int targetX, int targetY)
{
	bool isCheck = false;
	int player = chessBoard.square[pieceY][pieceX].piece->getOwner();

	ChessPiece *deletedPiece  = movePiece(chessBoard, pieceX, pieceY, targetX, targetY);

	if (lookForCheck(chessBoard, player))
		isCheck = true;

	revertMovePiece(chessBoard, pieceX, pieceY, targetX, targetY, deletedPiece, player);

	return (isCheck);
}

void	drawPossibleMoves(Data &data, ChessPiece *piece, int pieceX, int pieceY, int size)
{
	if (piece->getOwner() != data.turn)
		return ;
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			if ((!data.chessBoard.square[y][x].piece || data.chessBoard.square[y][x].piece->getOwner() != piece->getOwner())
				&& isMovePossible(data.chessBoard, pieceX, pieceY, x - pieceX, y - pieceY, false)
				&& !possibleMoveCheck(data.chessBoard, pieceX, pieceY, x, y))
			{
				raylib::Color circCol;
				if (y % 2 == 0)
				{
					if (x % 2 == 0)
						circCol = data.secondaryColor;
					else
						circCol = data.primaryColor;
				}
				else
				{
					if (x % 2 == 0)
						circCol = data.primaryColor;
					else
						circCol = data.secondaryColor;
				}
				if (!data.chessBoard.square[y][x].piece)
					DrawCircle(size * x + size / 2, size * y + size / 2, size / 6, circCol);
				else
				{
					DrawCircle(size * x + size / 2, size * y + size / 2, size / 2.2, circCol);
					if (data.lastMove[0] == &data.chessBoard.square[y][x] || data.lastMove[1] == &data.chessBoard.square[y][x])
					{
						raylib::Color lastMoveCol(GRAY);
						DrawCircle(size * x + size / 2, size * y + size / 2, size / 2.7, lastMoveCol);
					}
					else if (circCol == data.primaryColor)
						DrawCircle(size * x + size / 2, size * y + size / 2, size / 2.7, data.secondaryColor);
					else if (circCol == data.secondaryColor)
						DrawCircle(size * x + size / 2, size * y + size / 2, size / 2.7, data.primaryColor);
				}
			}
		}
	}
}

void	drawBoard(Data &data)
{
	int size = SCREEN_WIDTH / 8;

	// Iterating through all possible Boards
	for (int x = 0; x < 8; ++x)
	{
		for (int y = 0; y < 8; ++y)
		{
			//Checking if King is in check and on the current looked at field. If yes, draw in red. Else draw in primary or secondary color.
			if ((y == data.chessBoard.kingPosY[WHITE_P] && x == data.chessBoard.kingPosX[WHITE_P] && data.chessBoard.kingCheck[WHITE_P])
				|| (y == data.chessBoard.kingPosY[BLACK_P] && x == data.chessBoard.kingPosX[BLACK_P] && data.chessBoard.kingCheck[BLACK_P]))
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
			// If the current square's address matches with one where the last move was made paint gray square over it.
			if (&data.chessBoard.square[y][x] == data.lastMove[0] || &data.chessBoard.square[y][x] == data.lastMove[1])
			{
				raylib::Color lastMoveCol(ORANGE);
				lastMoveCol.a = 200;
				DrawRectangle(size * x, size * y, size, size, lastMoveCol);
			}
		}
	}
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			if (data.grabbedPiece && data.grabbedPiecePosX == x && data.grabbedPiecePosY == y)
			{
				raylib::Color grabbedPieceCol(YELLOW);
				grabbedPieceCol.a = 150;
				DrawRectangle(size * x, size * y, size, size, grabbedPieceCol);
				drawPossibleMoves(data, data.chessBoard.square[y][x].piece, x, y, size);
			}
		}
	}
	int x = 0;
	for (int y = 0; y < 8; ++y)
	{
		std::string squareNbrY = std::to_string(x + 1);
		char c = y + '0' + 17;
		std::string squareNbrX;
		squareNbrX += c;
		int	indent = size - size / 6;
		if (x % 2 == 0)
		{
			// Bottom Numbers
			DrawText(squareNbrX.c_str(), size * x + size / 20 + indent, size * 7 + size / 20 + indent, size / 10, data.primaryColor);
			// Left Numbers
			DrawText(squareNbrY.c_str(), size * 0 + size / 20, size * y + size / 20, size / 10, data.secondaryColor);
		}
		else
		{
			// Bottom Numbers
			DrawText(squareNbrX.c_str(), size * x + size / 20 + indent, size * 7 + size / 20 + indent, size / 10, data.secondaryColor);
			// Left Numbers
			DrawText(squareNbrY.c_str(), size * 0 + size / 20, size * y + size / 20, size / 10, data.primaryColor);
		}
		++x;
	}
}

void	drawDebugInfo(Data &data)
{
	int size = SCREEN_WIDTH / 8;

	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			if (data.chessBoard.square[y][x].debug)
			{
				std::string text = "Debug on";
				int xOffset = size * x + size / 2 - MeasureText(text.c_str(), size / 10) / 2;
				int yOffset = size * y + (size / 10 * 0);
				DrawText(text.c_str(), xOffset, yOffset, size / 10, PINK);
				text = "HasMoved: ";
				if (data.chessBoard.square[y][x].piece)
				{
					if (data.chessBoard.square[y][x].piece->getHasMoved())
						text += "true";
					else
						text += "false";
				}
				else
					text += "?";
				xOffset = size * x + size / 2 - MeasureText(text.c_str(), size / 10) / 2;
				yOffset = size * y + (size / 10 * 2);
				DrawText(text.c_str(), xOffset, yOffset, size / 10, PINK);
			}
		}
	}
}

void	turnDebugOn(Data &data)
{
	if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
	{
		int size = SCREEN_WIDTH / 8;
		int x = GetMouseX();
		int y = GetMouseY();

		x /= size;
		y /= size;
		if (data.chessBoard.square[y][x].debug == true)
			data.chessBoard.square[y][x].debug = false;
		else
			data.chessBoard.square[y][x].debug = true;
	}
}

int	main()
{
	Data data;

	InitWindow(SCREEN_WIDTH * 1.02, SCREEN_HEIGHT, "Chess 42");
	SetTargetFPS(120);
	initTex(data);
	initPieces(data);
	while (!WindowShouldClose())
	{
		BeginDrawing();
		// ACTIONS
		turnDebugOn(data);
		grabPiece(data);
		placePiece(data, WHITE_P);
		// 	moveAI(data, data.chessBoard, WHITE_P);
		if (data.turn == BLACK_P && data.chessBoard.checkmate == false)
		{
			if (data.aiThinking == false)
				getBestAIMove(data, data.chessBoard, BLACK_P);
			executeAIMove(data, data.chessBoard, BLACK_P);
		}
		// std::cout << calculateBoard(data, data.chessBoard.square, WHITE_P) << "\n";
		// moveThroughHistory(data);
		// START DRAWING
		ClearBackground(RAYWHITE);
		drawBoard(data);
		drawAllPieces(data);
		drawDebugInfo(data);
		// END DRAWING
		EndDrawing();
	}
	CloseWindow();
}
