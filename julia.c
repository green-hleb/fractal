/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   julia.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: baylak <baylak@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/14 17:27:06 by npetrell          #+#    #+#             */
/*   Updated: 2020/01/30 00:41:23 by baylak           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

int		get_color(t_color clr)
{
	return ((clr.r << 16) | (clr.g << 8) | clr.b);
}

void			put_pxl(t_fract *data, int x, int y)
{
	t_color		clr;

	if (data->x < SIZE && data->y < SIZE)
	{
		clr.r = sin(0.25 * data->iter + 0) * 127 + 128;
   		clr.g = sin(0.25 * data->iter + 2) * 127 + 128;
   		clr.b = sin(0.25 * data->iter + 4) * 127 + 128;
		*(int*)(data->img_ptr + (x + y * SIZE) * data->bpp / 8) = get_color(clr);
		if (data->iter == data->max_iter)
			*(int*)(data->img_ptr + (x + y * SIZE) * data->bpp / 8) = 0x000000;
	}
}

static void		draw_julia(t_fract *struct_fract)
{
	double		z_re;
	double		z_im;
	double		z_im2;

	z_re = struct_fract->x / struct_fract->zoom + struct_fract->min_re;
	z_im = struct_fract->y / struct_fract->zoom + struct_fract->min_im;
	struct_fract->iter = 0;
	while (struct_fract->iter < struct_fract->max_iter &&
((pow(z_re, 2) + pow(z_im, 2)) < 4))
	{
		z_im2 = pow(z_im, 2);
		z_im = 2 * z_re * z_im + struct_fract->k_im;
		z_re = pow(z_re, 2) - z_im2 + struct_fract->k_re;
		struct_fract->iter++;
	}
	put_pxl(struct_fract, struct_fract->x,
	struct_fract->y);
}

void			*julia_func(void *data)
{
	t_fract		*struct_fract;
	double		tmp;

	struct_fract = (t_fract*)data;
	struct_fract->y = 0;
	tmp = struct_fract->x;
	while (struct_fract->y < SIZE)
	{
		struct_fract->x = tmp;
		while (struct_fract->x < struct_fract->image_width)
		{
			draw_julia(struct_fract);
			struct_fract->x++;
		}
		struct_fract->y++;
	}
	return (NULL);
}

void			julia_pthread(t_fract *struct_fract)
{
	t_fract		tmp[160];
	pthread_t	thread[160];
	int			i;

	i = 0;
	while (i < 160)
	{
		ft_memcpy((void*)&tmp[i], (void*)struct_fract, sizeof(t_fract));
		tmp[i].x = 5 * i;
		tmp[i].image_width = 5 * (i + 1);
		pthread_create(&thread[i], NULL, julia_func, &tmp[i]);
		i++;
	}
	while (i--)
		pthread_join(thread[i], NULL);
	mlx_put_image_to_window(struct_fract->mlx_ptr, struct_fract->win_ptr,
struct_fract->img, 0, 0);
}
