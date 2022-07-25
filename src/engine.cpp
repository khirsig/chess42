/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 11:11:38 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/25 08:44:23 by khirsig          ###   ########.fr       */
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

Board	*copyBoard(Board &chessBoard)
{
	Board *copyBoard = new Board();
	for (int y = 0; y < 8; ++y)
		for (int x = 0; x < 8; ++x)
		{
			if (chessBoard.square[y][x].piece)
				copyBoard->square[y][x].piece = new ChessPiece(*(chessBoard.square[y][x].piece));
			else
				copyBoard->square[y][x].piece = nullptr;
		}
	copyBoard->endgame =  chessBoard.endgame;
	copyBoard->checkmate = chessBoard.checkmate;
	copyBoard->iterations = chessBoard.iterations;
	copyBoard->moveTurn = chessBoard.moveTurn;
	copyBoard->stalemate = chessBoard.stalemate;
	for (int i = 0; i < 2; ++i)
	{
		copyBoard->castled[i] = chessBoard.castled[i];
		copyBoard->kingCheck[i] = chessBoard.kingCheck[i];
		copyBoard->kingPosX[i] = chessBoard.kingPosX[i];
		copyBoard->kingPosY[i] = chessBoard.kingPosY[i];
	}
	return (copyBoard);
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

float	calculateBoard(Board &chessBoard, int player, bool relative)
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
	if (relative == false)
		return (((playerPieceValue + (playerSquareValue / 10)) - (opponentPieceValue + (opponentSquareValue / 10))) / 100);

	if (player == WHITE_P)
	{
		if (chessBoard.castled[WHITE_P] == -1)
			playerSquareValue -= 60;
		else
		{
			if (chessBoard.square[chessBoard.kingPosY[WHITE_P] - 1][chessBoard.kingPosX[WHITE_P]].piece == NULL)
				playerSquareValue -= 20;
			if (chessBoard.square[chessBoard.kingPosY[WHITE_P] - 1][chessBoard.kingPosX[WHITE_P] - 1].piece == NULL)
				playerSquareValue -= 20;
			if (chessBoard.square[chessBoard.kingPosY[WHITE_P] - 1][chessBoard.kingPosX[WHITE_P] + 1].piece == NULL)
				playerSquareValue -= 20;
		}
		if (chessBoard.castled[BLACK_P] == -1)
			opponentSquareValue -= 60;
		else
		{
			if (chessBoard.square[chessBoard.kingPosY[BLACK_P] + 1][chessBoard.kingPosX[BLACK_P]].piece == NULL)
				opponentSquareValue -= 20;
			if (chessBoard.square[chessBoard.kingPosY[BLACK_P] + 1][chessBoard.kingPosX[BLACK_P] - 1].piece == NULL)
				opponentSquareValue -= 20;
			if (chessBoard.square[chessBoard.kingPosY[BLACK_P] + 1][chessBoard.kingPosX[BLACK_P] + 1].piece == NULL)
				opponentSquareValue -= 20;
		}
	}
	else
	{
		if (chessBoard.castled[BLACK_P] == -1)
			playerSquareValue -= 60;
		else
		{
			if (chessBoard.square[chessBoard.kingPosY[BLACK_P] + 1][chessBoard.kingPosX[BLACK_P]].piece == NULL)
				playerSquareValue -= 20;
			if (chessBoard.square[chessBoard.kingPosY[BLACK_P] + 1][chessBoard.kingPosX[BLACK_P] - 1].piece == NULL)
				playerSquareValue -= 20;
			if (chessBoard.square[chessBoard.kingPosY[BLACK_P] + 1][chessBoard.kingPosX[BLACK_P] + 1].piece == NULL)
				playerSquareValue -= 20;
		}
		if (chessBoard.castled[WHITE_P] == -1)
			opponentSquareValue -= 60;
		else
		{
			if (chessBoard.square[chessBoard.kingPosY[WHITE_P] - 1][chessBoard.kingPosX[WHITE_P]].piece == NULL)
				opponentSquareValue -= 20;
			if (chessBoard.square[chessBoard.kingPosY[WHITE_P] - 1][chessBoard.kingPosX[WHITE_P] - 1].piece == NULL)
				opponentSquareValue -= 20;
			if (chessBoard.square[chessBoard.kingPosY[WHITE_P] - 1][chessBoard.kingPosX[WHITE_P] + 1].piece == NULL)
				opponentSquareValue -= 20;
		}
	}
	if (chessBoard.endgame == false && playerPieceValue < endGameThreshold && opponentPieceValue < endGameThreshold)
	{
		chessBoard.endgame = true;
		std::cout << "ENDGAME ACTIVATED\n";
	}
	float boardValue = (playerPieceValue + playerSquareValue) / (opponentPieceValue + opponentSquareValue);
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
	int castledSave = -1;
	if (chessBoard.castled[player] == chessBoard.moveTurn  && targetX == 7)
	{
		chessBoard.square[pieceY][pieceX].piece = chessBoard.square[targetY][6].piece;
		chessBoard.square[targetY][6].piece = nullptr;
		chessBoard.square[targetY][5].piece = nullptr;
		castledSave = chessBoard.castled[player];
		chessBoard.castled[player] = -1;
	}
	else if (chessBoard.castled[player] == chessBoard.moveTurn && targetX == 0)
	{
		chessBoard.square[pieceY][pieceX].piece = chessBoard.square[targetY][2].piece;
		chessBoard.square[targetY][2].piece = nullptr;
		chessBoard.square[targetY][3].piece = nullptr;
		castledSave = chessBoard.castled[player];
		chessBoard.castled[player] = -1;
	}
	else
		chessBoard.square[pieceY][pieceX].piece = chessBoard.square[targetY][targetX].piece;
	chessBoard.square[targetY][targetX].piece = deletedPiece;

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
		if (castledSave == chessBoard.moveTurn && deletedPiece)
		{
			deletedPiece->setHasMoved(false);
			deletedPiece->setHasMovedTurn(-1);
		}
	}
	chessBoard.moveTurn--;
}

