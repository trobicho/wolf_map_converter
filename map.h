/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 14:59:37 by trobicho          #+#    #+#             */
/*   Updated: 2019/09/02 21:44:36 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAP_H
# define MAP_H

# include <SDL.h>

typedef struct	s_map_conf
{
	int		w_tile;
	int		h_tile;
}				t_map_conf;

typedef struct	s_avg
{
	double	r;
	double	g;
	double	b;
	char	c;
}				t_avg;

void		init_fd(int fd);
void		init_avg();
void		set_detect_mode(int m);
void		map_print(SDL_Surface *map, t_map_conf map_conf);
uint32_t	get_pixel(SDL_Surface *surface, int x, int y);
int			get_tile_index(SDL_Surface *map, t_map_conf map_conf, int x, int y);
#endif
