/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evalBar.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/23 05:14:35 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/29 08:47:23 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Data.hpp"

void	runEvalBar(Data &data, Board &chessBoard)
{
	data.barBoard = copyBoard(chessBoard);
	if (data.currentBarDepth == 3)
		data.nextBarDepth = data.currentBarDepth + 1;

	data.barThreadMove = std::async(lookForMoves, data.barBoard, data.turn, data.currentBarDepth, false);
	std::cout << "Current Bar Depth: " << data.currentBarDepth << " evaluated for player " << data.turn << "\n";
	data.currentBarDepth++;
}

void	drawEvalBar(Data &data, Board &chessBoard)
{
	if (data.barThreadMove.valid() && data.barThreadMove.wait_for(std::chrono::milliseconds(1)) == std::future_status::ready)
	{
		Move bestMove = data.barThreadMove.get();
		data.barEval = bestMove.getEvalPoints();
		if (data.nextBarDepth < 6)
			data.nextBarDepth = data.currentBarDepth + 1;
		if (data.turn == WHITE_P)
			data.barEval *= -1;
	}

	int endBlack = (float)SCREEN_HEIGHT / 20 * (data.barEval + 10);

	DrawRectangle(SCREEN_WIDTH, 0, SCREEN_WIDTH * 1.03, endBlack, BLACK);
	DrawRectangle(SCREEN_WIDTH, endBlack, SCREEN_WIDTH * 1.03, SCREEN_HEIGHT, RAYWHITE);
	float	evalPts = data.barEval;
	bool	whiteInFront;
	if (data.barEval < 0)
	{
		whiteInFront = true;
		evalPts *= -1;
	}
	else
	{
		whiteInFront = false;
	}
	std::stringstream stream;
	stream << std::fixed << std::setprecision(1) << evalPts;
	std::string points = stream.str();
	int textLen = MeasureText(points.c_str(), SCREEN_WIDTH / 80);
	if (whiteInFront)
		DrawText(points.c_str(), SCREEN_WIDTH + ((SCREEN_WIDTH * 1.03 - SCREEN_WIDTH) / 2 - textLen / 2), SCREEN_HEIGHT - SCREEN_HEIGHT / 50, SCREEN_WIDTH / 80, BLACK);
	else
		DrawText(points.c_str(), SCREEN_WIDTH + ((SCREEN_WIDTH * 1.03 - SCREEN_WIDTH) / 2 - textLen / 2), SCREEN_HEIGHT / 50, SCREEN_WIDTH / 80, WHITE);
}
