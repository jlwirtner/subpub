CXX			:= g++
CXXFLAGS	:= -std=c++17
LDFLAGS		:= 
BUILD 		:= ./build
THIRD_PARTY	:= ./third_party
OBJ_DIR		:= $(BUILD)/objects
APP_DIR		:= $(BUILD)/bin
LIB_DIR		:= $(BUILD)/lib
TARGETS		:= ./targets
TARGET_SCHED		:= sp_sched
TARGET_LIB 	:= libsubpub.a
TARGET_TEST	:= sp_test
INCLUDE		:= -Iinc/
SRC			:= $(wildcard src/*.cpp) \
			   $(TARGETS)/sched_main.cpp
LIB_SRC		:= $(wildcard src/*.cpp)
TEST_SRC	:= $(TARGETS)/test.cpp
CATCH_INCLUDE := -Ithird_party/


OBJECTS		:= $(SRC:%.cpp=$(OBJ_DIR)/%.o)
LIB_OBJECTS	:= $(LIB_SRC:%.cpp=$(OBJ_DIR)/%.o)

all: build $(APP_DIR)/$(TARGET_SCHED)

lib: build $(LIB_DIR)/$(TARGET_LIB)

test: build $(APP_DIR)/$(TARGET_TEST)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@ $(LDFLAGS)

$(APP_DIR)/$(TARGET_TEST): $(TEST_SRC) $(LIB_DIR)/$(TARGET_LIB)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(CATCH_INCLUDE) $(INCLUDE) $< -o $@ $(LDFLAGS) -L $(LIB_DIR) -lsubpub

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