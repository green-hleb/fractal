/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbar.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: npetrell <npetrell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/27 15:20:09 by npetrell          #+#    #+#             */
/*   Updated: 2020/02/09 17:56:09 by npetrell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/fractol.h"

static void		mandelbar_put(t_fract *struct_fract)
{
	double		z_re;
	double		z_im;
	double		z_im2;

	struct_fract->c_im = struct_fract->y / struct_fract->zoom +
struct_fract->min_im;
	struct_fract->c_re = struct_fract->x / struct_fract->zoom +
struct_fract->min_re;
	z_re = struct_fract->c_re;
	z_im = struct_fract->c_im;
	struct_fract->iter = 0;
	while (struct_fract->iter < struct_fract->max_iter &&
((pow(z_re, 2) + pow(z_im, 2)) <= 4))
	{
		z_im2 = pow(z_im, 2);
		z_im = -2 * z_re * z_im + struct_fract->c_im;
		z_re = pow(z_re, 2) - z_im2 + struct_fract->c_re;
		struct_fract->iter++;
	}
	put_pxl(struct_fract, struct_fract->x,
struct_fract->y);
}

void			*mandelbar_func(void *data)
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
			mandelbar_put(struct_fract);
			struct_fract->x++;
		}
		struct_fract->y++;
	}
	return (NULL);
}
