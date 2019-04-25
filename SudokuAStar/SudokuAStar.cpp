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

	CellState() {
		memset(&values, 0, SIZE*SIZE*4);
		value = 0;
	}

	void removeValue(int value) {
		if (value == 0)
			return;

		for (int i = 0; i < SIZE*SIZE; i++)
			if (values[i] == value)
				values[i] = 0;

		if (CountValues()==1) {
			for (int i = 0; i < SIZE*SIZE; i++)
				if (values[i] !=0)
					value = values[i];
		}
	}

	unsigned int CountValues() {
		int len = 0;

		for (int i = 0; i < SIZE*SIZE; i++)
			if (values[i] != 0) {
				len++;
			}
			
		return len;
	}

	unsigned int Count() {
		int len = 0;

		for (int i = 0; i < SIZE*SIZE; i++)
			if (values[i] != 0) {
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

CellState* preprocess(CellState* current_state) {
	CellState *new_state = (CellState*)malloc(sizeof(CellState)*(SIZE*SIZE)*(SIZE*SIZE));

	//oid* memcpy( void* dest, const void* src, std::size_t count );
	memcpy(new_state, current_state, sizeof(CellState)*(SIZE*SIZE)*(SIZE*SIZE));

	for (int i = 0; i < SIZE*SIZE; i++) {
		for (int j = 0; j < SIZE*SIZE; j++) {
			CellState *state = (new_state + i * SIZE*SIZE + j);
			if (state->value == 0) {
			
				//valutare quali elementi rimuovere dalla RIGA I-ESIMA
				for (int k = 0; k < SIZE*SIZE; k++) {
					CellState cellState = *(new_state + k * SIZE*SIZE + j);
					if (k != j && cellState.value != 0) {
						state->removeValue(cellState.value);
					}
				}

				//valutare quali elementi rimuovere dalla COLONNA I-ESIMA
				for (int k = 0; k < SIZE*SIZE; k++) {
					CellState cellState = *(new_state + i * SIZE*SIZE + k);
					if (k != i && cellState.value != 0) {
						state->removeValue(cellState.value);
					}
				}

				////valutare quali elementi rimuovere dal  Quadrante i-esimo
				int q_i = i / SIZE;
				int q_j = j / SIZE;

				int q_offset_x = q_i * SIZE;
				int q_offset_y = q_j * SIZE;


				for (int x = 0; x < SIZE; x++) {
					for (int y = 0; y < SIZE; y++) {
						CellState cellState = *(new_state + (q_offset_x + x) + (q_offset_y + y));
						if (cellState.value != 0) {
							state->removeValue(cellState.value);
						}
					}
				}



			}
		}
	}

	return (CellState*)new_state;
}

/*
Prints table
*/
void printSudoku(CellState* current_state) {
	cout << "\n     0   1   2   3   4   5   6   7   8";
	cout << "\n   \xDA\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xC2\xC4\xC4\xC4\xBF";
	
	for (int j = 0; j < SIZE*SIZE; j++) {
		cout << "\n " << j << " \xB3";
		for (int i = 0; i < SIZE*SIZE; i++) {
				CellState state = *(current_state + i * SIZE*SIZE + j);
				if (state.value!=0)
			cout <<" " << state.value << " " << "\xB3";
				else
					cout << "   " << "\xB3";
		}
		
		if (j!= SIZE * SIZE-1)
			cout << "\n   \xC3\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xC5\xC4\xC4\xC4\xB4";

	}

	cout << "\n   \xC0\xC4\xC4\xC4\xC1\xC4\xC4\xC4\xC1\xC4\xC4\xC4\xC1\xC4\xC4\xC4\xC1\xC4\xC4\xC4\xC1\xC4\xC4\xC4\xC1\xC4\xC4\xC4\xC1\xC4\xC4\xC4\xC1\xC4\xC4\xC4\xD9\n";
}


/*
prints possible values
*/
void printValues(CellState* current_state) {
	for (int j = 0; j < SIZE*SIZE; j++) {
		for (int i = 0; i < SIZE*SIZE; i++) {
			CellState state = *(current_state + i * SIZE*SIZE + j);

			if (state.value == 0) {
				cout << "\n(" << j << "," << i << ")=";
				for (int k = 0; k < SIZE*SIZE; k++)
					if (state.values[k] != 0)
						cout << " " << state.values[k];
			}
		}
	}
}

int main()
{

	std::cout << "Sudoku!\n";

	CellState c0[SIZE*SIZE][SIZE*SIZE];
	
	c0[0][2].value = 4;
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
			CellState *state = &c0[i][j];
			if (state->value==0)
				for (int k = 0; k < SIZE*SIZE; k++)
					state->values[k] = k + 1;
		}
	}

	std::cout << "Sudoku is ready.";


	
	printSudoku((CellState *)c0);
	printValues((CellState *)c0);


	CellState *c1 = preprocess((CellState *)c0);
	
	printSudoku((CellState *)c1);
	printValues((CellState *)c1);


}


