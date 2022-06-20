/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChessPiece.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 19:33:09 by khirsig           #+#    #+#             */
/*   Updated: 2022/06/20 13:20:38 by khirsig          ###   ########.fr       */
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
		ChessPiece(int owner, int type) : _owner(owner), _type(type) { }
		~ChessPiece() { }

		void	setGrabbed(bool isGrabbed) { this->_grabbed = isGrabbed; }
		int		getOwner() const { return (this->_owner); }
		int		getType() const { return (this->_type); }
		bool	getGrabbed() const { return (this->_grabbed); }
	private:
		int		_owner;
		int		_type;
		bool	_grabbed = false;
};

#endif