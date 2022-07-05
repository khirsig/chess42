/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 11:11:38 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/05 16:44:49 by khirsig          ###   ########.fr       */
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
	return (boardValue);
}

float	getBestMove(Data &data, BoardSquare currentBoard[8][8], BoardSquare currentSquare, int pieceX, int pieceY, int player, int &targetX, int &targetY, int depth);

static float checkEnemyMoves(Data &data, BoardSquare currentBoard[8][8], int player, int depth, int &startX, int &startY, int &newtargetX, int &newtargetY)
{
	Move bestMove;
	bestMove.evaluatedPoints = -1;
	float	avgPoints = 0;
	int		moveAmt = 0;

	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			if (currentBoard[y][x].piece && currentBoard[y][x].piece->getOwner() != player)
			{
				float evaluatedMove;
				int targetX = -1, targetY = -1;

				evaluatedMove = getBestMove(data, currentBoard, currentBoard[y][x], x, y, player, targetX, targetY, 0);
				if (bestMove.evaluatedPoints == -1 || bestMove.evaluatedPoints > evaluatedMove)
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

static float	testEnemyMoves(Data &data, BoardSquare currentBoard[8][8], int player, int depth, float currentMove)
{
	float avgPoints = 0;
	float worstOutcome = currentMove;
	Move	bestMove;
	BoardSquare	copyBoard[8][8];
	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			copyBoard[y][x].piece = currentBoard[y][x].piece;
		}
	}
	while (depth < 3)
	{
		std::cout << "DEPTH = " << depth << std::endl;
		int	nextPlayer;
		if ((player == BLACK_P && depth % 2 == 0) || (player == WHITE_P && depth % 2 == 1))
			avgPoints = checkEnemyMoves(data, copyBoard, BLACK_P, depth, bestMove.startX, bestMove.startY, bestMove.targetX, bestMove.targetY);
		else
			avgPoints = checkEnemyMoves(data, copyBoard, WHITE_P, depth, bestMove.startX, bestMove.startY, bestMove.targetX, bestMove.targetY);
		depth++;
		copyBoard[bestMove.targetY][bestMove.targetX].piece = copyBoard[bestMove.startY][bestMove.startX].piece;
		copyBoard[bestMove.startY][bestMove.startX].piece = NULL;
		if (worstOutcome > avgPoints)
			worstOutcome = avgPoints;
	}
	return (worstOutcome);
}

float	getBestMove(Data &data, BoardSquare currentBoard[8][8], BoardSquare currentSquare, int pieceX, int pieceY, int player, int &targetX, int &targetY, int depth)
{
	float	bestMove = -1;
	int		savedX = -1;
	int		savedY = -1;

	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			if (isMovePossible(data, currentSquare.piece, pieceX, pieceY, x - pieceX, y - pieceY, false))
			{
				depth++;
				ChessPiece *deletedPiece;
				if (currentBoard[pieceY][pieceX].piece)
					deletedPiece = currentBoard[y][x].piece;
				currentBoard[y][x].piece = currentBoard[pieceY][pieceX].piece;
				currentBoard[pieceY][pieceX].piece = NULL;
				float currentMove = calculateBoard(data, currentBoard, player);
				float avgPoints = currentMove;
				if (depth != 0)
					avgPoints = testEnemyMoves(data, currentBoard, player, depth, currentMove);
				if (avgPoints < currentMove)
					currentMove = avgPoints;
				if (bestMove == -1 || bestMove < currentMove)
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
	targetX = savedX;
	targetY = savedY;
	return (bestMove);
}

void	moveAI(Data &data, BoardSquare currentBoard[8][8], int player)
{
	if (data.waitAI == 80)
	{
		if (data.turn == BLACK_P)
		{
			Move bestMove;
			bestMove.evaluatedPoints = -1;

			for (int y = 0; y < 8; ++y)
			{
				for (int x = 0; x < 8; ++x)
				{
					if (data.square[y][x].piece && data.square[y][x].piece->getOwner() == BLACK_P)
					{
						float evaluatedMove;
						int targetX = -1, targetY = -1;

						evaluatedMove = getBestMove(data, currentBoard, currentBoard[y][x], x, y, player, targetX, targetY, 1);
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
			data.square[bestMove.targetY][bestMove.targetX].piece = data.square[bestMove.startY][bestMove.startX].piece;
			data.square[bestMove.startY][bestMove.startX].piece = NULL;
			data.turn = WHITE_P;
			data.waitAI = 0;
		}
	}
	else
		data.waitAI++;
}