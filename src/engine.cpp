/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 11:11:38 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/11 13:03:35 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Data.hpp"
#include "../include/engine.hpp"

static float	getOppositeSquare(int i)
{
	if (i == 0)
		return (7);
	if (i == 1)
		return (6);
	if (i == 2)
		return (5);
	if (i == 3)
		return (4);
	if (i == 4)
		return (3);
	if (i == 5)
		return (2);
	if (i == 6)
		return (1);
	if (i == 7)
		return (0);
	return (0);
}

static float	getPieceSquareValue(ChessPiece *piece, int x, int y)
{
	switch (piece->getType()) {
		case PAWN :
			return (pawnFieldValues[y][x]);
		case BISHOP :
			return (bishopFieldValues[y][x]);
		case KNIGHT :
			return (knightFieldValues[y][x]);
		case ROOK :
			return (rookFieldValues[y][x]);
		case QUEEN :
			return (queenFieldValues[y][x]);
		case KING :
			return (kingFieldValues[y][x]);
	}
	return (0);
}

float	calculateBoard(Data &data, BoardSquare currentBoard[8][8], int player, float factor)
{
	float playerValue = 0.0f;
	float opponentValue = 0.0f;

	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			if (currentBoard[y][x].piece)
			{
				int pieceOwner = currentBoard[y][x].piece->getOwner();
				if (player == WHITE_P)
				{
					if (pieceOwner == player)
					{
						playerValue += currentBoard[y][x].piece->getValue();
						playerValue += getPieceSquareValue(currentBoard[y][x].piece, getOppositeSquare(x), getOppositeSquare(y));
					}
					else
					{
						opponentValue += currentBoard[y][x].piece->getValue();
						opponentValue += getPieceSquareValue(currentBoard[y][x].piece, x, y);
					}
				}
				else
				{
					if (pieceOwner == player)
					{
						playerValue += currentBoard[y][x].piece->getValue();
						playerValue += getPieceSquareValue(currentBoard[y][x].piece, x, y);
					}
					else
					{
						opponentValue += currentBoard[y][x].piece->getValue();
						opponentValue += getPieceSquareValue(currentBoard[y][x].piece, getOppositeSquare(x), getOppositeSquare(y));
					}
				}
			}
		}
	}
	float boardValue = playerValue / opponentValue;
	return (boardValue);
}

void	depthCalculation(Data &data, BoardSquare currentBoard[8][8], int movingPlayer, int calcPlayer, float &moveValue, int currentDepth, int totalDepth)
{
	static int ii = 0;

	for (int pieceY = 0; pieceY < 8; ++pieceY)
	{
		for (int pieceX = 0; pieceX < 8; ++pieceX)
		{
			if (currentBoard[pieceY][pieceX].piece && currentBoard[pieceY][pieceX].piece->getOwner() == movingPlayer)
			{
				for (int targetY = 0; targetY < 8; ++targetY)
				{
					for (int targetX = 0; targetX < 8; ++targetX)
					{
						// std::cout << currentDepth << " " << pieceX << pieceY << " " << targetX << targetY << "\n";
						if ((!currentBoard[targetY][targetX].piece || (currentBoard[targetY][targetX].piece && currentBoard[targetY][targetX].piece->getOwner() != movingPlayer))
							&& isMovePossible(data, currentBoard, currentBoard[pieceY][pieceX].piece, pieceX, pieceY, targetX - pieceX, targetY - pieceY, false))
							// && !possibleMoveCheck(data, currentBoard[pieceY][pieceX].piece, pieceX, pieceY, targetX, targetY))
						{
							BoardSquare	copyBoard[8][8];
							for (int y = 0; y < 8; ++y)
								for (int x = 0; x < 8; ++x)
									copyBoard[y][x].piece = currentBoard[y][x].piece;

							if (currentDepth < totalDepth)
							{
								ii++;
								// if ((calcPlayer == BLACK_P && data.currentDepth % 2 == 0) || (calcPlayer == WHITE_P && data.currentDepth % 2 == 1))
								// 	movingPlayer = WHITE_P;
								// else
								// 	movingPlayer = BLACK_P;
								copyBoard[targetY][targetX].piece = copyBoard[pieceY][pieceX].piece;
								copyBoard[pieceY][pieceX].piece = NULL;
								if (totalDepth == currentDepth + 1)
								{
									float currentValue = calculateBoard(data, copyBoard, calcPlayer, 100);
									if (currentValue < moveValue)
									{
										moveValue = currentValue;
									}
								}
								// currentDepth++;
								depthCalculation(data, copyBoard, (movingPlayer  + 1) % 2, calcPlayer, moveValue, currentDepth + 1, totalDepth);
							}
							// currentDepth = 1;
						}
					}
				}
			}
		}
	}
									// std::cout << ii << "\n";
	// if (currentDepth == 1)
	// 	std::cout << movingPlayer << " " << currentDepth << "  " << ii << std::endl;
}

