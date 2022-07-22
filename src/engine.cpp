/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 11:11:38 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/22 12:21:06 by khirsig          ###   ########.fr       */
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

static float	getPieceSquareValue(ChessPiece *piece, int x, int y, bool endgame)
{
	if (endgame)
		return (egFieldValues[piece->getType()][y * x]);
	return (mgFieldValues[piece->getType()][y * x]);
}

float	calculateBoard(Board &chessBoard, int player, float factor)
{
	float playerPieceValue = 0.0f;
	float playerSquareValue = 0.0f;
	float opponentPieceValue = 0.0f;
	float opponentSquareValue = 0.0f;

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
						playerPieceValue += chessBoard.square[y][x].piece->getValue();
						playerSquareValue += getPieceSquareValue(chessBoard.square[y][x].piece, getOppositeSquare(x), getOppositeSquare(y), chessBoard.endgame);
						if ((pieceType == KNIGHT || pieceType == BISHOP) && !pieceMoved)
							playerSquareValue -= 50;
					}
					else
					{
						opponentPieceValue += chessBoard.square[y][x].piece->getValue();
						opponentSquareValue += getPieceSquareValue(chessBoard.square[y][x].piece, x, y, chessBoard.endgame);
						if ((pieceType == KNIGHT || pieceType == BISHOP) && !pieceMoved)
							opponentSquareValue -= 50;
					}
				}
				else
				{
					if (pieceOwner == player)
					{
						playerPieceValue += chessBoard.square[y][x].piece->getValue();
						playerSquareValue += getPieceSquareValue(chessBoard.square[y][x].piece, x, y, chessBoard.endgame);
						if ((pieceType == KNIGHT || pieceType == BISHOP) && !pieceMoved)
							playerSquareValue -= 50;
					}
					else
					{
						opponentPieceValue += chessBoard.square[y][x].piece->getValue();
						opponentSquareValue += getPieceSquareValue(chessBoard.square[y][x].piece, getOppositeSquare(x), getOppositeSquare(y), chessBoard.endgame);
						if ((pieceType == KNIGHT || pieceType == BISHOP) && !pieceMoved)
							opponentSquareValue -= 50;
					}
				}
			}
		}
	}
	// if (player == WHITE_P)
	// {
	// 	if (chessBoard.castled[WHITE_P] == -1)
	// 		playerSquareValue -= 60;
	// 	if (chessBoard.castled[BLACK_P] == -1)
	// 		opponentSquareValue -= 60;
	// }
	// else
	// {
	// 	if (chessBoard.castled[BLACK_P] == -1)
	// 		playerSquareValue -= 60;
	// 	if (chessBoard.castled[WHITE_P] == -1)
	// 		opponentSquareValue -= 60;
	// }
	if (chessBoard.endgame == false && playerPieceValue < endGameThreshold && opponentPieceValue < endGameThreshold)
	{
		chessBoard.endgame = true;
		std::cout << "ENDGAME ACTIVATED\n";
	}
	float boardValue = (playerPieceValue + playerSquareValue) - (opponentPieceValue + opponentSquareValue);
	return (boardValue);
}

ChessPiece	*movePiece(Board &chessBoard, int pieceX, int pieceY, int targetX, int targetY)
{
	chessBoard.moveTurn++;

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

	bool pieceHasMoved = chessBoard.square[targetY][targetX].piece->getHasMoved();
;
	if (!pieceHasMoved)
	{
		chessBoard.square[targetY][targetX].piece->setHasMoved(true);
		chessBoard.square[targetY][targetX].piece->setHasMovedTurn(chessBoard.moveTurn);
	}

	if (chessBoard.square[targetY][targetX].piece->getType() == KING)
	{
		if (targetX == 7 && deletedPiece && deletedPiece->getType() == ROOK
			&& deletedPiece->getHasMoved() == false
			&& pieceHasMoved == false)
		{
			chessBoard.square[pieceY][pieceX + 2].piece = chessBoard.square[targetY][targetX].piece;
			chessBoard.square[targetY][targetX].piece = nullptr;
			chessBoard.square[pieceY][pieceX + 1].piece = deletedPiece;
			chessBoard.kingPosX[pieceOwner] = pieceX + 2;
			chessBoard.castled[pieceOwner] = chessBoard.moveTurn;
			chessBoard.square[pieceY][pieceX + 1].piece->setHasMoved(true);
			chessBoard.square[pieceY][pieceX + 1].piece->setHasMovedTurn(chessBoard.moveTurn);
		}
		else if (targetX == 0 && deletedPiece && deletedPiece->getType() == ROOK
			&& deletedPiece->getHasMoved() == false
			&& pieceHasMoved == false)
		{
			chessBoard.square[pieceY][pieceX - 2].piece = chessBoard.square[targetY][targetX].piece;
			chessBoard.square[targetY][targetX].piece = nullptr;
			chessBoard.square[pieceY][pieceX - 1].piece = deletedPiece;
			chessBoard.kingPosX[pieceOwner] = pieceX - 2;
			chessBoard.castled[pieceOwner] = chessBoard.moveTurn;
			chessBoard.square[pieceY][pieceX - 1].piece->setHasMoved(true);
			chessBoard.square[pieceY][pieceX - 1].piece->setHasMovedTurn(chessBoard.moveTurn);
		}
		else
			chessBoard.kingPosX[pieceOwner] = targetX;
		chessBoard.kingPosY[pieceOwner] = targetY;
	}

	return (deletedPiece);
}

