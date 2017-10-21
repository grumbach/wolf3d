# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/05/25 11:36:26 by agrumbac          #+#    #+#              #
#    Updated: 2017/10/21 13:26:40 by agrumbac         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

############################## VAR #############################################

NAME = wolf3d

SRC = wolf3d.c cl.c sdl.c sdl_events.c errors.c

SRCDIR = srcs/C/

OBJDIR = objs

CFLAGS = -Wall -Wextra -Werror -g

DEP = libft/includes/libft.h includes/wolf3d.h \
	includes/easy_cl.h includes/easy_sdl.h

CC = clang #/Users/agrumbac/.brew/bin/gcc-7

OBJ = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))

LIB = -Llibft -lft

INCL_SDL = -I Frameworks/SDL2.framework/Headers/ \
	-I Frameworks/SDL2_ttf.framework/Headers/ \
	-I Frameworks/SDL2_image.framework/Headers/

INCL = -Ilibft/includes -Iincludes $(INCL_SDL)

FRAMEWORKS = -framework opencl -rpath @loader_path/frameworks/ \
	-framework SDL2 -framework SDL2_ttf -framework SDL2_image -F ./Frameworks

############################## COLORS ##########################################

BY = "\033[33;1m"
BR = "\033[31;1m"
BG = "\033[32;1m"
BB = "\033[34;1m"
BM = "\033[35;1m"
BC = "\033[36;1m"
BW = "\033[37;1m"
Y = "\033[33m"
R = "\033[31m"
G = "\033[32m"
B = "\033[34m"
KW = "\033[37;5m"
KR = "\033[31;5m"
KG = "\033[32;5m"
KB = "\033[34;5m"
X = "\033[0m"
UP = "\033[A"
CUT = "\033[K"

############################## RULES ###########################################

all: libft ${NAME}
	@echo ${G}Success"   "[${NAME}]${X}

libft: art
	@make -C libft/

${NAME}: ${OBJ}
	@echo ${B}Compiling [${NAME}]...${X}
	@${CC} ${CFLAGS} ${LIB} ${INCL} ${FRAMEWORKS} -o $@ ${OBJ}

${OBJDIR}/%.o: ${SRCDIR}%.c ${DEP}
	@echo ${Y}Compiling [$@]...${X}
	@/bin/mkdir -p ${OBJDIR}
	@${CC} ${CFLAGS} ${INCL} -c -o $@ $<
	@printf ${UP}${CUT}

clean:
	@echo ${R}Cleaning"  "[libft objs]...${X}
	@make -C libft/ clean
	@echo ${R}Cleaning"  "[objs]...${X}
	@/bin/rm -Rf ${OBJDIR}

fclean: clean
	@make -C libft/ fclean
	@echo ${R}Cleaning"  "[${NAME}]...${X}
	@/bin/rm -f ${NAME}

test:
	@${CC} ${INCL} ${FRAMEWORKS} -g -fsanitize=address ${LIB} -o ${NAME} \
	$(addprefix ${SRCDIR}, ${SRC})

debug: test
	lldb ./${NAME}

build: ${OBJ}

run: ${NAME}
	./${NAME}

re: fclean all

############################## ART #############################################

art:
	@echo ${BB}
	@echo "                                __"
	@echo "                              .dSSb"
	@echo "                            .' TOS;\\"
	@echo "                           /  : TP._;"
	@echo "                          / _.;  :Tb|         "${BR}"W"${BY}"O"${BG}"L"${BM}"F"${BB}" 3D "${KR}"!"${X}${BB}
	@echo "                         /   /   ;"${BW}"j"${BB}"S"${BW}"j"${BB}"        /"
	@echo "                     _.-\"       dSSSS"
	@echo "                   .' ..       "${BW}"d"${BB}"SSSS"${BW}";"${BB}
	@echo "                  /  /"${KR}"P"${X}${BB}"'      dSSSSP. "${BW}"|\\" ${BB}
	@echo "                 /   \"      ."${BW}"d"${BB}"SSSP' "${BW}"|\\" ${BB}"^\"l"
	@echo "               .'           \`TSP"${BW}"^"${BB}"\"\"\"\"\"  :"
	@echo "           ._.'      _.'                ;"
	@echo "        \`-.-\".-'-' ._.       _.-\"    .-\""
	@echo "      \`.-\" _____  ._              .-\""
	@echo "     -(.gSSSSSSSb.              .'"
	@echo "       \"\"^^TSSSP^)            .(:"
	@echo "         _/  -\"  /.'         /:/;"
	@echo "      ._.'-'\`-'  \")/         /;/;"
	@echo "   \`-.-\"..--\"\"   \" /         /  ;"
	@echo "  .-\" ..--\"\"        -'          :"
	@echo "  ..--\"\"--.-\"         (\\      .-(\\"
	@echo "    ..--\"\"              \`-\\(\\/;\`"
	@echo "      _.                      :"
	@echo "                              ;\`-"
	@echo "                             :\\"
	@echo "                             ;"
	@echo ${X}

.PHONY: all clean fclean re test build debug run art
