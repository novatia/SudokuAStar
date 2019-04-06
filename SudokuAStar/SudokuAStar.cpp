// SudokuAStar.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>

using namespace std;

#define SIZE 3

class CellState {
public:
	int value = 0;
	int values[SIZE*SIZE];

	void removeValue(int value) {
	
	}

	unsigned int Count() {
		int len = 0;

		for (int i = 0; i < SIZE*SIZE; i++)
			if (values != 0) {
				len++;
			}
			else break;
		return len;
	}
};

unsigned long product(CellState *game) {
	unsigned long C=1;

	for (int i = 0; i < SIZE*SIZE; i++) {
		for (int j = 0; j < SIZE*SIZE; j++) {
			CellState state =  *(game+ i* SIZE+ j);
			C *= state.value;
		}
	}

	return C;
}

unsigned long sum(CellState *game) {
	unsigned long C=0;

	for (int i = 0; i < SIZE*SIZE; i++) {
		for (int j = 0; j < SIZE*SIZE; j++) {
			CellState state = *(game + i * SIZE + j);
			C += state.Count();
		}
	}

	C -= SIZE * SIZE;

	return C;
}

void processCellState(CellState *map,CellState *cellState,int i, int j) {
	//valutare quali elementi rimuovere dalla RIGA I-ESIMA
	for (int k = 0; k < SIZE*SIZE; k++) {
		CellState state = *(map + k * SIZE + j);
		if (k != j && state.value != 0) {
			cellState->removeValue(state.value);
		}
	}
	//valutare quali elementi rimuovere dalla COLONNA I-ESIMA
	for (int k = 0; k < SIZE*SIZE; k++) {
		CellState state = *(map + i * SIZE + k);
		if (k != i && state.value != 0) {
			cellState->removeValue(state.value);
		}
	}

	////valutare quali elementi rimuovere dal  Quadrante i-esimo
	int q_i = i / SIZE;
	int q_j = j / SIZE;

	int q_offset_x = q_i * SIZE;
	int q_offset_y = q_j * SIZE;


	for (int x = 0; x < SIZE; x++) {
		for (int y = 0; y < SIZE; y++) {
			CellState state = *(map + (q_offset_x +x) + (q_offset_y + y));
			if (state.value != 0) {
				cellState->removeValue(state.value);
			}
		}
	}
}

CellState* preprocess(CellState* current_state) {
	CellState new_state[SIZE*SIZE][SIZE*SIZE];

	//oid* memcpy( void* dest, const void* src, std::size_t count );

	memcpy(new_state, current_state, sizeof(CellState)*(SIZE*SIZE)*(SIZE*SIZE));

	for (int i = 0; i < SIZE*SIZE; i++) {
		for (int j = 0; j < SIZE*SIZE; j++) {
			CellState state = *(current_state + i * SIZE + j);
			if (state.value == 0) {
				processCellState(current_state,&state,i,j);
			}
		}
	}

	return (CellState*)new_state;
}


void printSudoku(CellState* current_state) {
	cout << "----------------------------------------\n";
		for (int j = 0; j < SIZE*SIZE; j++) {
			for (int i = 0; i < SIZE*SIZE; i++) {
				CellState state = *(current_state + i * SIZE + j);
			cout <<" " << state.value << " " << "|";
		}
		cout << "\n";
	}
}

int main()
{

	std::cout << "Sudoku!\n";

	CellState c0[SIZE*SIZE][SIZE*SIZE];
	
	c0[0][2].value= 4;
	c0[0][5].value = 8;
	c0[0][6].value = 2;
	c0[0][7].value = 1;
	c0[0][8].value = 5;
	c0[1][1].value = 5;
	c0[1][2].value = 7;
	c0[1][7].value = 8;
	c0[2][0].value = 1;
	c0[2][4].value = 4;
	c0[2][5].value = 6;
	c0[3][7].value = 2;
	c0[3][8].value = 3;
	c0[4][3].value = 8;
	c0[4][4].value = 9;
	c0[4][5].value = 3;
	c0[5][0].value = 4;
	c0[5][1].value = 3;
	c0[6][3].value = 2;
	c0[6][4].value = 8;
	c0[6][8].value = 9;
	c0[7][1].value = 1;
	c0[7][6].value = 6;
	c0[7][7].value = 4;
	c0[8][0].value = 2;
	c0[8][1].value = 9;
	c0[8][2].value = 5;
	c0[8][3].value = 7;
	c0[8][6].value = 8;
	
	//numero di step per il calcolo del nodo i-esimo da incrementare ogni volta che si fa un loop e a cui va sommata la produttoria
	int G = 1;

	//inizializzo stati con valori possibili
	for (int i = 0; i < SIZE*SIZE; i++) {
		for (int j = 0; j < SIZE*SIZE; j++) {
			CellState state = c0[i][j];
			if (state.value==0)
				for (int k = 0; k < SIZE; k++)
					state.values[k] = k + 1;
		}
	}

	std::cout << "Sudoku is ready.";


	CellState *c1 = preprocess((CellState *)c0);

	printSudoku((CellState *)c0);
}


