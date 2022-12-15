/**
 * @file SparseMatrix.cpp
 * @authors Antonio Joabe Alves Morais  | Matrícula: 539029
 *          Iarley Natã Lopes Souza     | Matrícula: 535779
 * @brief
 * Arquivo protóripo de um TAD de uma Matriz Esparsa. */

#ifndef SPARSEMATRIX_HPP
#define SPARSEMATRIX_HPP

#include "Node.hpp"

/* TAD da Matriz Esparsa */
class SparseMatrix {
private:
	Node *head { nullptr }; // Ponteiro inicial da matriz
	int lineQty { 0 }; // Número de linhas da matriz
	int colQty { 0 }; // Número de colunas da matriz
public:
	/* Inicializa a matriz para conter m linhas e n colunas,
	 * além de checar se os parâmetros são > 0.
	 * Caso contrário, lança uma exceção. */
	SparseMatrix(int m, int n);

	/* Libera a memória alocada pela "SparseMatrix(int m, int n)". */
	~SparseMatrix();

	Node *getHead();
	int getLineQty();
	int getColQty();

	/* Insere um nó na posição (i, j) se não houver um nó lá.
	 * Se houver, só substitui o valor dele.
	 * Além disso, se o parametro "value" tiver valor "0",
	 * desaloca o nó(i, j), se ele existir. */
	void insert(int i, int j, double value);

	/* Retorna o valor do nó na posição (i, j)
	 * se o mesmo existir. Retorna "0" caso contrário. */
	double get(int i, int j);

	/* Imprime a matriz. Se um nó não existir, imprime "0" no lugar */
	void print();
};

#endif