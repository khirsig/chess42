/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 11:11:38 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/06 16:50:34 by khirsig          ###   ########.fr       */
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

float	calculateBoard(Data &data, BoardSquare currentBoard[8][8], int player)
{
	float boardValue = 0.0f;

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
						boardValue += currentBoard[y][x].piece->getValue();
						boardValue += getPieceSquareValue(currentBoard[y][x].piece, getOppositeSquare(x), getOppositeSquare(y));
					}
					else
					{
						boardValue -= currentBoard[y][x].piece->getValue();
						boardValue -= getPieceSquareValue(currentBoard[y][x].piece, x, y);
					}
				}
				else
				{
					if (pieceOwner == player)
					{
						boardValue += currentBoard[y][x].piece->getValue();
						boardValue += getPieceSquareValue(currentBoard[y][x].piece, x, y);
					}
					else
					{
						boardValue -= currentBoard[y][x].piece->getValue();
						boardValue -= getPieceSquareValue(currentBoard[y][x].piece, getOppositeSquare(x), getOppositeSquare(y));
					}
				}
			}
		}
	}
	return (boardValue);
}

float	getBestDeepMove(Data &data, BoardSquare currentBoard[8][8], BoardSquare currentSquare, int pieceX, int pieceY, int player, int &targetX, int &targetY)
{
	float	bestMove = -10000;
	int		savedX = -1;
	int		savedY = -1;

	static int i = 0;
	i++;
	std::cout << i << std::endl;
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			if (isMovePossible(data, currentBoard, currentSquare.piece, pieceX, pieceY, x - pieceX, y - pieceY, false))
			{
				ChessPiece *deletedPiece;
				if (currentBoard[pieceY][pieceX].piece)
					deletedPiece = currentBoard[y][x].piece;
				currentBoard[y][x].piece = currentBoard[pieceY][pieceX].piece;
				currentBoard[pieceY][pieceX].piece = NULL;
				float currentMove = calculateBoard(data, currentBoard, player);
				if (bestMove == -1 || bestMove < currentMove)
				{
					// std::cout << "Best Move: " << pieceX << pieceY << " --> " << x << y << " = " << currentMove << " > " << bestMove << "\n";
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
	targetX = savedX;
	targetY = savedY;
	return (bestMove);
}

static float checkEnemyMoves(Data &data, BoardSquare currentBoard[8][8], int calcPlayer, int player, int &startX, int &startY, int &newtargetX, int &newtargetY)
{
	Move bestMove;
	bestMove.evaluatedPoints = -10000;
	float	avgPoints = 0;
	int		moveAmt = 0;

	bestMove.startX = -1;
	bestMove.startY = -1;
	bestMove.targetX = -1;
	bestMove.targetY = -1;
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			if (currentBoard[y][x].piece && currentBoard[y][x].piece->getOwner() == player)
			{
				float evaluatedMove;
				int targetX = -1, targetY = -1;

				evaluatedMove = getBestDeepMove(data, currentBoard, currentBoard[y][x], x, y, player, targetX, targetY);
				if (evaluatedMove != -10000 && bestMove.evaluatedPoints < evaluatedMove)
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
	startX = bestMove.startX;
	startY = bestMove.startY;
	newtargetX = bestMove.targetX;
	newtargetY = bestMove.targetY;
	return (bestMove.evaluatedPoints);
}

static float	calculateDeep(Data &data, BoardSquare currentBoard[8][8], int player, float currentMove)
{
	float avgPoints = 0;
	float worstOutcome = currentMove;
	Move	bestMove;
	BoardSquare	copyBoard[8][8];

	bestMove.startX = -1;
	bestMove.startY = -1;
	bestMove.targetX = -1;
	bestMove.targetY = -1;
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			copyBoard[y][x].piece = currentBoard[y][x].piece;
		}
	}
	while (data.currentDepth < data.depth)
	{
		if ((player == BLACK_P && data.currentDepth % 2 == 0) || (player == WHITE_P && data.currentDepth % 2 == 1))
		{
			// std::cout << "BLACK moves" << "\n";
			checkEnemyMoves(data, copyBoard, player, BLACK_P, bestMove.startX, bestMove.startY, bestMove.targetX, bestMove.targetY);
		}
		else
		{
			// std::cout << "WHITE moves" << "\n";
			checkEnemyMoves(data, copyBoard, player, WHITE_P, bestMove.startX, bestMove.startY, bestMove.targetX, bestMove.targetY);
		}
		// std::cout << bestMove.startX << " " << bestMove.startY << " --> " << bestMove.targetX << " " << bestMove.targetY << "\n";
		if (bestMove.startX != -1 && bestMove.startY != -1 && bestMove.targetX != -1 && bestMove.targetY != -1)
		{
			copyBoard[bestMove.targetY][bestMove.targetX].piece = copyBoard[bestMove.startY][bestMove.startX].piece;
			copyBoard[bestMove.startY][bestMove.startX].piece = NULL;
			avgPoints = calculateBoard(data, copyBoard, player);

			if (worstOutcome > avgPoints)
			{
				// std::cout << "Had to adjust" << "\n";
				worstOutcome = avgPoints;
			}
		}
		// std::cout << data.currentDepth << "  " << worstOutcome << "\n";
		data.currentDepth++;
	}
	return (worstOutcome);
}

float	getBestMove(Data &data, BoardSquare currentBoard[8][8], BoardSquare currentSquare, int pieceX, int pieceY, int player, int &targetX, int &targetY)
{
	float	bestMove = -10000;
	int		savedX = -1;
	int		savedY = -1;

	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			if (isMovePossible(data, currentBoard, currentSquare.piece, pieceX, pieceY, x - pieceX, y - pieceY, false))
			{
				ChessPiece *deletedPiece;
				if (currentBoard[pieceY][pieceX].piece)
					deletedPiece = currentBoard[y][x].piece;
				currentBoard[y][x].piece = currentBoard[pieceY][pieceX].piece;
				currentBoard[pieceY][pieceX].piece = NULL;
				float currentMove = calculateBoard(data, currentBoard, player);
				float avgPoints = currentMove;

				if (data.currentDepth < data.depth)
				{
					avgPoints = calculateDeep(data, currentBoard, player, currentMove);
					data.currentDepth = 1;
				}
				if (avgPoints < currentMove)
					currentMove = avgPoints;
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
	if (data.waitAI >= 80)
	{
		if (data.turn == BLACK_P)
		{
			Move bestMove;
			bestMove.evaluatedPoints = -10000;

			data.currentDepth = 1;
			data.depth = 5;
			for (int y = 0; y < 8; ++y)
			{
				for (int x = 0; x < 8; ++x)
				{
					if (data.square[y][x].piece && data.square[y][x].piece->getOwner() == BLACK_P)
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
			std::cout << bestMove.startX << "  " << bestMove.startY << " --> " << bestMove.targetX << "  " << bestMove.targetY << std::endl;
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
			data.turn = WHITE_P;
			data.waitAI = 0;
		}
	}
	else
		data.waitAI++;
}