float	getBestMove(Data &data, BoardSquare currentBoard[8][8], BoardSquare currentSquare, int pieceX, int pieceY, int player, int &targetX, int &targetY)
{
	float	bestMove = -10000;
	int		savedX = -1;
	int		savedY = -1;

	int otherPlayer;
	if (player == BLACK_P)
		otherPlayer = WHITE_P;
	else
		otherPlayer = BLACK_P;
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			if ((!currentBoard[y][x].piece || currentBoard[y][x].piece->getOwner() != player) && isMovePossible(data, currentBoard, currentSquare.piece, pieceX, pieceY, x - pieceX, y - pieceY, false) && !possibleMoveCheck(data, currentBoard[pieceY][pieceX].piece, pieceX, pieceY, x, y))
			{
				ChessPiece *deletedPiece;
				if (currentBoard[pieceY][pieceX].piece)
					deletedPiece = currentBoard[y][x].piece;
				currentBoard[y][x].piece = currentBoard[pieceY][pieceX].piece;
				currentBoard[pieceY][pieceX].piece = NULL;
				float currentMove = 10000;

				std::cout << (char)(pieceX + '0' + 17) << pieceY + 1 << " -> " << (char)(x + '0' + 17) << y + 1;
				depthCalculation(data, currentBoard, otherPlayer, player, currentMove, data.currentDepth, DEPTH);
				std::cout << " = " << currentMove << "\n";


				if (bestMove == -10000 || bestMove < currentMove)
				{
					savedX = x;
					savedY = y;
					bestMove = currentMove;
				}
				currentBoard[pieceY][pieceX].piece = currentBoard[y][x].piece;
				if (deletedPiece)
					currentBoard[y][x].piece = deletedPiece;
				else
					currentBoard[y][x].piece = NULL;
			}
		}
	}
	if (savedX != -1 && savedY != -1)
	{
		targetX = savedX;
		targetY = savedY;
	}
	return (bestMove);
}

void	moveAI(Data &data, BoardSquare currentBoard[8][8], int player)
{
	if (data.waitAI >= 180 && !data.checkmate)
	{
		if (lookForCheckmate(data))
		{
			data.checkmate = true;
			return ;
		}
		if (data.turn == player)
		{
			Move bestMove;
			bestMove.evaluatedPoints = -10000;

			data.currentDepth = 1;
			data.depth = 3;
			for (int y = 0; y < 8; ++y)
			{
				for (int x = 0; x < 8; ++x)
				{
					if (data.square[y][x].piece && data.square[y][x].piece->getOwner() == player)
					{
						float evaluatedMove;
						int targetX = -1, targetY = -1;

						evaluatedMove = getBestMove(data, currentBoard, currentBoard[y][x], x, y, player, targetX, targetY);
						if (bestMove.evaluatedPoints == -1 || bestMove.evaluatedPoints < evaluatedMove)
						{
							bestMove.startX = x;
							bestMove.startY = y;
							bestMove.targetX = targetX;
							bestMove.targetY = targetY;
							bestMove.evaluatedPoints = evaluatedMove;
						}
					}
				}
			}
			ChessPiece *deletedPiece = NULL;
			if (data.square[bestMove.targetY][bestMove.targetX].piece)
				deletedPiece = data.square[bestMove.targetY][bestMove.targetX].piece;
			data.square[bestMove.targetY][bestMove.targetX].piece = data.square[bestMove.startY][bestMove.startX].piece;
			data.square[bestMove.startY][bestMove.startX].piece = NULL;
			if (data.moveNbr < data.history.size() - 1)
			{
				int	elemToDelete = (int) data.history.size() - 1 - data.moveNbr;
				for (int i = 0; i < elemToDelete; ++i)
					data.history.pop_back();
			}

			int pieceOwner = data.square[bestMove.targetY][bestMove.targetX].piece->getOwner();
			if (data.square[bestMove.targetY][bestMove.targetX].piece->getType() == PAWN && ((bestMove.targetY == 7 && pieceOwner == BLACK_P) || (bestMove.targetY == 0 && pieceOwner == WHITE_P)))
				data.square[bestMove.targetY][bestMove.targetX].piece->setType(QUEEN);
			if (data.square[bestMove.targetY][bestMove.targetX].piece->getType() == KING)
			{
				data.kingPosX[pieceOwner] = bestMove.targetX;
				data.kingPosY[pieceOwner] = bestMove.targetY;
			}

			History	history;
			history.movedPiece = data.grabbedPiece;
			if (deletedPiece != nullptr)
				history.removedPiece = deletedPiece;
			history.fromX = bestMove.startX;
			history.fromY = bestMove.startY;
			history.toX = bestMove.targetX;
			history.toY = bestMove.targetY;
			data.history.push_back(history);
			data.moveNbr++;
			data.lastMove[0] = &data.square[bestMove.startY][bestMove.startX];
			data.lastMove[1] = &data.square[bestMove.targetY][bestMove.targetX];
			toggleCheckBothPlayers(data);
			lookForCheckmate(data);
			if (player == BLACK_P)
				data.turn = WHITE_P;
			else
				data.turn = BLACK_P;
			data.waitAI = 0;
		}
	}
	else
		data.waitAI++;
}
