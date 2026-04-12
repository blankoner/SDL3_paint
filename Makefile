prog:
	clang++ src/main.cpp src/interface.cpp src/program.cpp -lSDL3 -L/opt/homebrew/opt/sdl3/lib -o prog
clean:
	rm prog
