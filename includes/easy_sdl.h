/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   easy_sdl.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/28 11:55:45 by agrumbac          #+#    #+#             */
/*   Updated: 2017/12/10 17:50:47 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EASY_SDL_H
# define EASY_SDL_H

/*
** ********************************** SDL **************************************
** BPP				BytesPerPixel : is 32 for (uint32_t*)sdl->pixels
**
** ********************************** functions ********************************
** void		sdl_init(t_sdl *sdl, const char *window_name);
** 					initialize sdl window
** void		sdl_run(t_sdl *sdl);
** 					draw sdl->pixels to sdl->screen of sdl->window
** void		cl_end(t_cl *cl);
** 					terminate and free all sdl environment
**
** void		sdl_init_window(t_sdl *sdl);
** 					initializes window, use in case of resize event
** *****************************************************************************
*/

# include <SDL.h>
# include <SDL_image.h>
# include <SDL_ttf.h>
# include <stdint.h>

# define BPP				32
# define NB_TEXTURES		4

typedef struct			s_xy
{
	int					x;
	int					y;
}						t_xy;

typedef struct			s_minimap
{
	int					display;
	int					radius;
}						t_minimap;

typedef struct			s_sdl
{
	SDL_Window			*window;
	SDL_Surface			*screen;
	SDL_Surface			*draw_surface;
	SDL_Surface			*texture[NB_TEXTURES + 1];
	uint32_t			*pixels;
	t_xy				size;
	t_minimap			minimap;
}						t_sdl;

void					sdl_init(t_sdl *sdl, const char *window_name);
void					sdl_end(t_sdl *sdl);

int						sdl_init_window(t_sdl *sdl);

#endif
