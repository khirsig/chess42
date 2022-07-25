/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChessPiece.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 19:33:09 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/25 16:21:52 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHESSPIECE_HPP
# define CHESSPIECE_HPP

# define WHITE_P 0
# define BLACK_P 1

# define PAWN 0
# define BISHOP 1
# define KNIGHT 2
# define ROOK 3
# define QUEEN 4
# define KING 5

class ChessPiece {
	public:
		ChessPiece() { }
		ChessPiece(int owner, int type, int value) : _owner(owner), _type(type), _value(value) { }
		~ChessPiece() { }

		void	setGrabbed(bool isGrabbed) { this->_grabbed = isGrabbed; }
		void	setHasMoved(bool hasMoved) { this->_hasMoved = hasMoved; }
		void	setType(int type) { this->_type = type; }
		void	setHasMovedTurn(int hasMovedTurn) { this->_hasMovedTurn = hasMovedTurn; }
		int		getOwner() const { return (this->_owner); }
		int		getType() const { return (this->_type); }
		int		getValue() const { return (this->_value); }
		int		getHasMovedTurn() const { return (this->_hasMovedTurn); }
		bool	getGrabbed() const { return (this->_grabbed); }
		bool	getHasMoved() const { return (this->_hasMoved); }
		int		getPromotedPawn() const { return (this->_promotedPawn); }

		void	promotePawn(int	moveTurn) { this->_promotedPawn = moveTurn; }
		void	demotePawn() { this->_promotedPawn = -1; }
	private:
		int		_owner;
		int		_type;
		int		_value;
		bool	_grabbed = false;
		bool	_hasMoved = false;
		int		_promotedPawn = -1;
		int		_hasMovedTurn = -1;
};

#endif