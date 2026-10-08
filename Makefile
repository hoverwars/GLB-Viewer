SOURCES := $(shell bash -c "find src -type f -iname '*.cpp'") \
           $(shell bash -c "find src -type f -iname '*.c'")

OBJECTS:=$(foreach x, $(basename $(SOURCES)), $(x).o)

TARGET_FOLDER:=./bin/
TARGET:=$(TARGET_FOLDER)app.exe

INCLUDES:=-I./include/
LIBS:=-L./libs/ -lglfw3 -lgdi32

$(TARGET): $(OBJECTS)
	mkdir -p $(TARGET_FOLDER)
	rm -rf $(TARGET_FOLDER)*.*
	g++ -Wall $(INCLUDES) $^ -o $@ $(LIBS)
	./$(TARGET)

%.o: %.cpp
	g++ -Wall $(INCLUDES) -c $< -o $@

%.o: %.c
	g++ -Wall $(INCLUDES) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)