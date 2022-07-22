/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BoardSquare.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 19:32:27 by khirsig           #+#    #+#             */
/*   Updated: 2022/07/22 09:07:26 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BOARDSQUARE_HPP
# define BOARDSQUARE_HPP

# include "./ChessPiece.hpp"

struct BoardSquare {
	ChessPiece	*piece;
	bool		debug = false;
};


#endif