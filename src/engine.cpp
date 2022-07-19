/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 11:11:38 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/19 10:22:55 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Data.hpp"

bool	operator<(const Move &first, const Move &other)
{
	return (first.getEvalPoints() > other.getEvalPoints());
}

bool	operator>(const Move &first, const Move &other)
{
	return (first.getEvalPoints() < other.getEvalPoints());
}

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
				int		pieceOwner = currentBoard[y][x].piece->getOwner();
				int		pieceType = currentBoard[y][x].piece->getType();
				bool	pieceMoved = currentBoard[y][x].piece->getHasMoved();
				if (player == WHITE_P)
				{
					if (pieceOwner == player)
					{
						playerValue += currentBoard[y][x].piece->getValue();
						playerValue += getPieceSquareValue(currentBoard[y][x].piece, getOppositeSquare(x), getOppositeSquare(y));
						if ((pieceType == KNIGHT || pieceType == BISHOP) && !pieceMoved)
							playerValue -= 5.0f;
					}
					else
					{
						opponentValue += currentBoard[y][x].piece->getValue();
						opponentValue += getPieceSquareValue(currentBoard[y][x].piece, x, y);
						if ((pieceType == KNIGHT || pieceType == BISHOP) && !pieceMoved)
							opponentValue -= 5.0f;
					}
				}
				else
				{
					if (pieceOwner == player)
					{
						playerValue += currentBoard[y][x].piece->getValue();
						playerValue += getPieceSquareValue(currentBoard[y][x].piece, x, y);
						if ((pieceType == KNIGHT || pieceType == BISHOP) && !pieceMoved)
							playerValue -= 5.0f;
					}
					else
					{
						opponentValue += currentBoard[y][x].piece->getValue();
						opponentValue += getPieceSquareValue(currentBoard[y][x].piece, getOppositeSquare(x), getOppositeSquare(y));
						if ((pieceType == KNIGHT || pieceType == BISHOP) && !pieceMoved)
							opponentValue -= 5.0f;
					}
				}
			}
		}
	}
	float boardValue = playerValue / opponentValue;
	return (boardValue);
}

float	depthCalculation(Data &data, BoardSquare currentBoard[8][8], int movingPlayer, int calcPlayer, int currentDepth, int totalDepth)
{
	float value;

	if (movingPlayer == calcPlayer)
		value = -10000;
	else
		value = 100005;

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
						// std::cout << currentDepth << " " << pieceX << pieceY << " " << getTargetX << getTargetY() << "\n";
						if ((!currentBoard[targetY][targetX].piece || (currentBoard[targetY][targetX].piece && currentBoard[targetY][targetX].piece->getOwner() != movingPlayer))
							&& isMovePossible(data, currentBoard, currentBoard[pieceY][pieceX].piece, pieceX, pieceY, targetX - pieceX, targetY - pieceY, false))
							// && !possibleMoveCheck(data, currentBoard[pieceY][pieceX].piece, pieceX, pieceY, getTargetX, getTargetY()))
						{
							BoardSquare	copyBoard[8][8];
							for (int y = 0; y < 8; ++y)
								for (int x = 0; x < 8; ++x)
									copyBoard[y][x].piece = currentBoard[y][x].piece;

							if (currentDepth < totalDepth)
							{
								copyBoard[targetY][targetX].piece = copyBoard[pieceY][pieceX].piece;
								copyBoard[pieceY][pieceX].piece = NULL;
								if (totalDepth == currentDepth + 1)
								{
									value = calculateBoard(data, copyBoard, calcPlayer, 100);
									return (value);
								}
								float ret = depthCalculation(data, copyBoard, (movingPlayer  + 1) % 2, calcPlayer, currentDepth + 1, totalDepth);
								if ((movingPlayer == calcPlayer && ret > value) || (movingPlayer != calcPlayer && ret < value))
								{
									value = ret;
								}
							}
						}
					}
				}
			}
		}
	}
	// if (value > 10000)
	// 	std::cout << value << "\n";
	return (value);
}