float	depthCalculation(Board &chessBoard, int movingPlayer, int calcPlayer, int currentDepth, int totalDepth, float alpha, float beta, bool relative)
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
									value = calculateBoard(chessBoard, calcPlayer, relative);
									revertMovePiece(chessBoard, pieceX, pieceY, targetX, targetY, deletedPiece, movingPlayer);
									return (value);
								}
								float ret = depthCalculation(chessBoard, (movingPlayer  + 1) % 2, calcPlayer, currentDepth + 1, totalDepth, alpha, beta, relative);
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

float	getBestMove(Board *chessBoard, int pieceX, int pieceY, int player, std::vector<Move> &allMoves, int depth, bool relative)
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
			if (isMovePossible(*chessBoard, pieceX, pieceY, x - pieceX, y - pieceY, false))
				// && !possibleMoveCheck(chessBoard, pieceX, pieceY, x, y))
			{
				ChessPiece *deletedPiece = movePiece(*chessBoard, pieceX, pieceY, x, y);

				chessBoard->iterations++;
				float currentMove;
				if (player == WHITE_P)
					currentMove = depthCalculation(*chessBoard, otherPlayer, player, 1, depth, -INFINITY, INFINITY, relative);
				else
					currentMove = depthCalculation(*chessBoard, otherPlayer, player, 1, depth, -INFINITY, INFINITY, relative);

				aiMutex.lock();
				allMoves.push_back(Move(pieceX, pieceY, x, y, currentMove));
				aiMutex.unlock();

				revertMovePiece(*chessBoard, pieceX, pieceY, x, y, deletedPiece, player);
			}
		}
	}
	delete chessBoard;
	return (bestMove);
}

Move	lookForMoves(Board *chessBoard, int player, int depth, bool relative)
{
	chessBoard->iterations = 0;
	std::vector<Move> allMoves;
	std::vector<std::thread> threads;

	for (int y = 0; y < 8; ++y)
	{
		for (int x = 0; x < 8; ++x)
		{
			if (chessBoard->square[y][x].piece && chessBoard->square[y][x].piece->getOwner() == player)
			{
				Board *cpy = copyBoard(*chessBoard);
				threads.push_back(std::thread(getBestMove, cpy, x, y, player, std::ref(allMoves), depth, relative));
			}
		}
	}
	for (auto& th : threads)
		th.join();
	exitBarDepth = false;
	std::sort(allMoves.begin(), allMoves.end());
	Move bestMove(allMoves[0]);
	allMoves.clear();

	delete chessBoard;
	return (bestMove);
}

void	getBestAIMove(Data &data, Board &chessBoard, int player)
{
	data.aiThinking = true;
	Board	*cpy = copyBoard(chessBoard);

	int depth;
	if (player == WHITE_P)
		depth = DEPTH_WHITE;
	else
		depth = DEPTH_BLACK;
	data.aiThreadMove = std::async(lookForMoves, cpy, player, depth, true);
}

void	executeAIMove(Data &data, Board &chessBoard, int player)
{
	if (data.aiThreadMove.wait_for(std::chrono::milliseconds(1)) == std::future_status::ready)
	{
		Move bestMove = data.aiThreadMove.get();
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
		data.currentBarDepth = 5;
		data.aiThinking = false;
		data.waitAI = 0;
	}
}
