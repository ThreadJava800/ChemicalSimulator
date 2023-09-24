CC=g++

CFLAGS  = -D _DEBUG -ggdb3 -std=c++17 -O3 -w -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations \
-Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body \
-Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor \
-Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion \
-Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods \
-Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused \
-Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast \
-Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging \
-fno-omit-frame-pointer -pie -fPIE -Werror=vla \
-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr


SOURCES=src/objects/objects.cpp src/buttons/button.cpp src/plots/plot.cpp src/manager/manager.cpp src/main.cpp

EXECUTABLE=sim

SFML_EXEC= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
 
all: compile run

erase:
	clear

compile:
	@${CC} ${CFLAGS} $(SOURCES) -o $(EXECUTABLE) $(SFML_EXEC)

run:
	@./${EXECUTABLE}

clean:
	@rm -rf ${EXECUTABLE}