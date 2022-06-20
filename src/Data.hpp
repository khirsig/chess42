/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Data.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khirsig <khirsig@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/04 19:27:45 by khirsig           #+#    #+#             */
/*   Updated: 2022/06/20 12:48:31 by khirsig          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DATA_HPP
# define DATA_HPP

# include "../raylib-cpp/include/raylib-cpp.hpp"
# include <vector>
# include "./BoardSquare.hpp"

# define SCREEN_WIDTH 800
# define SCREEN_HEIGHT 800

struct Data {
	BoardSquare							square[8][8];
	raylib::Color						primaryColor = RAYWHITE;
	raylib::Color						secondaryColor = BLUE;
	std::vector<raylib::Texture2D>		tex;
};




#endif