CXX			:= g++
CXXFLAGS	:= -std=c++11
LDFLAGS		:= 
BUILD 		:= ./build
OBJ_DIR		:= $(BUILD)/objects
APP_DIR		:= $(BUILD)/bin
LIB_DIR		:= $(BUILD)/lib
TARGETS		:= ./targets
TARGET_SCHED		:= sp_sched
TARGET_LIB 	:= libsubpub.a
INCLUDE		:= -Iinc/
SRC			:= $(wildcard src/*.cpp) \
			   $(TARGETS)/sched_main.cpp
LIB_SRC		:= $(wildcard src/*.cpp)


OBJECTS		:= $(SRC:%.cpp=$(OBJ_DIR)/%.o)
LIB_OBJECTS	:= $(LIB_SRC:%.cpp=$(OBJ_DIR)/%.o)

all: build $(APP_DIR)/$(TARGET_SCHED)

lib: build $(LIB_DIR)/$(TARGET_LIB)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@ $(LDFLAGS)

$(LIB_DIR)/$(TARGET_LIB): $(LIB_OBJECTS)
	@mkdir -p $(@D)
	ar rcs $(LIB_DIR)/$(TARGET_LIB) $^

$(APP_DIR)/$(TARGET_SCHED): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(APP_DIR)/$(TARGET_SCHED) $^ $(LDFLAGS)


.PHONY: all lib build clean debug release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(LIB_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

release: CXXFLAGS += -O2
release: all

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(APP_DIR)/*