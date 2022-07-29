/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pieceRules.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 10:29:46 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/29 10:41:05 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Data.hpp"

static int	kingMove(Board &chessBoard, int pieceX, int pieceY, int xAdd, int yAdd)
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

static bool	rookMove(Board &chessBoard, int owner, int pieceX, int pieceY, int xAdd, int yAdd)
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
	while (((yIncr == 1 && y <= yAdd)
			|| (yIncr == -1 && y >= yAdd)
			|| (yIncr == 0)) && ((xIncr == 1 && x <= xAdd)
			|| (xIncr == -1 && x >= xAdd)
			|| (xIncr == 0)))
	{
		if (chessBoard.square[pieceY + y][pieceX + x].piece
			!= nullptr && (x != xAdd || y != yAdd))
			return (false);

		if ((xIncr == 1 && x <= xAdd) || (xIncr == -1 && x >= xAdd))
			x += xIncr;
		if ((yIncr == 1 && y <= yAdd) || (yIncr == -1 && y >= yAdd))
			y += yIncr;
	}
	return (true);
}

static bool	knightMove(Board &chessBoard, int owner, int pieceX, int pieceY, int xAdd, int yAdd)
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

static bool	bishopMove(Board &chessBoard, int owner, int pieceX, int pieceY, int xAdd, int yAdd)
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
	while (((yIncr == 1 && y <= yAdd) || (yIncr == -1 && y >= yAdd)
			|| (yIncr == 0)) && ((xIncr == 1 && x <= xAdd)
			|| (xIncr == -1 && x >= xAdd) || (xIncr == 0)))
	{
		if (chessBoard.square[pieceY + y][pieceX + x].piece
			!= nullptr && (x != xAdd || y != yAdd))
			return (false);

		if ((xIncr == 1 && x <= xAdd) || (xIncr == -1 && x >= xAdd))
			x += xIncr;
		if ((yIncr == 1 && y <= yAdd) || (yIncr == -1 && y >= yAdd))
			y += yIncr;
	}
	return (true);
}

static bool	pawnMove(Board &chessBoard, int pieceX, int pieceY, int xAdd, int yAdd, bool lookForCheck)
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
		if (yAdd == -1 && abs(xAdd) == 1 && chessBoard.square[pieceY + yAdd][pieceX + xAdd].piece
			&& chessBoard.square[pieceY + yAdd][pieceX + xAdd].piece->getOwner() == BLACK_P)
			return (true);
	}
	else
	{
		if (yAdd < 0)
			return (false);
		if (pieceY != 1 && abs(yAdd) > 1)
			return (false);
		if (yAdd == 1 && abs(xAdd) == 1 && chessBoard.square[pieceY + yAdd][pieceX + xAdd].piece
			&& chessBoard.square[pieceY + yAdd][pieceX + xAdd].piece->getOwner() == WHITE_P)
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
			if (bishopMove(chessBoard, pieceOwner, pieceX, pieceY, xAdd, yAdd)
				|| rookMove(chessBoard, pieceOwner, pieceX, pieceY, xAdd, yAdd))
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
			if (chessBoard.square[y][x].piece
				&& chessBoard.square[y][x].piece->getOwner() == otherPlayer)
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
