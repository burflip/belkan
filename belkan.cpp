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
void PasarVectoraMapaCaracteres(int fila, int columna, char m[200][200],
		char *v, int brujula) {
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
	srand (time(NULL));
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

}
void Agent::Perceive(Environment &env) {
	env.SenSorStatus(VISTA_, SURFACE_, MENSAJE_, REINICIADO_, EN_USO_,
			MOCHILLA_, PUNTUACION_, FIN_JUEGO_, SALUD_, false);

}

bool Agent::Perceive_Remote(conexion_client &Cliente, Environment &env) {
	bool actualizado = false;

	actualizado = env.Perceive_Remote(Cliente);
	if (actualizado)
		env.SenSorStatus(VISTA_, SURFACE_, MENSAJE_, REINICIADO_, EN_USO_,
				MOCHILLA_, PUNTUACION_, FIN_JUEGO_, SALUD_, true);

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
		cout
				<< "CUIDADO: NO ESTAS CONTROLANDO BIEN LA UBICACION DE TU AGENTE\n";
	}

	PasarVectoraMapaCaracteres(y_, x_, mapa_entorno_, VISTA_, orientacion_);
	PasarVectoraMapaCaracteres(y_, x_, mapa_objetos_, SURFACE_, orientacion_);
	env->ActualizarMatrizUsuario(mapa_entorno_);

}

// -----------------------------------------------------------
Agent::ActionType Agent::Think() {
	Agent::ActionType accion;
	accion = (Agent::ActionType) brain.Think(*this);
	/*if(!(VISTA_[1] == 'S' || VISTA_[1] == 'T' || VISTA_[1] == 'K') || (SURFACE_[1] >= 'a' && SURFACE_[1] <= 'z')) {
		int r = rand() % 100;
		if(r > 60) {
			accion = Agent::ActionType::actTURN_L;
		} else {
			accion = Agent::ActionType::actTURN_R;
		}
	} else {
		accion = Agent::ActionType::actFORWARD;
	}*/
	last_accion_ = accion;
	return accion;
}

pair<int, int> Agent::getCoord() {
	return pair<int,int>(x_,y_);
}
