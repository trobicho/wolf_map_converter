/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trobicho <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/10 14:59:43 by trobicho          #+#    #+#             */
/*   Updated: 2019/09/02 23:23:34 by trobicho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <SDL.h>
#include <stdio.h>
#include "map.h"
#include <unistd.h>
#include <sys/types.h>
#include "libft.h"
#include <fcntl.h>

int		choose_size(int s, int n_avg)
{
	double	d, d_min = (double)s / (n_avg - 1);
	d_min -= floor(d_min);
	int		n_best = n_avg - 2;

	printf("\n");
	d = d_min;
	for (int i = 1; i < 2; i++)
	{
		printf("%lf\n", d);
		d = (double)s / (n_avg - 1 + i);
		d -= floor(d);
		if (d_min > d)
		{
			d_min = d;
			n_best = n_avg - 1 + i;
		}
	}
	printf("%lf\n", d);
	return (n_best);
}

SDL_Surface *get_tex_opt(int ac, char **av)
{
	int	n_opt = 0;

	for (int i = 1; i < ac; i++)
	{
		if (av[i][0] == '-')
		{
			switch (av[i][1])
			{
				case 'g':
					set_detect_mode(1);
					break ;
			}
			n_opt++;
		}
		else
			break ;
	}
	printf("%s\n", av[1+n_opt]);
	return (SDL_LoadBMP(av[1 + n_opt]));
}

int		main(int ac, char **av)
{
	t_map_conf	map_conf;
	int			fd;

	if (ac < 2)
	{
		printf("usage: ./map_convert file\n");
		return (1);
	}
	SDL_Surface *tex_bad_format = get_tex_opt(ac, av);
	SDL_Surface *tex = SDL_ConvertSurfaceFormat(tex_bad_format, SDL_PIXELFORMAT_ARGB8888, 0);
	SDL_FreeSurface(tex_bad_format);
	if (tex == NULL)
	{
		printf("invalid file\n");
		return (1);
	}
	if ((fd = open("./map_out.ppm", O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR)) < 0)
		return (1);
	init_fd(fd);
	map_conf.w_tile = tex->w / 64;
	map_conf.h_tile = tex->h / 64;
	map_conf.w_tile = choose_size(tex->w, 8);
	map_conf.h_tile = choose_size(tex->h, 8);
	if (tex->w / map_conf.w_tile > 64)
		map_conf.w_tile = 8;
	if (tex->h / map_conf.h_tile > 64)
		map_conf.h_tile = 8;
	write(fd, "P6\n", 3);
	ft_putnbr_fd(tex->w / map_conf.w_tile, fd);
	write(fd, "\n", 1);
	ft_putnbr_fd(tex->h / map_conf.h_tile, fd);
	write(fd, "\n", 1);
	write(fd, "255\n", 4);
	printf("{%d, %d} %d, %d, %d, %d\n", map_conf.w_tile, map_conf.h_tile, tex->w / map_conf.w_tile, tex->h / map_conf.h_tile, tex->w, tex->h);
	init_avg();
	map_print(tex, map_conf);
	SDL_FreeSurface(tex);
	return (0);
}
