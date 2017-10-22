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

#define NOCOLOR				0xff000000
#define WALLMIDNIGHTBLUE	0xff191970
#define WALLSKYBLUE			0xff51bdff
#define WALLPINK			0xffa90e64
#define WALLCRIMSONRED		0xffb00718
#define SCREEN_HEIGHT		640

/*
** 1) compute ray position and direction
** cameraX is x-coordinate in camera space
** map represents which box of the map we're in
** deltaDist is length of ray from one x or y-side to next x or y-side
*/

/*
** 2) compute step and sideDist
** step
** sideDist is length of ray from current position to next x or y-side
*/



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

/*
** 1) compute step and sideDist
** 2) compute step and sideDist
** 3) compute dda and calculate distance to perpendicular wall
** 4)
*/


static t_vector get_step_sideDist(t_vector map_pos, t_vector *step, \
	t_vector rayPos, t_vector rayDir, t_vector deltaDist)
{
	t_vector		sideDist;

	if (rayDir.x < 0)
	{
		step->x = -1;
		sideDist.x = (rayPos.x - map_pos.x) * deltaDist.x;
	}
	else
	{
		step->x = 1;
		sideDist.x = (map_pos.x + 1.0 - rayPos.x) * deltaDist.x;
	}
	if (rayDir.y < 0)
	{
		step->y = -1;
		sideDist.y = (rayPos.y - map_pos.y) * deltaDist.y;
	}
	else
	{
		step->y = 1;
		sideDist.y = (map_pos.y + 1.0 - rayPos.y) * deltaDist.y;
	}
	return (sideDist);
}


static t_vector getDeltaDist(t_vector rayDir)
{
	float tmp;

	tmp = (rayDir.y * rayDir.y) / (rayDir.x * rayDir.x);
	rayDir.x = sqrt(1 + tmp);
	rayDir.y = sqrt(1 + 1.0f / tmp);
	return (rayDir);
}

__kernel void		core(__constant char *map, __constant t_cam *cam, \
					__global uint *wall_height, __global uint *wall_color)
{
	const int 		x = get_global_id(0);
	const int 		w = get_global_size(0);

	//compute_rayposdir(cam, x, w, rayDir, deltaDist);
	const double	cameraX = 2 * x / double(w) - 1;


//	printf("cam->direction.x : %f  .y: %f\n cam->origin.x : %f  .y : %f\n", cam->direction.x, cam->direction.y, cam->origin.x, cam->origin.y);//


//	printf("cameraX : [%d] : \n", cameraX);//
	t_vector		plane;
	t_vector		rayDir;
	t_vector		deltaDist;
	t_vector		sideDist;
	t_vector		step;
	t_vector		map_pos;

	plane.x = 0;
	plane.y = 0.66;
	rayDir.x = cam->direction.x + plane.x * cameraX;
	rayDir.y = cam->direction.y + plane.y * cameraX; // 0.66 plane Y
//	printf("rayDir.x : %f  rayDir.y : %f\n", rayDir..x, rayDir.y);//
	deltaDist = getDeltaDist(rayDir);
	const t_vector	rayPos = cam->origin;
	map_pos.x = (int)rayPos.x;
	map_pos.y = (int)rayPos.y;
	sideDist = get_step_sideDist(map_pos, &step, rayPos, rayDir, deltaDist);

//printf("sideDist.x : [%d]      sideDist.y : [%d]\n", sideDist.x, sideDist.y);//


//	printf("step.x : [%d]\n", step.x);//
	float			perpWallDist;
	perpWallDist = dda(step, sideDist, map_pos, deltaDist, map, rayDir, \
		rayPos);

	//printf("PerpWallDist : [%d]\n", x, perpWallDist);//
	//printf("x : [%d], [%d]\n", x, w);//
	//printf("\n\n\n\n\n\n");//

	//send result
	wall_height[x] = (uint)(SCREEN_HEIGHT / perpWallDist);
//	printf("height : [%d]\n", wall_height[x]);//
	wall_color[x] = getcolor(cam->direction);
}