void	revertMovePiece(Board &chessBoard, int pieceX, int pieceY, int targetX, int targetY, ChessPiece *deletedPiece, int player)
{
	if (chessBoard.castled[player] == chessBoard.moveTurn  && targetX == 7)
	{
		std::cout << "SHORT CASTLE REVERSED " << chessBoard.castled[player] << " = " << chessBoard.moveTurn << std::endl;
		chessBoard.square[pieceY][pieceX].piece = chessBoard.square[targetY][6].piece;
		chessBoard.castled[player] = -1;
	}
	else if (chessBoard.castled[player] == chessBoard.moveTurn && targetX == 0)
	{
		chessBoard.square[pieceY][pieceX].piece = chessBoard.square[targetY][2].piece;
		chessBoard.castled[player] = -1;
		std::cout << "LONG CASTLE REVERSED" << std::endl;
	}
	else
		chessBoard.square[pieceY][pieceX].piece = chessBoard.square[targetY][targetX].piece;
	chessBoard.square[targetY][targetX].piece = deletedPiece;

	std::cout << pieceX << pieceY << " " << targetX << targetY << "\n";
	if (!chessBoard.square[pieceY][pieceX].piece)
		std::cout << "NULL\n";

	if (chessBoard.square[pieceY][pieceX].piece->getPromotedPawn() == true)
	{
		chessBoard.square[pieceY][pieceX].piece->setType(PAWN);
		chessBoard.square[pieceY][pieceX].piece->demotePawn();
	}

	if (chessBoard.square[pieceY][pieceX].piece->getType() == KING)
	{
		chessBoard.kingPosX[player] = pieceX;
		chessBoard.kingPosY[player] = pieceY;
	}

	if (chessBoard.square[pieceY][pieceX].piece->getHasMovedTurn() == chessBoard.moveTurn)
	{
		chessBoard.square[pieceY][pieceX].piece->setHasMovedTurn(-1);
		chessBoard.square[pieceY][pieceX].piece->setHasMoved(false);
		if (chessBoard.castled[player] == chessBoard.moveTurn && deletedPiece)
		{
			deletedPiece->setHasMoved(false);
			deletedPiece->setHasMovedTurn(-1);
		}
	}
	chessBoard.moveTurn--;
}

float	depthCalculation(Board &chessBoard, int movingPlayer, int calcPlayer, int currentDepth, int totalDepth, float alpha, float beta)
{
	float value;

	if (movingPlayer == calcPlayer)
		value = -10000;
	else
		value = 10000;

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
						if (isMovePossible(chessBoard, pieceX, pieceY, targetX - pieceX, targetY - pieceY, false))
							// && !possibleMoveCheck(chessBoard, pieceX, pieceY, targetX, targetY))
						{
							if (currentDepth < totalDepth)
							{
								ChessPiece *deletedPiece = movePiece(chessBoard, pieceX, pieceY, targetX, targetY);
								if (totalDepth == currentDepth + 1)
								{
									value = calculateBoard(chessBoard, calcPlayer, 100);
									revertMovePiece(chessBoard, pieceX, pieceY, targetX, targetY, deletedPiece, movingPlayer);
									return (value);
								}
								float ret = depthCalculation(chessBoard, (movingPlayer  + 1) % 2, calcPlayer, currentDepth + 1, totalDepth, alpha, beta);
								chessBoard.iterations++;
								revertMovePiece(chessBoard, pieceX, pieceY, targetX, targetY, deletedPiece, movingPlayer);
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
			if (isMovePossible(chessBoard, pieceX, pieceY, x - pieceX, y - pieceY, false))
				// && !possibleMoveCheck(chessBoard, pieceX, pieceY, x, y))
			{
				ChessPiece *deletedPiece = movePiece(chessBoard, pieceX, pieceY, x, y);

				chessBoard.iterations++;
				float currentMove;
				if (player == WHITE_P)
					currentMove = depthCalculation(chessBoard, otherPlayer, player, 1, DEPTH_WHITE, -INFINITY, INFINITY);
				else
					currentMove = depthCalculation(chessBoard, otherPlayer, player, 1, DEPTH_BLACK, -INFINITY, INFINITY);

				allMoves.push_back(Move(pieceX, pieceY, x, y, currentMove));

				revertMovePiece(chessBoard, pieceX, pieceY, x, y, deletedPiece, player);
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

			data.moveNbr++;
			if (chessBoard.castled[player] != chessBoard.moveTurn)
			{
				data.lastMove[0] = &chessBoard.square[bestMove.getStartY()][bestMove.getStartX()];
				data.lastMove[1] = &chessBoard.square[bestMove.getTargetY()][bestMove.getTargetX()];
			}
			else
			{
				if (chessBoard.kingPosX[player] == 6)
					data.lastMove[0] = &chessBoard.square[chessBoard.kingPosY[player]][chessBoard.kingPosX[player] - 1];
				else
					data.lastMove[0] = &data.chessBoard.square[chessBoard.kingPosY[player]][chessBoard.kingPosX[player] + 1];
				data.lastMove[1] = &data.chessBoard.square[chessBoard.kingPosY[player]][chessBoard.kingPosX[player]];
			}

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
