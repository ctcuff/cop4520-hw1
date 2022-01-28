CXX = g++ -std=c++17

SRC = src/primes.cpp
ARGS = -Wall -lpthread
EXE_NAME = primes
OUT_DIR = ./out

all: $(SRC)
	@ mkdir -p out
	$(CXX) $(SRC) -o $(OUT_DIR)/$(EXE_NAME) $(ARGS)

run:
	@ make all
	@ ./out/primes

clean:
	@ rm -rf ./out
