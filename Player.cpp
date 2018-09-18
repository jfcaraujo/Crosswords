#include "Player.h"

void Player::PlayerName(){ // Introduz o nome do jogador
	while (true) {
		cout << "Introduce your name: "; cin >> name;
		if (cin.fail()) {
			cin.clear();
			//	cin.ignore(100, '\n');
			cout << "That is not a valid name!" << endl;
		}
		else break;
	}
	clues = 0;
	return;
}

string Player::ShowName(){ // Se for preciso para mostar o nome do jogador senao apagas Filipe
	cout << "Player name: " << name << endl;
	return name;
}

void Player::StartTime(){ // Guardar o tempo inicial
	start = clock(); // Tempo Inicial
	return;
}

double Player::TotalTime(){ // Mostrar o tempo final
	double duration;
	duration = (clock() - start) / (double)CLOCKS_PER_SEC; // Tempo duracao do jogo
	cout << "Game time: " << duration << " seconds" << endl;
	return duration;
}

void Player::newClue(){ // acrescenta uma pista
	clues++;
	return;
}

int Player::showclues() {
	cout << "Clues: " << clues << endl;
	return clues;
}