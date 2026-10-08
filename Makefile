SOURCES := $(shell bash -c "find src -type f -iname '*.cpp'") \
           $(shell bash -c "find src -type f -iname '*.c'")

OBJECTS := $(patsubst src/%.cpp,obj/%.o,$(filter %.cpp,$(SOURCES))) \
           $(patsubst src/%.c,obj/%.o,$(filter %.c,$(SOURCES)))

TARGET_FOLDER := ./bin/
TARGET := $(TARGET_FOLDER)app.exe

INCLUDES := -I./include/
LIBS := -L./libs/ -lglfw3 -lgdi32

$(TARGET): $(OBJECTS)
	@mkdir -p $(TARGET_FOLDER)
	g++ -Wall $(INCLUDES) $^ -o $@ $(LIBS)
	$(TARGET)

obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	g++ -Wall $(INCLUDES) -c $< -o $@

obj/%.o: src/%.c
	@mkdir -p $(dir $@)
	g++ -Wall $(INCLUDES) -c $< -o $@

run:
	$(TARGET)

clean:
	rm -rf $(TARGET) ./obj