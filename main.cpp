/**
 * @file Node.hpp
 * @authors Antonio Joabe Alves Morais  | Matrícula: 539029
 *          Iarley Natã Lopes Souza     | Matrícula: 535779
 * @brief
 * Arquivo de testes e implementação de funções operacionais da matriz. */

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "SparseMatrix.hpp"

using namespace std;

/* Soma 2 matrizes e retorna uma matriz resultado */
SparseMatrix *sum(SparseMatrix *A, SparseMatrix *B);
/* Multiplica 2 matrizes e retorna uma matriz resultado */
SparseMatrix *multiply(SparseMatrix *A, SparseMatrix *B);
/* Lê um arquivo .txt e monta uma matriz com base nos dados desse arquivo */
SparseMatrix *readSparseMatrix(string fileName);

int main() {
	vector<SparseMatrix*> matrices;

	while(true) {
		string command, token;
		getline(cin, command);

		stringstream ss;
		ss << command;
		ss >> token;

		cout << "$" << ss.str() << endl;

		if(token == "create") {
			int line, col;
			ss >> line >> col;
			SparseMatrix *newMatrix = new SparseMatrix(line, col);
			matrices.push_back(newMatrix);
		}
		else if(token == "insert") {
			int line, col, index;
			double value;
			ss >> line >> col >> value >> index;
			matrices[index]->insert(line, col, value);
		}
		else if(token == "get") {
			int line, col, index;
			ss >> line >> col >> index;
			cout << matrices[index]->get(line, col) << endl;
		}
		else if(token == "show") {
			int index;
			ss >> index;
			cout << "Matriz " << index << ":\n"; 
			matrices[index]->print();
		}
		else if(token == "showAll") {
			for(int i = 0; i < matrices.size(); i++) {
				cout << "Matriz " << i << ":" << endl; 
				matrices[i]->print();
			}
		}
		else if(token == "sum") {
			int m1, m2;
			ss >> m1 >> m2;
			SparseMatrix *newMatrix = sum(matrices[m1], matrices[m2]);
			matrices.push_back(newMatrix);
		}
		else if(token == "mult") {
			int m1, m2;
			ss >> m1 >> m2;
			SparseMatrix *newMatrix = multiply(matrices[m1], matrices[m2]);
			matrices.push_back(newMatrix);
		}
		else if(token == "exit") {
			for(int i = 0; i < matrices.size(); i++) {
				delete matrices[i];
			}
			break;
		}
		else if(token == "read") {
			string file;
			ss >> file;
			SparseMatrix *newMatrix = readSparseMatrix(file);
			matrices.push_back(newMatrix);
		}
		else {
			cout << "Unknown command, please enter another input:" << endl;
		}
	}

	return 0;
}

SparseMatrix *sum(SparseMatrix *A, SparseMatrix *B) {
	if(A->getLineQty() != B->getLineQty() || A->getColQty() != B->getColQty()) {
		throw std::runtime_error("Matrices must have equal orders");
	}

	SparseMatrix *C = new SparseMatrix(A->getLineQty(), A->getColQty()); // Matriz C é inicializada com a mesma ordem de A e B

	/* Este bloco de código preenche os espacos(i, j) de C com os respectivos valores(i, j) de A.
	Basicamente, C vira uma cópia de A. */
	Node *auxLine = A->getHead()->down; // Ptr que apontará para as linhas (respectivo nó sentinela).
	while(auxLine != A->getHead()) {
		Node *auxCol = auxLine; // Ptr que percorre os nós das linhas apontadas por auxLine.

		while(auxCol->right != auxLine) {
			/* À medida que auxCol percorre a linha, inserimos seus valores(i, j) nos espaços(i, j) de C */
			auxCol = auxCol->right;
			C->insert(auxCol->line, auxCol->col, A->get(auxCol->line, auxCol->col));
		}
		auxLine = auxLine->down; // Faz com que auxLine aponte para a próx linha.
	}

	/* O processo anterior é repetido, mas somamos os valores de B(i, j) aos valores de C(i, j) */
	auxLine = B->getHead()->down; // Ptr que apontará para as linhas (respectivo nó sentinela).
	while(auxLine != B->getHead()) {
		Node *auxCol = auxLine; // Ptr que percorre os nós das linhas apontadas por auxLine.

		while(auxCol->right != auxLine) {
			/* À medida que auxCol percorre a linha, somamos B(i, j) à C(i, j) e inserimos */
			auxCol = auxCol->right;
			C->insert(auxCol->line, auxCol->col, C->get(auxCol->line, auxCol->col) + B->get(auxCol->line, auxCol->col));
		}
		auxLine = auxLine->down; // Faz com que auxLine aponte para a próx linha.
	}

	return C;
}

SparseMatrix *multiply(SparseMatrix *A, SparseMatrix *B) {
	if(A->getColQty() != B->getLineQty()) {
		throw std::runtime_error("Number of collumns of the 1st matrix must be equal to the number of lines of the 2nd matrix");
	}

	SparseMatrix *C = new SparseMatrix(A->getLineQty(), B->getColQty());

	Node *auxLineA = A->getHead(); // Aponta para as linhas de A
	Node *auxColB = B->getHead();	// Aponta para as colunas de B

	/* Faz com que auxLineA alterne entre as linhas de A */
	for(int i = 1; i <= A->getLineQty(); i++) {
		auxLineA = auxLineA->down;
		
		for(int j = 1; j <= B->getColQty(); j++) { // Faz com que auxColB alterne entre as colunas de B
			auxColB = auxColB->right;
			Node *currentA = auxLineA->right; // Percorre as linhas de A
			Node *currentB = auxColB->down;  // Percorre as colunas de B
			static double sum = 0; // Guarda a soma dos produtos dos elementos das linhas e colunas de A e B
			for(int k = 1; k <= A->getColQty(); k++) { // Faz com que correA e correB percorram as linhas e colunas de A e B, respectivamente
				sum = sum + A->get(i, k) * B->get(k, j);
				currentA = currentA->right;
				currentB = currentB->down;
			}
			C->insert(i, j, sum); // Insere o valor da soma no nó(i, j) de C
			sum = 0;
		}
	}

	return C;
}

SparseMatrix *readSparseMatrix(string fileName) {
	ifstream file;
	int lineQty, colQty, i, j;
	double value;

	/* Abre o arquivo com o nome passado pelo parâmetro da função */
	file.open(fileName);
	/* Se não conseguir abrir o arquivo, lança uma exceção */
	if(!file.is_open()) {
		throw std::runtime_error("File not found");
	}

	/* Lê a quantidade de linhas e colunas da matriz */
	file >> lineQty >> colQty;
	SparseMatrix *matrix = new SparseMatrix(lineQty, colQty);
	/* Lê as cordenadas, os valores e insere-os na matriz */
	while(file >> i >> j >> value) {
		matrix->insert(i, j, value);
	}

	return matrix;
}