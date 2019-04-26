// SudokuAStar.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <map>

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
			if (value == 0) {
				if (values[i] != 0) {
					len++;
				}
			}
		return len;
	}

	unsigned int Count() {
		int len = 0;

		for (int i = 0; i < SIZE*SIZE; i++)
			if (value == 0) {
				if (values[i] != 0) {
					len++;
				}
			}
			else break;
		return len;
	}
};

unsigned long product(CellState *game) {
	unsigned long C=1;

	for (int i = 0; i < SIZE*SIZE; i++) {
		for (int j = 0; j < SIZE*SIZE; j++) {
			CellState state =  *(game+ i* SIZE*SIZE + j);
			if (state.value!=0)
			C *= state.value;
		}
	}

	return C;
}

unsigned long sum(CellState *game) {
	unsigned long C=0;

	for (int i = 0; i < SIZE*SIZE; i++) {
		for (int j = 0; j < SIZE*SIZE; j++) {
			CellState state = *(game + i * SIZE*SIZE + j);
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

	for (int row = 0; row < SIZE*SIZE; row++) {
		for (int col = 0; col < SIZE*SIZE; col++) {

			CellState *state = (new_state + row *SIZE*SIZE + col);

			if (state->value == 0) {
				cout << "\n" << "PREPROCESS " << row << "," << col << "="<< state->value;

				cout << "\nH={ ";
				//valutare quali elementi rimuovere dalla RIGA I-ESIMA
				for (int k = 0; k < SIZE*SIZE; k++) {
					CellState cellState = *(new_state + k * SIZE*SIZE + row);
					
					if ( cellState.value != 0 ) {
						cout << cellState.value;
						state->removeValue(cellState.value);
					}
				}
				cout << "}";

				cout << "\nV={";
				//valutare quali elementi rimuovere dalla COLONNA I-ESIMA
				for (int k = 0; k < SIZE*SIZE; k++) {
					CellState cellState = *(new_state + col * SIZE*SIZE + k);
					if ( cellState.value != 0) {
						cout << cellState.value;
						state->removeValue(cellState.value);
					}
				}
				cout << "}";

				////valutare quali elementi rimuovere dal  Quadrante i-esimo
				int q_row = row / SIZE;
				int q_col = col / SIZE;

				int q_offset = q_row * SIZE * SIZE * SIZE+ q_col* SIZE;
				

				cout << "\nQ(" << q_row << ","<<q_col <<")";

				CellState cellState;
				cout << "\nQ={";
				for (int y = 0; y < SIZE; y++) {
					for (int x = 0; x < SIZE; x++) {
						
						cellState = *(new_state + q_offset +  y * SIZE*SIZE  + x);
						
						if (&cellState == state)
							continue;

						if (cellState.value != 0) {
							cout << cellState.value;

							state->removeValue(cellState.value);
						}
					}
				}
				cout << "}";
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


vector<CellState*> *GetChildStates(CellState* current_state) {
	unsigned long min_values_count= SIZE * SIZE;

	vector<CellState *> *child_states = new vector<CellState *>();

	//foreach cell check mins CountValues
	for (int j = 0; j < SIZE*SIZE; j++) {
		for (int i = 0; i < SIZE*SIZE; i++) {
			CellState state = *(current_state + i * SIZE*SIZE + j);
			if (state.value==0)
				if (state.CountValues() < min_values_count)
					min_values_count = state.CountValues();
		}
	}

	for (int j = 0; j < SIZE*SIZE; j++) {
		for (int i = 0; i < SIZE*SIZE; i++) {
			CellState state = *(current_state + i * SIZE*SIZE + j);

			if (state.CountValues() == min_values_count) {
				for (int k = 0; k < SIZE*SIZE; k++) {
					int p_value = state.values[k];

					if (p_value == 0)
						continue;

					CellState *new_state = (CellState*)malloc(sizeof(CellState)*(SIZE*SIZE)*(SIZE*SIZE));

					//oid* memcpy( void* dest, const void* src, std::size_t count );
					memcpy(new_state, current_state, sizeof(CellState)*(SIZE*SIZE)*(SIZE*SIZE));

					CellState *p_state = (new_state + i * SIZE*SIZE + j);
					memset(&p_state->values,0, sizeof(p_state->values) * sizeof(int));
					p_state->value = p_value;

					CellState *child_state = preprocess((CellState *)new_state);

					child_states->push_back(child_state);
				}
			}
		}
	}

	return child_states;
}

map<unsigned long, CellState*> state_queue;

void Solver(CellState* current_state) {
	unsigned long G = sum((CellState *)current_state);

	cout << "\nSUM : " << G;

	state_queue[G] = current_state;

	vector<CellState*>*child_state = GetChildStates(current_state);

	unsigned long G_min = G;

	//calculate G for every next node
	CellState* next_state = current_state;

	for (vector<CellState*>::iterator it = child_state->begin(); it != child_state->end(); ++it) {
		cout << "\n * * * * * * * * * CHILD";
		unsigned long G = sum(*it);
		cout << "\nG:" << G;
		printSudoku(*it);
		printValues(*it);

		if (G_min > G) {
			G_min = G;
			//current next state
			next_state = *it;
		}
	}

	//if G=0 return
	//SOLVED
	if (G_min == 0)
		return;
	else {
		//call solver with the lowes one.
		if ( next_state == current_state)
			return;

		Solver(next_state);
	}

	//else
	//if solution is not possible, get the priority queue and try to expand the next value
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

	CellState *c1 = preprocess((CellState *)c0);
	
	printSudoku(c1);
	printValues(c1);

	Solver(c1);
}


