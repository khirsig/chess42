/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 11:11:38 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/20 14:53:51 by khirsig          ###   ########.fr       */
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
	// switch (piece->getType()) {
	// 	case PAWN :
	// 		return (pawnFieldValues[y][x]);
	// 	case BISHOP :
	// 		return (bishopFieldValues[y][x]);
	// 	case KNIGHT :
	// 		return (knightFieldValues[y][x]);
	// 	case ROOK :
	// 		return (rookFieldValues[y][x]);
	// 	case QUEEN :
	// 		return (queenFieldValues[y][x]);
	// 	case KING :
	// 		return (kingFieldValues[y][x]);
	// }
	return (mgFieldValues[piece->getType()][y * x]);
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
				if (player == BLACK_P)
				{
					if (pieceOwner == player)
					{
						playerValue += chessBoard.square[y][x].piece->getValue();
						playerValue += getPieceSquareValue(chessBoard.square[y][x].piece, getOppositeSquare(x), getOppositeSquare(y));
					}
					else
					{
						opponentValue += chessBoard.square[y][x].piece->getValue();
						opponentValue += getPieceSquareValue(chessBoard.square[y][x].piece, x, y);
					}
				}
				else
				{
					if (pieceOwner == player)
					{
						playerValue += chessBoard.square[y][x].piece->getValue();
						playerValue += getPieceSquareValue(chessBoard.square[y][x].piece, x, y);
					}
					else
					{
						opponentValue += chessBoard.square[y][x].piece->getValue();
						opponentValue += getPieceSquareValue(chessBoard.square[y][x].piece, getOppositeSquare(x), getOppositeSquare(y));
					}
				}
			}
		}
	}
	float boardValue = playerValue / opponentValue;
	// if (lookForCheckmate(chessBoard))
	// {
	// 	boardValue += 10000;
	// }
	return (boardValue);
}

ChessPiece	*movePiece(Board &chessBoard, int pieceX, int pieceY, int targetX, int targetY)
{
	ChessPiece *deletedPiece = chessBoard.square[targetY][targetX].piece;
	chessBoard.square[targetY][targetX].piece = chessBoard.square[pieceY][pieceX].piece;
	chessBoard.square[pieceY][pieceX].piece = nullptr;

	int pieceOwner = chessBoard.square[targetY][targetX].piece->getOwner();
	if (chessBoard.square[targetY][targetX].piece->getType() == PAWN
		&& ((targetY == 7 && pieceOwner == BLACK_P) || (targetY == 0 && pieceOwner == WHITE_P)))
	{
		chessBoard.square[targetY][targetX].piece->setType(QUEEN);
		chessBoard.square[targetY][targetX].piece->promotePawn();
	}

	if (chessBoard.square[targetY][targetX].piece->getType() == KING)
	{
		chessBoard.kingPosX[pieceOwner] = targetX;
		chessBoard.kingPosY[pieceOwner] = targetY;
	}
	return (deletedPiece);
}

void	revertMovePiece(Board &chessBoard, int pieceX, int pieceY, int targetX, int targetY, ChessPiece *deletedPiece)
{
	chessBoard.square[pieceY][pieceX].piece = chessBoard.square[targetY][targetX].piece;
	chessBoard.square[targetY][targetX].piece = deletedPiece;

	int pieceOwner = chessBoard.square[pieceY][pieceX].piece->getOwner();
	if (chessBoard.square[pieceY][pieceX].piece->getPromotedPawn() == true)
	{
		chessBoard.square[pieceY][pieceX].piece->setType(PAWN);
		chessBoard.square[pieceY][pieceX].piece->demotePawn();
	}

	if (chessBoard.square[pieceY][pieceX].piece->getType() == KING)
	{
		chessBoard.kingPosX[pieceOwner] = pieceX;
		chessBoard.kingPosY[pieceOwner] = pieceY;
	}
}

float	depthCalculation(Board &chessBoard, int movingPlayer, int calcPlayer, int currentDepth, int totalDepth, float alpha, float beta)
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
								ChessPiece *deletedPiece = movePiece(chessBoard, pieceX, pieceY, targetX, targetY);
								if (totalDepth == currentDepth + 1)
								{
									value = calculateBoard(chessBoard, calcPlayer, 100);
									revertMovePiece(chessBoard, pieceX, pieceY, targetX, targetY, deletedPiece);
									return (value);
								}
								float ret = depthCalculation(chessBoard, (movingPlayer  + 1) % 2, calcPlayer, currentDepth + 1, totalDepth, alpha, beta);
								chessBoard.iterations++;
								revertMovePiece(chessBoard, pieceX, pieceY, targetX, targetY, deletedPiece);
								if ((movingPlayer == calcPlayer && ret > value) || (movingPlayer != calcPlayer && ret < value))
								{
									value = ret;
									if (movingPlayer == calcPlayer)
										alpha = value;
									else
										beta = value;
								}
								if (beta <= alpha)
									return (value) ;
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
				ChessPiece *deletedPiece = movePiece(chessBoard, pieceX, pieceY, x, y);

				// std::cout << (char)(pieceX + '0' + 17) << pieceY + 1 << " -> " << (char)(x + '0' + 17) << y + 1;
				chessBoard.iterations++;
				float currentMove;
				if (player == WHITE_P)
					currentMove = depthCalculation(chessBoard, otherPlayer, player, 1, DEPTH_WHITE, -INFINITY, INFINITY);
				else
					currentMove = depthCalculation(chessBoard, otherPlayer, player, 1, DEPTH_BLACK, -INFINITY, INFINITY);
				// std::cout << " = " << currentMove << std::endl;

				allMoves.push_back(Move(pieceX, pieceY, x, y, currentMove));

				revertMovePiece(chessBoard, pieceX, pieceY, x, y, deletedPiece);
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
		chessBoard.iterations = 0;
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
			std::cout << "ITERATIONS: " << chessBoard.iterations << std::endl;
			std::sort(allMoves.begin(), allMoves.end());
			Move bestMove(allMoves[0]);
			allMoves.clear();

			ChessPiece *deletedPiece = movePiece(chessBoard, bestMove.getStartX(), bestMove.getStartY(), bestMove.getTargetX(), bestMove.getTargetY());
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
