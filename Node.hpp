/**
 * @file SparseMatrix.cpp
 * @authors Antonio Joabe Alves Morais  | Matrícula: 539029
 *          Iarley Natã Lopes Souza     | Matrícula: 535779
 * @brief
 * Arquivo de implementação de uma struct de Nó. */

#ifndef NODE_HPP
#define NODE_HPP

/* Estrutura nó da matriz */
struct Node {
	Node *right;    // Aponta para o próximo elemento não-nulo da linha
	Node *down;     // Aponta para o próximo elemento não-nulo da coluna
	int line;       // Dado da linha
	int col;        // Dado da coluna
	double value;   // Valor do elemento
	/* Construtor que inicializa o nó com seus 5 parâmetros */
	Node(Node *right, Node *down, int line, int col, double value) {
		this->right = right;
		this->down = down;
		this->line = line;
		this->col = col;
		this->value = value;
	}
};

#endif