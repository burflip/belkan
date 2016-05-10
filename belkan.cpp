#include "belkan.h"
#include "conexion.h"
#include "environment.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <cstdlib>
#include <ctime>

using namespace std;
void PasarVectoraMapaCaracteres(int fila, int columna, char m[200][200], char *v, int brujula) {
	m[fila][columna] = v[0];

	switch (brujula) {
	case 0: // Orientacion Norte

		m[fila - 1][columna] = v[1];
		for (int i = 0; i < 3; i++) {
			m[fila - 2][columna + i - 1] = v[2 + i];
		}
		for (int j = 0; j < 5; j++) {
			m[fila - 3][columna + j - 2] = v[5 + j];
		}
		break;
	case 1: // Orientacion Este
		m[fila][columna + 1] = v[1];
		for (int i = 0; i < 3; i++) {
			m[fila + i - 1][columna + 2] = v[2 + i];
		}
		for (int j = 0; j < 5; j++) {
			m[fila + j - 2][columna + 3] = v[5 + j];
		}
		break;
	case 2: // Orientacion Sur
		m[fila + 1][columna] = v[1];
		for (int i = 0; i < 3; i++) {
			m[fila + 2][columna + 1 - i] = v[2 + i];
		}
		for (int j = 0; j < 5; j++) {
			m[fila + 3][columna + 2 - j] = v[5 + j];
		}
		break;
	case 3: // Orientacion Oeste
		m[fila][columna - 1] = v[1];
		for (int i = 0; i < 3; i++) {
			m[fila + 1 - i][columna - 2] = v[2 + i];
		}
		for (int j = 0; j < 5; j++) {
			m[fila + 2 - j][columna - 3] = v[5 + j];
		}

		break;
	}

}
Agent::Agent() {
	srand(time(NULL));
	x_ = 99;
	y_ = 99;
	orientacion_ = 3;
	role_ = "PlYR";
	last_accion_ = 3;
	REINICIADO_ = false;
	size_ = 200;
	FIN_JUEGO_ = false;
	EN_USO_ = '0';
	SALUD_ = '5';
	for (int i = 0; i < 200; i++)
		for (int j = 0; j < 200; j++) {
			mapa_entorno_[i][j] = '?';
			mapa_objetos_[i][j] = '?';
		}

	for (int i = 0; i < 100; i++)
		for (int j = 0; j < 100; j++)
			mapa_solucion_[i][j] = '?';

	brain = Brain(*this);
	solved = false;

}
void Agent::Perceive(Environment &env) {
	env.SenSorStatus(VISTA_, SURFACE_, MENSAJE_, REINICIADO_, EN_USO_, MOCHILLA_, PUNTUACION_, FIN_JUEGO_, SALUD_, false);
}

bool Agent::Perceive_Remote(conexion_client &Cliente, Environment &env) {
	bool actualizado = false;

	actualizado = env.Perceive_Remote(Cliente);
	if (actualizado)
		env.SenSorStatus(VISTA_, SURFACE_, MENSAJE_, REINICIADO_, EN_USO_, MOCHILLA_, PUNTUACION_, FIN_JUEGO_, SALUD_, true);

	return actualizado;
}

string ActionStr(Agent::ActionType accion) {
	switch (accion) {
	case Agent::actFORWARD:
		return "FORWARD";
	case Agent::actTURN_L:
		return "TURN LEFT";
	case Agent::actTURN_R:
		return "TURN RIGHT";
	case Agent::actIDLE:
		return "IDLE";
	case Agent::actPICKUP:
		return "PICK UP";
	case Agent::actPUTDOWN:
		return "PUT DOWN";
	case Agent::actPUSH:
		return "PUSH";
	case Agent::actPOP:
		return "POP";
	case Agent::actGIVE:
		return "GIVE";
	case Agent::actTHROW:
		return "THROW";
	default:
		return "????";
	}
}

// -----------------------------------------------------------
void Agent::ActualizarInformacion(Environment *env) {
	// Actualizar mi informacion interna
	if (REINICIADO_) {
		for (int i = 0; i < 200; i++) {
			for (int j = 0; j < 200; j++) {
				mapa_entorno_[i][j] = '?';
				mapa_objetos_[i][j] = '?';
			}
		}
		x_ = 99;
		y_ = 99;
		orientacion_ = 3;
	}

	switch (last_accion_) {
	case 0: //avanzar
		switch (orientacion_) {
		case 0: // norte
			y_--;
			break;
		case 1: // este
			x_++;
			break;
		case 2: // sur
			y_++;
			break;
		case 3: // este
			x_--;
			break;
		}
		break;
	case 1: // girar izq
		orientacion_ = (orientacion_ + 3) % 4;
		break;
	case 2: // girar dch
		orientacion_ = (orientacion_ + 1) % 4;
		break;
	}

	// Comprobacion para no salirme del rango del mapa
	bool algo_va_mal = false;
	if (y_ < 0) {
		y_ = 0;
		algo_va_mal = true;
	} else if (y_ > 199) {
		y_ = 199;
		algo_va_mal = true;
	}
	if (x_ < 0) {
		x_ = 0;
		algo_va_mal = true;
	} else if (x_ > 199) {
		x_ = 199;
		algo_va_mal = true;
	}

	if (algo_va_mal) {
		cout << "CUIDADO: NO ESTAS CONTROLANDO BIEN LA UBICACION DE TU AGENTE\n";
	}

	PasarVectoraMapaCaracteres(y_, x_, mapa_entorno_, VISTA_, orientacion_);
	PasarVectoraMapaCaracteres(y_, x_, mapa_objetos_, SURFACE_, orientacion_);
	env->ActualizarMatrizUsuario(mapa_entorno_);
	this->the_env = env;

}

