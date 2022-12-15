/**
 * @file SparseMatrix.cpp
 * @authors Antonio Joabe Alves Morais  | Matrícula: 539029
 *          Iarley Natã Lopes Souza     | Matrícula: 535779
 * @brief
 * Arquivo de implementação de um TAD de uma Matriz Esparsa. */

#include "SparseMatrix.hpp"
#include <iostream>

SparseMatrix::SparseMatrix(int m, int n) {
	/* Lança uma exceção caso m ou n não forem positivos */
	if(m <= 0 || n <= 0)
		throw std::invalid_argument("Parameters m & n must be positive");

	/* Primeiro sentinela: linha 0, coluna 0 */
	Node * firstSentinel = new Node(nullptr, nullptr, 0, 0, 0);
	head = firstSentinel; // Faz o ptr. cabeça apontar pra ele
	firstSentinel->right = firstSentinel->down = firstSentinel; // E os ptrs. right e down do sentinela apontarem para ele mesmo

	Node *current = firstSentinel; // Ptr. usado para percorrer os sentinelas nos for's seguintes

	/* Inicializa os sentinelas das linhas */
	for(int i = 1; i <= m; ++i) {
		/* Inicializa e "trata" os atributos do novo nó sentinela */
		Node *sentinel = new Node(nullptr, nullptr, 0, 0, 0);
		sentinel->line = i; // Atribui o número da linha
		current->down = sentinel; // "Linka" o sentinela atual com o anterior
		sentinel->right = sentinel; // Faz o ptr. right apontar para ele mesmo
		sentinel->down = firstSentinel; // E o ptr. down apontar para o início da coluna
		current = current->down; // Anda o ptr. atual
		lineQty++; // Incrementa o número de linhas
	}

	current = firstSentinel; // Renicia o ptr.

	/* Inicializa os sentinelas das colunas */
	for(int j = 1; j <= n; ++j) {
		/* Inicializa e "trata" os atributos do novo nó sentinela */
		Node *sentinel = new Node(nullptr, nullptr, 0, 0, 0);
		sentinel->col = j; // Atribui o número da coluna
		current->right = sentinel; // "Linka" o sentinela atual com o anterior
		sentinel->right = firstSentinel; // Faz o ptr. right apontar para o início da linha
		sentinel->down = sentinel; // E o ptr. down apontar para ele mesmo
		current = current->right; // Anda o ptr. atual
		colQty++; // Incrementa o número de colunas
	}
}

SparseMatrix::~SparseMatrix() {
	Node *aux_line = head; 
	/* Deleta os elementos a partir da coluna 1 */
	while(aux_line->down != head) { 
		while(aux_line->right != aux_line){
			Node *aux_col = aux_line;
			while (aux_col->right->right != aux_line){
				aux_col = aux_col->right; // Anda na linha
			}
			delete aux_col->right; // Deleta o último elemento da linha
			aux_col->right = aux_line; // Faz o penúltimo (agr último) apontar para o início
		}
		aux_line = aux_line->down; // Anda na coluna
	}

	/* Deleta os elementos da coluna 0 */
	while(head->down != head) {
		aux_line = head;
		while(aux_line->down->down != head) {
			aux_line = aux_line->down; // Anda na coluna 0
		}
		delete aux_line->down; // Deleta o último elemento da coluna
		aux_line->down = head; // Faz o penúltimo (agr último) apontar para o início
	}

	delete head; // Deleta o primeiro sentinela (0, 0)

	head = aux_line = nullptr;
	lineQty = colQty = 0;
}

Node *SparseMatrix::getHead() { return head; }
int SparseMatrix::getLineQty() { return lineQty; }
int SparseMatrix::getColQty() { return colQty; }

