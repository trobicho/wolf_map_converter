/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 14:59:40 by trobicho          #+#    #+#             */
/*   Updated: 2019/09/02 23:04:27 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "map.h"
#include "unistd.h"

static int	the_fd;
static int	first_0 = 1;

char	min_dist_to_avg(double r, double g, double b);
double	dist_to_avg(int index, double r, double g, double b);

int		detect_mode = 0;
double	detect_threshold = 0.6;
double	detect_threshold_elem = 0.2;
int		nb_avg = 5;
t_avg	avg[5];

void		init_fd(int fd)
{
	the_fd = fd;
}

void		set_detect_mode(int m)
{
	detect_mode = m;
}

void		init_avg()
{
	avg[0] = (t_avg){0.12, 0.25, 0.37, 23};
	avg[1] = (t_avg){0.49, 0.49, 0.49, 1};
	avg[2] = (t_avg){0.25, 0.03, 0.04, 17};
	avg[3] = (t_avg){0.028186, 0.028676, 0.578676, 33};
	avg[4] = (t_avg){0.277941, 0.515686, 0.524020, 47};
}

void		detect_new_tex(int index, double d, double r, double g, double b)
{
	if (fabs(avg[index].r  - r) > detect_threshold_elem
		|| fabs(avg[index].g - g) > detect_threshold_elem
		|| fabs(avg[index].b - b) > detect_threshold_elem)
	{
		printf("perhaps this is a new tex: avg[%d] = (t_avg){%lf, %lf, %lf}\n", nb_avg, r, g, b);
	}
	else if (d > detect_threshold)
	{
		printf("perhaps this is a new tex: avg[%d] = (t_avg){%lf, %lf, %lf}\n", nb_avg, r, g, b);
	}
}

void		map_print(SDL_Surface *map, t_map_conf map_conf)
{
	int			x, y;
	uint32_t	pix;

	for (y = 0; y < map->h / map_conf.h_tile; y++)
	{
		for (x = 0; x < map->w / map_conf.w_tile; x++)
		{
			get_tile_index(map, map_conf, x, y);
		}
		if (!detect_mode)
		printf("\n");
	}
}

uint32_t	get_pixel(SDL_Surface *surface, int x, int y)
{
	if (x >= surface->w || y >= surface->h)
		return (0);
	return (((uint32_t*)surface->pixels)[y * surface->w + x]);
}

int		get_tile_index(SDL_Surface *map, t_map_conf map_conf, int x, int y)
{
	double		r_avg = 0.0, g_avg = 0.0, b_avg = 0.0;
	double		r_threshold = 0.2, g_threshold = 0.2, b_threshold = 0.2;
	int			pad = 2;
	int			ratio = (map_conf.w_tile - pad * 2) * (map_conf.h_tile - pad * 2);
	uint32_t	pix;
	char		c;

	for (int yA = pad; yA < map_conf.h_tile - pad; yA++)
	{
		for (int xA = pad; xA < map_conf.w_tile - pad; xA++)
		{
			pix = get_pixel(map, x * map_conf.w_tile + xA
					, y * map_conf.h_tile + yA);
			r_avg += (((unsigned char*)&pix)[0] / 255.0) / ratio;
			g_avg += (((unsigned char*)&pix)[1] / 255.0) / ratio;
			b_avg += (((unsigned char*)&pix)[2] / 255.0) / ratio;
		}
	}
	//printf("(%d, %d){%lf, %lf, %lf}\n", x, y, r_avg, g_avg, b_avg);
	if (!detect_mode)
	{
		if (r_avg > r_threshold || g_avg > g_threshold || b_avg > b_threshold)
		{
			c = min_dist_to_avg(r_avg, g_avg, b_avg);
			printf("%c", min_dist_to_avg(r_avg, g_avg, b_avg) + 'a');
			write(the_fd, &c, 1);
		}
		else
		{
			if (first_0)
			{
				c = 111;
				write(the_fd, &c, 1);
				first_0 = 0;
			}
			else
				write(the_fd, "\0", 1);
			printf(".");
		}
	}
	else if (r_avg > r_threshold || g_avg > g_threshold || b_avg > b_threshold)
		min_dist_to_avg(r_avg, g_avg, b_avg);
	return (0);
}

char	min_dist_to_avg(double r, double g, double b)
{
	double	d, min_d = dist_to_avg(0, r, g, b);
	int		min_index = 0;

	for (int i = 1; i < nb_avg; i++)
	{
		d = dist_to_avg(i, r, g, b);
		if (d < min_d)
		{
			min_d = d;
			min_index = i;
		}
	}
	if (detect_mode)
		detect_new_tex(min_index, min_d, r, g, b);
	return (avg[min_index].c);
}

double	dist_to_avg(int index, double r, double g, double b)
{
	double	d_r, d_g, d_b;

	d_r = fabs(avg[index].r - r);
	d_g = fabs(avg[index].g - g);
	d_b = fabs(avg[index].b - b);
	return (d_r + d_g + d_b);
}
