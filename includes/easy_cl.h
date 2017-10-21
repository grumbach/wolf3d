/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   easy_cl.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/28 11:54:05 by agrumbac          #+#    #+#             */
/*   Updated: 2017/10/21 17:27:24 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EASY_CL_H
# define EASY_CL_H

/*
** ********************************** OpenCL ***********************************
** MAX_SOURCE_SIZE	max size in bytes of [.cl file]
** MAX_ERR_LOG		max size in bytes of error log
** CL_FILENAME		relative path to [.cl file] containing __kernel func
** CL_CC_FLAGS		[.cl file] compilation flags (can be NULL)
** CL_DEVICE		device used
**		CL_DEVICE_TYPE_GPU
** 		CL_DEVICE_TYPE_CPU
** 		CL_DEVICE_TYPE_ALL
** MAX_KERNEL_ARGS	max numbers of args passed to __kernel func
** WORK_DIM			number of dimentions (1, 2 or 3),
** 					affects cl->work_size[WORK_DIM] : must be (size_t[WORK_DIM])
** 					filled with size values for each dimentions
** *****************************************************************************
*/

# include <OpenCL/opencl.h>

# define MAX_SOURCE_SIZE	(0x100000)
# define MAX_ERR_LOG		(10000)
# define CL_FILENAME		"srcs/CL/wolf3d.cl"
# define CL_KERNEL_NAME		"core"
# define CL_CC_FLAGS		"-I. -Isrcs/CL -cl-mad-enable -cl-fast-relaxed-math"
# define CL_DEVICE			CL_DEVICE_TYPE_ALL
# define MAX_KERNEL_ARGS	5
# define WORK_DIM			1

typedef struct			s_arg
{
	void				*ptr;
	size_t				size;
	cl_mem_flags		flag;
}						t_arg;

typedef struct			s_cl
{
	cl_context			context;
	cl_command_queue	command_queue;
	cl_program			program;
	cl_kernel			kernel;
	cl_device_id		device_id;
	cl_mem				variables[MAX_KERNEL_ARGS + 1];
	int					nb_const;
}						t_cl;

/*
** ********************************** functions ********************************
** void		cl_init(t_cl *cl, const int nb_const, ...);
** 					initializes cl program, sets constant variables (optional)
** 					args of type (t_arg){void*, size_t, cl_mem_flags}
** 					for example : cl_init(cl, 1, \
** 						(t_arg){ptr_const, 1024, 4});//will always be read-only
** void		cl_run(t_cl *cl, size_t work_size[WORK_DIM], const int nb_arg, ...);
** 					runs the __kernel function with the given nb_arg arguments
** 						and the const arguments from cl_init (if present)
** 					work_size contains sizes for each WORK_DIM :
** 						for example WORK_DIM 2 with image width and height
** 						one thread (__kernel func) for each pixel
**					args of type (t_arg){void*, size_t, cl_mem_flags}
** 					use example : cl_run(cl, (size_t[2]){42, 42}, 1, \
** 						(t_arg){ptr, 1024, CL_MEM_READ_WRITE});
**					for cl_mem_flags choose from :
** 						CL_MEM_READ_WRITE			1
** 						CL_MEM_WRITE_ONLY			2
** 						CL_MEM_READ_ONLY			4
** void		cl_end(t_cl *cl);
** 					terminate and free all cl environment
** *****************************************************************************
*/

void					cl_init(t_cl *cl, const int nb_const, ...);
void					cl_run(t_cl *cl, size_t work_size[WORK_DIM], const int nb_arg, ...);
void					cl_end(t_cl *cl);

/*
** ********************************** errors ***********************************
*/

int						cl_error_log(t_cl *cl, const cl_int error_code);

#endif
