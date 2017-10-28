
#ifndef WOLF3D_H_CL
# define WOLF3D_H_CL

/*
** ********************************** typedefs *********************************
*/

typedef float2			t_vector;

typedef struct			s_xy
{
	int					x;
	int					y;
}						t_xy;

/*
** ********************************** camera ***********************************
*/

typedef struct			s_cam
{
	t_vector			origin;
	t_vector			direction;
	t_vector			plane;
	uint				screen_height;
}						t_cam;

/*
** ********************************** global ***********************************
*/

# define GLOBAL_X			1
# define GLOBAL_Y			0

# define MAP_SIZE			24

# define WALLMIDNIGHTBLUE	0xff191970
# define WALLSKYBLUE		0xff51bdff
# define WALLPINK			0xffa90e64
# define WALLCRIMSONRED		0xffb00718


#endif
