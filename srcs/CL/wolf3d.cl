/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.cl                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/18 23:53:40 by agrumbac          #+#    #+#             */
/*   Updated: 2017/06/27 21:51:41 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "wolf3d.h.cl"
# include "vector.cl"
// map tableau 2x2 ...
// camera dans le.h
// col_hei tableau de deux cases dans laquelle on a la couleur [0] et
// hauteur [1]
// les murs doivent etre de couleur differente en fonction de la direction
// pour chaque x on envoie la couleur

#define NOCOLOR				0x000000
#define WALLMIDNIGHTBLUE	0x191970
#define WALLSKYBLUE			0x51bdff
#define WALLPINK			0xa90e64
#define WALLCRIMSONRED		0xb00718

/*
** 1) compute ray position and direction
** cameraX is x-coordinate in camera space
** map represents which box of the map we're in
** deltaDist is length of ray from one x or y-side to next x or y-side
*/
static inline void		compute_rayposdir()
{
	const double cameraX = 2 * x / double(w) - 1;
	t_vector rayPos = camera.origin;
	t_vector map;
    t_vector deltaDist;

	map.x = (int)rayPos.x;
	map.y = (int)rayPos.y;
	rayDir.x = cam.direction.x + cam_plane.x * cameraX;
	rayDir.y = cam.direction.y + cam_plane.y * cameraX; // 0.66 plane Y
	deltaDist.x = sqrt(1 + (rayDir.y * rayDir.y) / (rayDir.x * rayDir.x));
    deltaDist.y = sqrt(1 + (rayDir.x * rayDir.x) / (rayDir.y * rayDir.y));
}

/*
** 2) compute step and sideDist
** step
** sideDist is length of ray from current position to next x or y-side
*/

static inline void		compute_step_sideDist()
{
	t_vector	step;
	t_vector	sideDist;

	if (rayDir.x < 0)
	{
		step.x = -1;
		sideDist.x = (rayPos.x - map.x) * deltaDist.x;
	}
	else
	{
	   step.x = 1;
	   sideDist.x = (map.x + 1.0 - rayPos.x) * deltaDist.x;
	}
	if (rayDir.y < 0)
	{
		   step.y = -1;
		   sideDist.y = (rayPos.y - map.y) * deltaDist.y;
	}
	else
	{
		step.y = 1;
		sideDist.y = (mapY + 1.0 - rayPos.y) * deltaDist.y;
	}
}

/*
** 3) DDA algo
** hit is a boolean to check if a wall was hit
** side is a boolean to check if a North/South or East/West wall was hit
*/

static inline void		dda()
{
	int		hit = 0;
	int		side;
	float	perpWallDist;

	while (!hit)
	{
        //jump to next map square, OR in x-direction, OR in y-direction
        if (sideDist.x < sideDist.y)
        {
          sideDist.x += deltaDist.x;
          map.x += step.x;
          side = 0;
        }
        else
        {
          sideDist.y += deltaDist.y;
          map.y += step.y;
          side = 1;
        }
        // Check if ray has hit a wall
        if (map[map.x][map.y] > 0)
			hit = 1;
    }
	perpWallDist = (!side) ? (map.x - rayPos.x + (1 - step.x) / 2) / rayDir.x \
 	: (map.y - rayPos.y + (1 - step.y) / 2) / rayDir.y;
}

/*
** modify the color of walls according to the camera direction
*/

static inline uint_t	getcolor()
{
	uint_t color;

	if (cam.direction.y > 0)
		color = (cam.direction.x > 0) ? WALLPINK: WALLCRIMSONRED;
	else
		color = (cam.direction.x > 0) ? WALLSKYBLUE: WALLMIDNIGHTBLUE;
	return (color);
}

bool verLine(int x, int y1, int y2, ColorRGB color)
{
	
}

/*
** lineHeight is the height of the vertical line that should be drawn
** the start and end position of where we should really draw are calculated.
** The center of the wall should be at the center of the screen, and if these
** points lie outside the screen, they're capped to 0 or h-1.
*/

static inline void 		get_wall_height()
{
	int lineHeight = (int)(h / perpWallDist);
	int	drawStart;
	int	drawEnd;

	drawStart = -lineHeight / 2 + h / 2;
	drawEnd = lineHeight / 2 + h / 2;
	if (drawStart < 0)
		drawStart = 0;
		// replace if with: drawStart = (1 - (DrawStart >> 31)) * DrawStart;
	if (drawEnd >= h)
		drawEnd = h - 1;
	wallcolor[x] = getcolor();
	verLine(x, drawStart, drawEnd, color);
	pixels[index] = color;
}

/*
** 1) compute step and sideDist
** 2) compute step and sideDist
** 3) compute dda and calculate distance to perpendicular wall
** 4)
*/

__kernel void		core(__constant char *map, __constant t_cam *cam, \
					__global uint *wall_height, __global uint *wallcolor)
{
	const int x	= get_global_id(0);
	const int w = get_global_size(0);
	const char   (*map)[24][24] = map; // replace later 2 value /!/
	t_vector	cam_plane;

	compute_rayposdir();
	compute_step_sideDist();
	dda();
	//printf("x : [%d], [%d]\n", x, w);//
	get_wall_height()
}
