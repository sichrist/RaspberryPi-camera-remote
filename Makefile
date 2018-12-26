#########################################
# 			Initialisation  			#
#########################################

SRC_DIR = src
TST_DIR = test
MAIN_DIR= examples
SRC_CPP = ${wildcard ${SRC_DIR}/*.cpp}
SRC_HPP = ${wildcard ${SRC_DIR}/*.hpp}
MAIN_CPP= ${wildcard ${MAIN_DIR}/*.cpp}

#########################################
# 			Compiler + Flags  			#
#########################################

CXX 		:= g++
LDFLAGS  	:= -pthread -lopencv_core -lopencv_highgui
CPPFLAGS 	:=
CXXFLAGS 	:=

#########################################
# 			Build environment  			#
#########################################

BUILD_DIR 	= build
OBJECTS_DIR = ${BUILD_DIR}/objects
OBJECTS 	= ${patsubst ${SRC_DIR}/%.cpp,${OBJECTS_DIR}/%.o, ${SRC_CPP} }
MAIN_OBJ 	= ${patsubst ${MAIN_DIR}/%.cpp,${OBJECTS_DIR}/%.o, ${MAIN_CPP} }
MAIN 		= ${patsubst ${MAIN_DIR}/%.cpp,%, ${MAIN_CPP} }
 
#########################################
# 			Default Target  			#
#########################################

default: clean build_environment ${OBJECTS} ${MAIN_OBJ} ${MAIN}

#########################################
# 			Compile Binary  			#
#########################################

${MAIN}:
	$(CXX) $(LDFLAGS) -o $@ $^ ${OBJECTS} ${OBJECTS_DIR}/$@.o

build_environment:
	mkdir -p ${OBJECTS_DIR}

#########################################
# 			Create .o files				#
#########################################

$(OBJECTS_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $< $(LDFLAGS) 

$(OBJECTS_DIR)/%.o: $(MAIN_DIR)/%.cpp 
	$(CXX) $(CPPFLAGS) -I${SRC_DIR} $(CXXFLAGS) -c -o $@ $<



.PHONY: test clean default build_environment 

clean:
	rm -rf ${BUILD_DIR} ${MAIN}