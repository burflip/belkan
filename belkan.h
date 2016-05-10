
#ifndef AGENT_H_
#define AGENT_H_

#include <string>
#include <iostream>
#include <cstdlib>
#include <deque>
#include <queue>
#include <utility>
#include <map>
#include "conexion.h"
#include "Brain.h"

using namespace std;
#define SOLUTION_MAP_SIZE 100
#define MAP_MAX_SIZE 200
#define CURRENT_MAP_SIZE 30


// -----------------------------------------------------------
//				class Agent
// -----------------------------------------------------------
class Environment;
class Agent
{
public:
	Agent();

	enum ActionType
	{
	    actFORWARD, // avanzar
	    actTURN_L,  // Girar Izquierda
	    actTURN_R,  // Girar Derecha
	    actIDLE,    // No hacer nada
	    actPICKUP,  // Recoger un objeto
	    actPUTDOWN, // Soltar un objeto
	    actPUSH,    // Meter en la mochila
	    actPOP,     // Sacar de la mochila
	    actGIVE,	// Dar un objeto a un personaje
	    actTHROW	// Lanzar un objeto
	};

	void Perceive(Environment &env);
	bool Perceive_Remote(conexion_client &Cliente, Environment &env);
	void ActualizarInformacion(Environment *env);
	ActionType Think();
	void FixLastAction(Agent::ActionType x){last_accion_=x;};

	char mapa_entorno_[200][200]; // mapa que muestra el tipo de terreno
	char mapa_objetos_[200][200]; // mapa que muestra los objetos que estan encima del terreno
	char mapa_solucion_[100][100]; // Mapa que almacena la solucion que el alumno propone
	// Funciones de acceso a los datos
	void GetCoord(int &fila, int &columna, int &brujula){fila=y_;columna=x_;brujula=orientacion_;};
	pair <int,int> getCoord();
	inline int getOrientation() const{
		return orientacion_;
	}
	inline pair<char,char> whatISeeThere(int i){
		return pair<char,char>(VISTA_[i],SURFACE_[i]);
	}
	inline string whatIHear()
	{
		return MENSAJE_;
	}
	void cropAndStoreSolutionMap();
	void rotateSolution90();
	void rotateSolution180();
	void rotateSolution270();
	void rotateSolution(int i);
	void imagineBorders();
	void imagineForest();
	void printSolutionMap();

	inline void isSolved() {
		this->solved = true;
	}

private:
	//Variables de interaccion con el entorno grafico
	int size_;

	//SENSORES
	char VISTA_[10];
	char SURFACE_[10];
	bool REINICIADO_;
	string MENSAJE_;
	char EN_USO_;
	char MOCHILLA_[5];
	char PUNTUACION_[9];
	bool FIN_JUEGO_;
	char SALUD_;

	//Variables de estado
	int x_,y_, orientacion_;
	int last_accion_;
	string role_;
	Brain brain;
	Environment * the_env;
	bool solved;

};

string ActionStr(Agent::ActionType);

#endif