void SparseMatrix::insert(int i, int j, double value) {
	/* Lança uma exceção se os parâmetros forem inválidos */
	if(i <= 0 || j <= 0 || i > lineQty || j > colQty) 
		throw std::invalid_argument("Coordinates must be positive and whitin the matrix");

	/* Inicializa os ptrs. que vão percorrer a matriz */
	Node *currentLine = head;
	Node *currentCol = head;

	for(int m = 1; m <= i; ++m)
		currentLine = currentLine->down;

	currentCol = currentLine;

	/* Se o nó(i, j) existir, substitui o valor dele ou desaloca */
	if(get(i, j)) {
		if(value != 0) { // Substitui
			while(currentCol->col != j)
				currentCol = currentCol->right;
			currentCol->value = value;
		}
		else { // Desaloca
			/* Aponta para o nó a ser desalocado */
			Node *temp = currentLine;
			while(temp->col != j)
				temp = temp->right;

			/* Anda até o nó mais próximo ao nó(i, j) */
			while(currentCol->right->col < j) {
				if(currentCol->right == currentLine)
					break;
				currentCol = currentCol->right;
			}

			currentCol->right = temp->right; // Faz o nó anterior ao nó(i, j) apontar para o posterior ao mesmo
			temp->right = nullptr;

			currentLine = currentCol = head; // Renicia os ptrs.

			for(int n = 1; n <= j; ++n) // Acha a coluna "j"
				currentCol = currentCol->right;
			currentLine = currentCol;

			/* Faz a mesma coisa da linha 143, só que em relação à linha */
			while(currentLine->down->line < i) {
				if(currentLine->down == currentCol)
					break;
				currentLine = currentLine->down;
			}

			currentLine->down = temp->down; // Faz a mesma coisa da linha 150, só que em relação à linha
			temp->down = nullptr;

			delete temp; // Desaloca o nó(i, j)
		}
	}
	else { // Insere
		if(value == 0)
			return;

		/* Inicializa e "trata" os atributos do nó a ser inserido */
		Node *newNode = new Node(nullptr, nullptr, 0, 0, 0);
		newNode->line = i;
		newNode->col = j;
		newNode->value = value;

		/* Anda a coluna até o mais próx da posição em que o novo nó vai ser inserido */
		while(currentCol->right->col < j) { 
			if(currentCol->right == currentLine)
				break;
			currentCol = currentCol->right;
		}
		newNode->right = currentCol->right; // Faz o novo nó apontar para onde o nó anterior aponta
		currentCol->right = newNode; // E o anterior apontar para onde o novo nó

		/* Renicia os 2 ptrs. e encontra a coluna "j" */
		currentLine = currentCol = head;
		for(int n = 1; n <= j; ++n) {
			currentCol = currentCol->right;
		}
		currentLine = currentCol;

		/* E faz a mesma coisa da linha 180, só que em relação à linha */
		while(currentLine->down->line < i) {
			if(currentLine->down == currentCol)
				break;
			currentLine = currentLine->down;
		}
		newNode->down = currentLine->down;
		currentCol->down = newNode;
	}
}

double SparseMatrix::get(int i, int j) {
	/* Lança uma exceção se a coordenada não for válida */
	if(i <= 0 || j <= 0 || i > lineQty || j > colQty) 
		throw std::invalid_argument("Coordinates must be positive and within the matrix");

	Node *currentLine = head->down; // Ponteiro que aponta para o início das linhas	
	/* Encontra a linha "i" */
	while(currentLine->line != i)
		currentLine = currentLine->down;

	Node *currentCol = currentLine->right; // Ponteiro que aponta para os nós na linha "i" 
	/* Faz "currentCol" percorrer a linha "i" até que o nó(i, j) seja encontrado */
	while(currentCol->col != j) { 
		currentCol = currentCol->right;
		if(currentCol->right == currentLine && currentCol->col != j)
			return 0; // Se a lista foi percorrida e o nó(i, j) não foi encontrado, retorna "0"
	}
	return currentCol->value; // Se o nó(i, j) foi encontrado, retorna seu valor.
}

void SparseMatrix::print() {
	for(int i = 1; i <= lineQty; ++i) {
		for(int j = 1; j <= colQty; ++j) {
			std::cout << get(i, j) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}