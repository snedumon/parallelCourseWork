#Created by Alexandr Syrotiuk aka "Kridenberg" and Eugen Belyaev aka "Snedumon"

FLAGS	=	-pthread -lpthread -fopenmp
SOURCE	=	./source/*.cpp
NAME	=	main

all:
	@g++ -g -o $(NAME) $(SOURCE) $(FLAGS)

clear:
	@rm $(NAME)
