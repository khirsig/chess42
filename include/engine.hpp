/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   engine.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/05 11:20:38 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/29 14:58:06 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENGINE_HPP
# define ENGINE_HPP

# define DEPTH_WHITE 5
# define DEPTH_BLACK 5

# define PIECE_VALUE_FACTOR 1.2f
# define PIECE_SQUARE_FACTOR 1.5f
# define CASTLE_FACTOR 0.8f
# define DEVELOPMENT_FACTOR 1.5f
# define MOBILITY_FACTOR 1.0f

# include <algorithm>
# include <thread>

static int kingMGFieldValues[64] = {
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-30,-40,-40,-50,-50,-40,-40,-30,
	-20,-30,-30,-40,-40,-30,-30,-20,
	-10,-20,-20,-20,-20,-20,-20,-10,
	 20, 20,  0,  0,  0,  0, 20, 20,
	 20, 30, 10,  0,  0, 10, 30, 20
};
static int kingEGFieldValues[64] = {
	-50,-40,-30,-20,-20,-30,-40,-50,
	-30,-20,-10,  0,  0,-10,-20,-30,
	-30,-10, 20, 30, 30, 20,-10,-30,
	-30,-10, 30, 40, 40, 30,-10,-30,
	-30,-10, 30, 40, 40, 30,-10,-30,
	-30,-10, 20, 30, 30, 20,-10,-30,
	-30,-30,  0,  0,  0,  0,-30,-30,
	-50,-30,-30,-30,-30,-30,-30,-50
};
static int queenFieldValues[64] = {
	-20,-10,-10, -5, -5,-10,-10,-20,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-10,  0,  5,  5,  5,  5,  0,-10,
	 -5,  0,  5,  5,  5,  5,  0, -5,
	  0,  0,  5,  5,  5,  5,  0, -5,
	-10,  5,  5,  5,  5,  5,  0,-10,
	-10,  0,  5,  0,  0,  0,  0,-10,
	-20,-10,-10, -5, -5,-10,-10,-20
};
static int rookFieldValues[64] = {
	  0,  0,  0,  0,  0,  0,  0,  0,
	  5, 10, 10, 10, 10, 10, 10,  5,
	 -5,  0,  0,  0,  0,  0,  0, -5,
	 -5,  0,  0,  0,  0,  0,  0, -5,
	 -5,  0,  0,  0,  0,  0,  0, -5,
	 -5,  0,  0,  0,  0,  0,  0, -5,
	 -5,  0,  0,  0,  0,  0,  0, -5,
	  0,  0,  0,  5,  5,  0,  0,  0
};
static int bishopFieldValues[64] = {
	-20,-10,-10,-10,-10,-10,-10,-20,
	-10,  0,  0,  0,  0,  0,  0,-10,
	-10,  0,  5, 10, 10,  5,  0,-10,
	-10,  5,  5, 10, 10,  5,  5,-10,
	-10,  0, 10, 10, 10, 10,  0,-10,
	-10, 10, 10, 10, 10, 10, 10,-10,
	-10,  5,  0,  0,  0,  0,  5,-10,
	-20,-10,-10,-10,-10,-10,-10,-20
};
static int knightFieldValues[64] = {
	-50,-40,-30,-30,-30,-30,-40,-50,
	-40,-20,  0,  0,  0,  0,-20,-40,
	-30,  0, 10, 15, 15, 10,  0,-30,
	-30,  5, 15, 20, 20, 15,  5,-30,
	-30,  0, 15, 20, 20, 15,  0,-30,
	-30,  5, 10, 15, 15, 10,  5,-30,
	-40,-20,  0,  5,  5,  0,-20,-40,
	-50,-40,-30,-30,-30,-30,-40,-50,
};
static int pawnFieldValues[64] = {
	 0,  0,  0,  0,  0,  0,  0,  0,
	50, 50, 50, 50, 50, 50, 50, 50,
	10, 10, 20, 30, 30, 20, 10, 10,
	 5,  5, 10, 25, 25, 10,  5,  5,
	 0,  0,  0, 20, 20,  0,  0,  0,
	 5, -5,-10,  0,  0,-10, -5,  5,
	 5, 10, 10,-20,-20, 10, 10,  5,
	 0,  0,  0,  0,  0,  0,  0,  0
};
static int pawnOPFieldValues[64] = {
	 0,  0,  0,  0,  0,  0,  0,  0,
	50, 50, 50, 50, 50, 50, 50, 50,
	10, 10, 20, 35, 35, 20, 10, 10,
	-20, 5, 10, 35, 35, 10,  5, -20,
	-20, -5, -5, 30, 30, -5, -5, -20,
	 5, -5,-10, 10, 10,-10, -5,  5,
	25, 20, 20,-20,-20, 20, 20, 25,
	 0,  0,  0,  0,  0,  0,  0,  0
};
static int* mgFieldValues[6] =
{
	pawnFieldValues,
	bishopFieldValues,
	knightFieldValues,
	rookFieldValues,
	queenFieldValues,
	kingMGFieldValues
};
static int* egFieldValues[6] =
{
	pawnFieldValues,
	bishopFieldValues,
	knightFieldValues,
	rookFieldValues,
	queenFieldValues,
	kingEGFieldValues
};

static int endGameThreshold = 1350;

static std::mutex aiMutex;
static std::mutex barMutex;

class Move {
	public:
		Move() { _init = false; }
		Move(const Move &other)
			: _startX(other._startX),
			  _startY(other._startY),
			  _targetX(other._targetX),
			  _targetY(other._targetY),
			  _evaluatedPoints(other._evaluatedPoints) { _init = true; }
		Move(int sX, int sY, int tX, int tY, float eP)
			: _startX(sX),
			  _startY(sY),
			  _targetX(tX),
			  _targetY(tY),
			  _evaluatedPoints(eP) { _init = true; }

		void	setEvalPoints(float newEval) { _evaluatedPoints = newEval; }

		int		getStartX() const { return (_startX); }
		int		getStartY() const { return (_startY); }
		int		getTargetX() const { return (_targetX); }
		int		getTargetY() const { return (_targetY); }
		float	getEvalPoints() const { return (_evaluatedPoints); }
		float	getInit() const { return (_init); }
	private:
		bool _init = true;
		float	_evaluatedPoints;
		int	_startX;
		int	_startY;
		int	_targetX;
		int	_targetY;
};


#endif