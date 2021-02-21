INC = include
SRC = src
OBJ = obj
BIN = bin
CXX = g++
CPPFLAGS = -O2 -Wall -g -c -std=c++11 -w

BIN_C = $(BIN)/cluster
OBJ_C = $(OBJ)/util.o $(OBJ)/cluster.o $(OBJ)/random.o $(OBJ)/trayectoria.o 
SRC_C = $(SRC)/util.cpp $(SRC)/cluster.cpp $(SRC)/random.cpp $(SRC)/trayectoria.cpp 

all: preambulo \
cluster

#******************************
preambulo:
	@echo 
	@echo -----------------------------------------------
	@echo Práctica 3
	@echo Metaheurística - Grupo 1
	@echo 
	@echo Grado en Ingeniería Informática
	@echo Curso 19-20
	@echo 
	@echo Realizado por
	@echo Cumbreras Torrente, Paula
	@echo 49087324-B
	@echo -----------------------------------------------	
	@echo 

#******* COMPILACION **********
cluster: $(BIN_C)

$(BIN_C): $(OBJ_C)
	$(CXX) $^ -o $@

$(OBJ)/%.o: $(SRC)/%.cpp
	$(CXX) $(CPPFLAGS) $^ -o $@ -I$(INC)


#********** LIMPIEZA **********
clean:
	rm -f $(OBJ)/*
	echo "Objetos eliminados"

mrproper: clean
	rm -f $(BIN)/*
	echo "Ejecutables eliminados"
