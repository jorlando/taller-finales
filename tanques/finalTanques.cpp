//============================================================================
// Name        : finalTanques.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <list>
#include <math.h>
#include <string.h>
#include <stdlib.h>
using namespace std;

typedef struct{
	char id[20];
	float xInfIzq;
	float yInfIzq;
	float xSupDer;
	float ySupDer;
	bool controlable;
}Tanque;

typedef struct{
	char id[20];
	float x;
	float y;
	float radio;
}Pozo;

typedef struct{
	char tanqueId[20];
	char pozoId[20];
	float distancia;
}Choque;

bool comparoChoque(const Choque* a, const Choque*b){
	return ( a->distancia < b->distancia);
}

void ordenoChoque(list<Choque*>* losChoque){
	losChoque->sort(comparoChoque);
	//qsort(losChoque,losChoque->size(), sizeof(Choque), comparoChoque);
}

bool sePasoDeTodos(Tanque* unTanque,  list<Pozo*>* losPozos, float* distancia){
	for(list<Pozo*>::iterator itPozos = losPozos->begin(); itPozos != losPozos->end(); ++itPozos){
		if((unTanque->xSupDer+(*distancia)) <= ((*itPozos)->x+(*itPozos)->radio)){

			return false;
		}
		//cout << " " << unTanque->xSupDer << " " << (*distancia) << " "<< (*itPozos)->x << " " << (*itPozos)->radio<< endl;
	}
	return true;
}
bool puntoContenidoEnCirculo(float x, float y, Pozo* unPozo){
	return ((pow(x-unPozo->x,2)+pow(y-unPozo->y,2))<=pow(unPozo->radio,2));
}

bool hayChoque(Tanque* unTanque, Pozo* unPozo, float* distancia){
	bool choco = false;
	if(puntoContenidoEnCirculo(unTanque->xInfIzq+(*distancia), unTanque->yInfIzq, unPozo)&&
			puntoContenidoEnCirculo(unTanque->xInfIzq+(*distancia), unTanque->ySupDer, unPozo)&&
			puntoContenidoEnCirculo(unTanque->xSupDer+(*distancia), unTanque->ySupDer, unPozo)&&
			puntoContenidoEnCirculo(unTanque->xSupDer+(*distancia), unTanque->yInfIzq, unPozo)){
		choco = true;
	}
	return choco;
}

bool sigueSiendoControlable(float* distancia, Tanque* unTanque, list<Pozo*>* losPozos, list<Choque*>* losChoques){
	bool estaPendiente = true;
	for(list<Pozo*>::iterator itPozos = losPozos->begin(); itPozos != losPozos->end(); ++itPozos){
		bool choco = hayChoque(unTanque, (*itPozos), distancia);
		if(choco){
			//cout << "CHOQUE"<< endl;
			unTanque->controlable = false;
			Choque* elChoque = new Choque();
			strcpy(elChoque->tanqueId,unTanque->id);
			strcpy(elChoque->pozoId,(*itPozos)->id);
			elChoque->distancia = (*distancia);
			losChoques->push_front(elChoque);
			estaPendiente = false;
		}
	}
	if(estaPendiente  && sePasoDeTodos(unTanque, losPozos, distancia)){
		//cout << "ME PASE"<< endl;
		estaPendiente = false;
		unTanque->controlable = false;
	}
	return estaPendiente;
}

bool procesoLosTanques(float* distancia, list<Tanque*>* losTanques, list<Pozo*>* losPozos, list<Choque*>* losChoques){
	bool yaTermine = true;
	if(losTanques->size() == losChoques->size()){
		cout << "son iguales"<<endl;
		return true;
	}
	for(list<Tanque*>::iterator itTanque = losTanques->begin(); itTanque != losTanques->end(); ++itTanque){
		if((*itTanque)->controlable){
			(*itTanque)->controlable = sigueSiendoControlable(distancia, (*itTanque), losPozos, losChoques);
			if((*itTanque)->controlable){
				//cout << "modifique"<<endl;
				yaTermine = false;
			}
		}
	}

	return yaTermine;
}

void calcularCaidos(list<Tanque*>* losTanques, list<Pozo*>* losPozos, list<Choque*>* losChoques){
	float distancia = 0;
	bool fin = false;
	while(!fin){
		cout << "CICLO: "<< distancia<<endl;
		fin = procesoLosTanques(&distancia, losTanques, losPozos, losChoques);
		distancia += 0.1;
	}
}

void cargarTanques(list<Tanque*>* losTanques){
	FILE* archivo;
	archivo = fopen("/home/jorlando/Documentos/repositorios/finalTanques/tanques.txt","r");
	while(! feof(archivo)){
		Tanque* miTanque =  new Tanque();
		fscanf(archivo,"%s%f%f%f%f",&miTanque->id, &miTanque->xInfIzq, &miTanque->yInfIzq, &miTanque->xSupDer, &miTanque->ySupDer);
		miTanque->controlable = true;
		losTanques->push_back(miTanque);
	}
	fclose(archivo);
}

void cargarPozos(list<Pozo*>* losPozos){
	FILE* archivo;
	archivo = fopen("/home/jorlando/Documentos/repositorios/finalTanques/pozos.txt","r");
	while(! feof(archivo)){
		Pozo* unPozo = new Pozo();
		fscanf(archivo, "%s %f%f%f", &unPozo->id, &unPozo->x, &unPozo->y, &unPozo->radio);
		losPozos->push_back(unPozo);
	}
	fclose(archivo);
}

int main() {
	list<Tanque*> misTanques;
	list<Pozo*> misPozos;
	list<Choque*> losChoques;
	cargarTanques(&misTanques);
	cargarPozos(&misPozos);
	cout << "tanques: "<< misTanques.size()<<endl;
	cout << "pozos: " << misPozos.size()<<endl;
	calcularCaidos(&misTanques, &misPozos, &losChoques);

	cout << "choques: "<< losChoques.size()<<endl;
	ordenoChoque(&losChoques);
	for(list<Choque*>::iterator itChoque=losChoques.begin(); itChoque != losChoques.end(); ++itChoque){
		cout << (*itChoque)->tanqueId << " " << (*itChoque)->pozoId << " " << (*itChoque)->distancia<<endl;
	}
	return 0;
}
