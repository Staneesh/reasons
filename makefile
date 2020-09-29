TARGET_EXEC ?= main

SRC_DIR ?= ./src
BUILD_DIR ?= ./build
CXX = clang
CC = gcc

CPP_FLAGS = -MMD -MP -Wall -Wshadow -Wextra
DEBUG_FLAGS = $(CPP_FLAGS) -O0 -g
RELEASE_FLAGS := $(CPP_FLAGS) -O3
LINK_FLAGS := -ldl -lstdc++ -lglfw -lm
INCLUDE_FLAGS := -I./include  -I./src

SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c')
DEBUG_OBJS := $(SRCS:%=$(BUILD_DIR)/debug/%.o)
DEBUG_DEPS := $(DEBUG_OBJS:.o=.d)
RELEASE_OBJS := $(SRCS:%=$(BUILD_DIR)/release/%.o)
RELEASE_DEPS := $(RELEASE_OBJS:.o=.d)

$(BUILD_DIR)/debug/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c $< -o $@

$(BUILD_DIR)/debug/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) -c $< -o $@

$(BUILD_DIR)/release/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(RELEASE_FLAGS) $(INCLUDE_FLAGS) -c $< -o $@

$(BUILD_DIR)/release/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(RELEASE_FLAGS) $(INCLUDE_FLAGS) -c $< -o $@

debug: $(DEBUG_OBJS)
	$(CXX) $(DEBUG_OBJS) $(DEBUG_FLAGS) $(INCLUDE_FLAGS) $(LINK_FLAGS) -o $(TARGET_EXEC)

release: $(RELEASE_OBJS)
	$(CXX) $(RELEASE_OBJS) $(RELEASE_FLAGS) $(INCLUDE_FLAGS) $(LINK_FLAGS) -o $(TARGET_EXEC)

.PHONY: debug

clean:
	rm -rf $(BUILD_DIR) main

-include $(DEBUG_DEPS) $(RELEASE_DEPS)

MKDIR_P ?= mkdir -p