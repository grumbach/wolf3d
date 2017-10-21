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
#define SCREEN_HEIGHT		640

/*
** 1) compute ray position and direction
** cameraX is x-coordinate in camera space
** map represents which box of the map we're in
** deltaDist is length of ray from one x or y-side to next x or y-side
*/
static inline void		compute_rayposdir(__constant t_cam *cam, const int x, \
	const int w, t_vector rayDir, t_vector deltaDist)
{
	const double	cameraX = 2 * x / double(w) - 1;

	rayDir.x = cam->direction.x + cam->plane.x * cameraX;
	rayDir.y = cam->direction.y + cam->plane.y * cameraX; // 0.66 plane Y
	deltaDist.x = sqrt(1 + (rayDir.y * rayDir.y) / (rayDir.x * rayDir.x));
    deltaDist.y = sqrt(1 + (rayDir.x * rayDir.x) / (rayDir.y * rayDir.y));
}

/*
** 2) compute step and sideDist
** step
** sideDist is length of ray from current position to next x or y-side
*/

static inline void		compute_step_sideDist(t_vector rayDir, t_vector rayPos \
	, t_vector deltaDist, t_vector step, t_vector sideDist, t_vector map)
{
	map.x = (int)rayPos.x;
	map.y = (int)rayPos.y;
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
		sideDist.y = (map.y + 1.0 - rayPos.y) * deltaDist.y;
	}

}

/*
** 3) DDA algo
** hit is a boolean to check if a wall was hit
** side is a boolean to check if a North/South or East/West wall was hit
*/

static inline float		dda(t_vector step, t_vector sideDist, t_vector map_pos,
	t_vector deltaDist, __constant char *map, t_vector rayDir, t_vector rayPos)
{
	int		hit = 0;
	int		side;
	char (*maps)[24][24]; // REMPLACER 24

	maps = (void *)map;
	while (!hit)
	{
        //jump to next map square, OR in x-direction, OR in y-direction
        if (sideDist.x < sideDist.y)
        {
          sideDist.x += deltaDist.x;
          map_pos.x += step.x;
          side = 0;
        }
        else
        {
          sideDist.y += deltaDist.y;
          map_pos.y += step.y;
          side = 1;
        }
        // Check if ray has hit a wall
        if ((*maps)[(int)map_pos.x][(int)map_pos.y] > 0)
			hit = 1;
    }
	if (!side)
		return ((map_pos.x - rayPos.x + (1 - step.x) / 2) / rayDir.x) ;
	return ((map_pos.y - rayPos.y + (1 - step.y) / 2) / rayDir.y);
}

/*
** modify the color of walls according to the camera direction
*/

static inline uint getcolor(t_vector direction)
{
	if (direction.y > 0)
		return ((direction.x > 0) ? WALLPINK: WALLCRIMSONRED);
	return ((direction.x > 0) ? WALLSKYBLUE: WALLMIDNIGHTBLUE);

}

static inline void verLine(int x, int y1, int y2, uint color)
{

}

/*
** lineHeight is the height of the vertical line that should be drawn
** the start and end position of where we should really draw are calculated.
** The center of the wall should be at the center of the screen, and if these
** points lie outside the screen, they're capped to 0 or h-1.
*/

static inline void 		get_wall_height(const int lineHeight, \
	float perpWallDist, t_vector direction, const int x)
{
	int			drawStart;
	int			drawEnd;
	uint		color;

	drawStart = -lineHeight / 2 + SCREEN_HEIGHT / 2;
	drawEnd = lineHeight / 2 + SCREEN_HEIGHT / 2;
	if (drawStart < 0)
		drawStart = 0;
		// replace if with: drawStart = (1 - (DrawStart >> 31)) * DrawStart;
	if (drawEnd >= SCREEN_HEIGHT)
		drawEnd = SCREEN_HEIGHT - 1;
	color = getcolor(direction);
//	wallcolor[x] = getcolor();
	verLine(x, drawStart, drawEnd, color);
//	pixels[index] = color;
}

/*
** 1) compute step and sideDist
** 2) compute step and sideDist
** 3) compute dda and calculate distance to perpendicular wall
** 4)
*/
//
__kernel void		core(__constant char *map, __constant t_cam *cam, \
					__global uint *wall_height, __global uint *wall_color)
{
	const int 		x = get_global_id(0);
	const int 		w = get_global_size(0);
	t_vector		rayDir;
	const t_vector	rayPos = cam->origin;
    t_vector		deltaDist;
	t_vector		step;
	t_vector		sideDist;
	t_vector		map_pos;
	float			perpWallDist;

	compute_rayposdir(cam, x, w, rayDir, deltaDist);
	compute_step_sideDist(rayDir, rayPos, deltaDist, step, sideDist, map_pos);
	printf("step.x : [%d]\n", step.x);//
	perpWallDist = dda(step, sideDist, map_pos, deltaDist, map, rayDir, rayPos);
	//printf("x : [%d], [%d]\n", x, w);//
	get_wall_height((int)(SCREEN_HEIGHT / perpWallDist), perpWallDist, cam->direction, x);
}
