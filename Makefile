# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jledesma <jledesma@student.42malaga.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/03 16:59:11 by jledesma          #+#    #+#              #
#    Updated: 2023/05/03 18:04:05 by jledesma         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

INC = /Users/jledesma/.brew/opt/openssl@3.1/include
LIB = /Users/jledesma/.brew/opt/openssl@3.1/lib
CFLAGS = -Wall -Wextra -Werror -Wno-deprecated-declarations


all: corsair

corsair: corsair.o
	@clear
	gcc corsair.o -L$(LIB) -lssl -lcrypto -o corsair

corsair.o: corsair.c
	gcc -c corsair.c $(CFLAGS) -I$(INC) -o corsair.o

fclean:
	rm -rf corsair corsair.o resources/cert1.pem resources/cert2.pem resources/passwd.enc resources/encrypted_file.txt

generate:
	cd recursos && python3 generate.py