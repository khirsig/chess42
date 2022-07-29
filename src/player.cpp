/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/29 10:31:13 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/29 10:40:21 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Data.hpp"

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
						if (chessBoard.square[y][x].piece
							&& chessBoard.square[y][x].piece->getOwner() == player
							&& isMovePossible(chessBoard, x, y, innerX - x, innerY - y, false))
						{
							if (!possibleMoveCheck(chessBoard, x, y, innerX, innerY))
							{
								if ((chessBoard.square[innerY][innerX].piece
									&& chessBoard.square[innerY][innerX].piece->getOwner() != player)
									|| !chessBoard.square[innerY][innerX].piece)
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
		if (!data.chessBoard.checkmate && data.turn == data.grabbedPiece->getOwner()
			&& isMovePossible(data.chessBoard, data.grabbedPiecePosX, data.grabbedPiecePosY, x - data.grabbedPiecePosX, y - data.grabbedPiecePosY, false))
		{
			// If the move is theoretically possible, checks if the square our piece lands on does not have any piece of the same owner on it, or is empty.
			if ((data.chessBoard.square[y][x].piece != nullptr
				&& data.chessBoard.square[data.grabbedPiecePosY][data.grabbedPiecePosX].piece->getOwner() != data.chessBoard.square[y][x].piece->getOwner())
				|| data.chessBoard.square[y][x].piece == nullptr
				|| (data.chessBoard.square[data.grabbedPiecePosY][data.grabbedPiecePosX].piece->getType() == KING && (x == 7 || x == 0)))
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
					data.currentBarDepth = 5;
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
