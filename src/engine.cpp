/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 11:11:38 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/19 14:40:45 by khirsig          ###   ########.fr       */
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

float	calculateBoard(Board &chessBoard, int player, float factor)
{
	float playerValue = 0.0f;
	float opponentValue = 0.0f;

	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			if (chessBoard.square[y][x].piece)
			{
				int		pieceOwner = chessBoard.square[y][x].piece->getOwner();
				int		pieceType = chessBoard.square[y][x].piece->getType();
				bool	pieceMoved = chessBoard.square[y][x].piece->getHasMoved();
				if (player == WHITE_P)
				{
					if (pieceOwner == player)
					{
						playerValue += chessBoard.square[y][x].piece->getValue();
						playerValue += getPieceSquareValue(chessBoard.square[y][x].piece, getOppositeSquare(x), getOppositeSquare(y));
						if ((pieceType == KNIGHT || pieceType == BISHOP) && !pieceMoved)
							playerValue -= 5.0f;
					}
					else
					{
						opponentValue += chessBoard.square[y][x].piece->getValue();
						opponentValue += getPieceSquareValue(chessBoard.square[y][x].piece, x, y);
						if ((pieceType == KNIGHT || pieceType == BISHOP) && !pieceMoved)
							opponentValue -= 5.0f;
					}
				}
				else
				{
					if (pieceOwner == player)
					{
						playerValue += chessBoard.square[y][x].piece->getValue();
						playerValue += getPieceSquareValue(chessBoard.square[y][x].piece, x, y);
						if ((pieceType == KNIGHT || pieceType == BISHOP) && !pieceMoved)
							playerValue -= 5.0f;
					}
					else
					{
						opponentValue += chessBoard.square[y][x].piece->getValue();
						opponentValue += getPieceSquareValue(chessBoard.square[y][x].piece, getOppositeSquare(x), getOppositeSquare(y));
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

float	depthCalculation(Board &chessBoard, int movingPlayer, int calcPlayer, int currentDepth, int totalDepth)
{
	float value;

	if (movingPlayer == calcPlayer)
		value = -10000;
	else
		value = 10000;

	// std::cout << currentDepth << "/" << totalDepth << std::endl;
	for (int pieceY = 0; pieceY < 8; ++pieceY)
	{
		for (int pieceX = 0; pieceX < 8; ++pieceX)
		{
			if (chessBoard.square[pieceY][pieceX].piece && chessBoard.square[pieceY][pieceX].piece->getOwner() == movingPlayer)
			{
				for (int targetY = 0; targetY < 8; ++targetY)
				{
					for (int targetX = 0; targetX < 8; ++targetX)
					{
						if ((!chessBoard.square[targetY][targetX].piece || (chessBoard.square[targetY][targetX].piece
							&& chessBoard.square[targetY][targetX].piece->getOwner() != movingPlayer))
							&& isMovePossible(chessBoard, pieceX, pieceY, targetX - pieceX, targetY - pieceY, false)
							&& !possibleMoveCheck(chessBoard, pieceX, pieceY, targetX, targetY))
						{
							if (currentDepth < totalDepth)
							{
								ChessPiece *deletedPiece = chessBoard.square[targetY][targetX].piece;
								chessBoard.square[targetY][targetX].piece = chessBoard.square[pieceY][pieceX].piece;
								chessBoard.square[pieceY][pieceX].piece = nullptr;
								if (totalDepth == currentDepth + 1)
								{
									value = calculateBoard(chessBoard, calcPlayer, 100);
									chessBoard.square[pieceY][pieceX].piece = chessBoard.square[targetY][targetX].piece;
									chessBoard.square[targetY][targetX].piece = deletedPiece;
									return (value);
								}
								float ret = depthCalculation(chessBoard, (movingPlayer  + 1) % 2, calcPlayer, currentDepth + 1, totalDepth);
								chessBoard.square[pieceY][pieceX].piece = chessBoard.square[targetY][targetX].piece;
								chessBoard.square[targetY][targetX].piece = deletedPiece;
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
	return (value);
}

float	getBestMove(Board &chessBoard, int pieceX, int pieceY, int player, std::vector<Move> &allMoves)
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
			if ((!chessBoard.square[y][x].piece || chessBoard.square[y][x].piece->getOwner() != player)
				&& isMovePossible(chessBoard, pieceX, pieceY, x - pieceX, y - pieceY, false)
				&& !possibleMoveCheck(chessBoard, pieceX, pieceY, x, y))
			{
				ChessPiece *deletedPiece = chessBoard.square[y][x].piece;
				chessBoard.square[y][x].piece = chessBoard.square[pieceY][pieceX].piece;
				chessBoard.square[pieceY][pieceX].piece = NULL;

				std::cout << (char)(pieceX + '0' + 17) << pieceY + 1 << " -> " << (char)(x + '0' + 17) << y + 1 << std::endl;
				float currentMove = depthCalculation(chessBoard, otherPlayer, player, 1, DEPTH);
				std::cout << " = " << currentMove;

				allMoves.push_back(Move(pieceX, pieceY, x, y, currentMove));

				chessBoard.square[pieceY][pieceX].piece = chessBoard.square[y][x].piece;
				chessBoard.square[y][x].piece = deletedPiece;
			}
		}
	}
	return (bestMove);
}

void	moveAI(Data &data, Board &chessBoard, int player)
{
	if (data.waitAI >= 180 && !chessBoard.checkmate)
	{
		if (lookForCheckmate(chessBoard))
		{
			chessBoard.checkmate = true;
			return ;
		}
		if (data.turn == player)
		{
			std::vector<Move> allMoves;

			for (int y = 0; y < 8; ++y)
			{
				for (int x = 0; x < 8; ++x)
				{
					if (chessBoard.square[y][x].piece && chessBoard.square[y][x].piece->getOwner() == player)
					{
						getBestMove(chessBoard, x, y, player, allMoves);
					}
				}
			}

			std::sort(allMoves.begin(), allMoves.end());
			Move bestMove(allMoves[0]);
			allMoves.clear();

			chessBoard.square[bestMove.getStartY()][bestMove.getStartX()].piece->setHasMoved(true);
			ChessPiece *deletedPiece = NULL;
			if (chessBoard.square[bestMove.getTargetY()][bestMove.getTargetX()].piece)
				deletedPiece = chessBoard.square[bestMove.getTargetY()][bestMove.getTargetX()].piece;
			chessBoard.square[bestMove.getTargetY()][bestMove.getTargetX()].piece = chessBoard.square[bestMove.getStartY()][bestMove.getStartX()].piece;
			chessBoard.square[bestMove.getStartY()][bestMove.getStartX()].piece = NULL;
			if (data.moveNbr < data.history.size() - 1)
			{
				int	elemToDelete = (int) data.history.size() - 1 - data.moveNbr;
				for (int i = 0; i < elemToDelete; ++i)
					data.history.pop_back();
			}

			int pieceOwner = chessBoard.square[bestMove.getTargetY()][bestMove.getTargetX()].piece->getOwner();
			if (chessBoard.square[bestMove.getTargetY()][bestMove.getTargetX()].piece->getType() == PAWN
				&& ((bestMove.getTargetY() == 7 && pieceOwner == BLACK_P) || (bestMove.getTargetY() == 0 && pieceOwner == WHITE_P)))
				chessBoard.square[bestMove.getTargetY()][bestMove.getTargetX()].piece->setType(QUEEN);
			if (chessBoard.square[bestMove.getTargetY()][bestMove.getTargetX()].piece->getType() == KING)
			{
				chessBoard.kingPosX[pieceOwner] = bestMove.getTargetX();
				chessBoard.kingPosY[pieceOwner] = bestMove.getTargetY();
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
			data.lastMove[0] = &chessBoard.square[bestMove.getStartY()][bestMove.getStartX()];
			data.lastMove[1] = &chessBoard.square[bestMove.getTargetY()][bestMove.getTargetX()];
			toggleCheckBothPlayers(chessBoard);
			lookForCheckmate(chessBoard);
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