// -----------------------------------------------------------
Agent::ActionType Agent::Think() {
	Agent::ActionType accion;
	if (!this->solved) {
		accion = (Agent::ActionType) brain.Think(*this);
	} else {
		cout << "PORCENTAJE: " << the_env->Comparar(mapa_solucion_) << "%" << endl;
		the_env->PutFinJuego();
		printSolutionMap();
		exit(0);
	}
	last_accion_ = accion;
	return accion;
}

pair<int, int> Agent::getCoord() {
	return pair<int, int>(x_, y_);
}

void Agent::cropAndStoreSolutionMap() {
	printSolutionMap();
	pair<int, int> fc_top_left;
	int map_size = CURRENT_MAP_SIZE;
	bool first_appear_i = false, first_appear_j = false;
	int first_i, first_j;
	for (int i = 0; i < MAP_MAX_SIZE; i++) {
		for (int j = 0; j < MAP_MAX_SIZE; j++) {
			if (!first_appear_i && mapa_entorno_[i][j] != '?') {
				first_i = i;
				first_appear_i = true;
			}
			if (!first_appear_j && mapa_entorno_[j][i] != '?') {
				first_j = i;
				first_appear_j = true;
			}

		}
	}
	fc_top_left = pair<int, int>(first_i, first_j);

	for (int i = fc_top_left.first; i < map_size + fc_top_left.first; i++) {
		for (int j = fc_top_left.second; j < map_size + fc_top_left.second; j++) {
			mapa_solucion_[i - fc_top_left.first][j - fc_top_left.second] = mapa_entorno_[i][j];
		}
		cout << endl;
	}
	cout << "------------------------------------" << endl;
	printSolutionMap();
	imagineBorders();
	imagineForest();
	printSolutionMap();
}

void Agent::rotateSolution90() {

	char tmp_map[SOLUTION_MAP_SIZE][SOLUTION_MAP_SIZE];
	for (int i = 0; i < SOLUTION_MAP_SIZE; i++) {
		for (int j = 0; j < SOLUTION_MAP_SIZE; j++) {
			tmp_map[i][j] = mapa_solucion_[j][SOLUTION_MAP_SIZE - i - 1];
		}
	}
	for (int i = 0; i < SOLUTION_MAP_SIZE; i++) {
		for (int j = 0; j < SOLUTION_MAP_SIZE; j++) {
			mapa_solucion_[i][j] = tmp_map[i][j];
		}
	}
	//cout << "Rotated" << endl;

}

void Agent::rotateSolution180() {
	rotateSolution90();
	rotateSolution90();
}

void Agent::rotateSolution270() {
	rotateSolution180();
	rotateSolution90();
}

void Agent::rotateSolution(int rotations) {
	for (int i = 0; i < rotations; i++) {
		rotateSolution90();
	}
}

void Agent::imagineBorders() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < CURRENT_MAP_SIZE; j++) {
			mapa_solucion_[i][j] = 'P';
			mapa_solucion_[j][i] = 'P';
			if (i != 3) {
				mapa_solucion_[CURRENT_MAP_SIZE - i - 1][j] = 'P';
				mapa_solucion_[j][CURRENT_MAP_SIZE - i - 1] = 'P';
			}
		}
	}
}

void Agent::imagineForest() {
	for (int i = 0; i < CURRENT_MAP_SIZE; i++) {
		for (int j = 0; j < CURRENT_MAP_SIZE; j++) {
			if (mapa_solucion_[i][j] == '?') {
				mapa_solucion_[i][j] = 'B';
			}
		}
	}
}

void Agent::printSolutionMap() {
	cout << "MAPA SOLUCION:" << endl;
	cout << "------------------------------------" << endl;
	for (int i = 0; i < SOLUTION_MAP_SIZE; i++) {
		for (int j = 0; j < SOLUTION_MAP_SIZE; j++) {
			cout << mapa_solucion_[i][j];
		}
		cout << endl;
	}
	cout << "------------------------------------" << endl;

}


