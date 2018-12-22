#########################################
# 			Initialisation  			#
#########################################

BINARY  = ./main.exe
SRC_DIR = src
TST_DIR = test
SRC_CPP = ${wildcard ${SRC_DIR}/*.cpp}
SRC_HPP = ${wildcard ${SRC_DIR}/*.hpp}

#########################################
# 			Compiler + Flags  			#
#########################################

CXX 		:= g++
LDFLAGS  	:= 
CPPFLAGS 	:=
CXXFLAGS 	:=

#########################################
# 			Build environment  			#
#########################################

BUILD_DIR = build
OBJECTS_DIR = ${BUILD_DIR}/objects
OBJECTS = ${patsubst ${SRC_DIR}/%.cpp,${OBJECTS_DIR}/%.o, ${SRC_CPP} }

#########################################
# 			Default Target  			#
#########################################

default: build_environment ${BINARY}

#########################################
# 			Compile Binary  			#
#########################################

${BINARY}: $(OBJECTS)
	echo ${SRC_CPP}
	echo ${SRC_HPP}
	echo ${OBJECTS}
	$(CXX) $(LDFLAGS) -o $@ $^

build_environment:
	mkdir -p ${OBJECTS_DIR}

#########################################
# 			Create .o files				#
#########################################

$(OBJECTS_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf ${BUILD_DIR} ${BINARY}