float	getBestMove(Data &data, BoardSquare currentBoard[8][8], BoardSquare currentSquare, int pieceX, int pieceY, int player, std::vector<Move> &allMoves)
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

				// std::cout << (char)(pieceX + '0' + 17) << pieceY + 1 << " -> " << (char)(x + '0' + 17) << y + 1;
				float currentMove = depthCalculation(data, currentBoard, otherPlayer, player, data.currentDepth, 5);

				allMoves.push_back(Move(pieceX, pieceY, x, y, currentMove));
				data.savedMoves++;
				// std::cout << " = " << allMoves[data.savedMoves - 1].getEvalPoints() << "\n";

				currentBoard[pieceY][pieceX].piece = currentBoard[y][x].piece;
				if (deletedPiece)
					currentBoard[y][x].piece = deletedPiece;
				else
					currentBoard[y][x].piece = NULL;
			}
		}
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
			std::vector<Move> allMoves;

			data.currentDepth = 1;
			// data.depth = 3;
			// std::vector<std::thread> threads;
			for (int y = 0; y < 8; ++y)
			{
				for (int x = 0; x < 8; ++x)
				{
					if (data.square[y][x].piece && data.square[y][x].piece->getOwner() == player)
					{
						getBestMove(data, currentBoard, currentBoard[y][x], x, y, player, allMoves);
					}
				}
			}
			// for (auto& th : threads)
			// 	th.join();
			std::sort(allMoves.begin(), allMoves.end());
			Move bestMove(allMoves[0]);
			allMoves.clear();
			data.savedMoves = 0;

			data.square[bestMove.getStartY()][bestMove.getStartX()].piece->setHasMoved(true);
			ChessPiece *deletedPiece = NULL;
			if (data.square[bestMove.getTargetY()][bestMove.getTargetX()].piece)
				deletedPiece = data.square[bestMove.getTargetY()][bestMove.getTargetX()].piece;
			data.square[bestMove.getTargetY()][bestMove.getTargetX()].piece = data.square[bestMove.getStartY()][bestMove.getStartX()].piece;
			data.square[bestMove.getStartY()][bestMove.getStartX()].piece = NULL;
			if (data.moveNbr < data.history.size() - 1)
			{
				int	elemToDelete = (int) data.history.size() - 1 - data.moveNbr;
				for (int i = 0; i < elemToDelete; ++i)
					data.history.pop_back();
			}

			int pieceOwner = data.square[bestMove.getTargetY()][bestMove.getTargetX()].piece->getOwner();
			if (data.square[bestMove.getTargetY()][bestMove.getTargetX()].piece->getType() == PAWN
				&& ((bestMove.getTargetY() == 7 && pieceOwner == BLACK_P) || (bestMove.getTargetY() == 0 && pieceOwner == WHITE_P)))
				data.square[bestMove.getTargetY()][bestMove.getTargetX()].piece->setType(QUEEN);
			if (data.square[bestMove.getTargetY()][bestMove.getTargetX()].piece->getType() == KING)
			{
				data.kingPosX[pieceOwner] = bestMove.getTargetX();
				data.kingPosY[pieceOwner] = bestMove.getTargetY();
			}

			History	history;
			history.movedPiece = data.grabbedPiece;
			if (deletedPiece != nullptr)
				history.removedPiece = deletedPiece;
			history.fromX = bestMove.getStartX();
			history.fromY = bestMove.getStartY();
			history.toX = bestMove.getTargetX();
			history.toY = bestMove.getTargetY();
			data.history.push_back(history);
			data.moveNbr++;
			data.lastMove[0] = &data.square[bestMove.getStartY()][bestMove.getStartX()];
			data.lastMove[1] = &data.square[bestMove.getTargetY()][bestMove.getTargetX()];
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
