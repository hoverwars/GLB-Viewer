compile: ./src/main.cpp
	mkdir -p ./bin/
	rm -rf ./bin/*.*
	g++ -I./include/ -Wall -o ./bin/app.exe ./src/glad.c ./src/main.cpp -L./libs/ -lglfw3 -lgdi32
	./bin/app.exe

clean:
	rm -rf ./bin/