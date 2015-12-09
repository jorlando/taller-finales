//============================================================================
// Name        : finalRectangulos.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <list>
#include <stdio.h>
#include <math.h>

//using namespace std;
typedef struct{
	char id[20];
	float xInf;
	float yInf;
	float xSup;
	float ySup;
}Rectangulo;

typedef struct{
	Rectangulo* rectangulo1;
	Rectangulo* rectangulo2;
	Rectangulo* rectangulo3;
	float area;
	float distancia;
}Choque;

bool comparoChoque(const Choque* uno,const Choque* dos){
	return uno->distancia > dos->distancia;
}
void ordernarChoques(std::list<Choque*>* choques){
	choques->sort(comparoChoque);
}

float calcularDistancia(std::pair<float,float> inferior){
	return sqrt(pow(inferior.first,2) + pow(inferior.second,2));
}

float calcularArea(std::pair<float,float> inferior, std::pair<float,float> superior){
	return ((superior.first-inferior.first) * (superior.second-inferior.second));
}
std::pair<float, float> obtenerVerticeInferior(Choque* unChoque){
	std::pair <float, float> vertice;
	float miX = std::max(unChoque->rectangulo1->xInf, unChoque->rectangulo2->xInf);
	miX = std::max(miX, unChoque->rectangulo3->xInf);
	vertice.first = miX;
	float miY = std::max(unChoque->rectangulo1->yInf, unChoque->rectangulo2->yInf);
	miY = std::max(miY, unChoque->rectangulo3->yInf);
	vertice.second = miY;
	return vertice;
}

std::pair<float, float> obtenerVerticeSuperior(Choque* unChoque){
	std::pair <float, float> vertice;
	float miX = std::min(unChoque->rectangulo1->xSup, unChoque->rectangulo2->xSup);
	miX = std::min(miX, unChoque->rectangulo3->xSup);
	vertice.first = miX;
	float miY = std::min(unChoque->rectangulo1->ySup, unChoque->rectangulo2->ySup);
	miY = std::min(miY, unChoque->rectangulo3->ySup);
	vertice.second = miY;
	return vertice;
}

void calcularDatos(Choque* unChoque){
	std::pair<float,float> inferior = obtenerVerticeInferior(unChoque);
	std::pair<float,float> superior = obtenerVerticeSuperior(unChoque);
	unChoque->area = calcularArea(inferior, superior);
	unChoque->distancia = calcularDistancia(inferior);
}

bool chocanEstosDos(Rectangulo* primerRec, Rectangulo* segundoRec){
	if(primerRec->xInf>= segundoRec->xInf && primerRec->xInf <= segundoRec->xSup){
		if(primerRec->yInf >= segundoRec->yInf && primerRec->yInf <= segundoRec->ySup){
			return true;
		}
	}else if(segundoRec->xInf>= primerRec->xInf && segundoRec->xInf <= primerRec->xSup){
		if(segundoRec->yInf>= primerRec->yInf && segundoRec->yInf <= primerRec->ySup){
			return true;
		}
	}
	return false;
}
void choqueConAlguno(Rectangulo* unRectangulo,std::list<Rectangulo*> rectangulos, std::list<Choque*>* choques){
	Choque* unChoque = new Choque();
	unChoque->area = 0;
	unChoque->distancia = 0;
	unChoque->rectangulo1 = unRectangulo;
	unChoque->rectangulo2 = NULL;
	unChoque->rectangulo3 = NULL;
	bool yaAvance = false;
	for (std::list<Rectangulo*>::iterator itRect=rectangulos.begin(); itRect!= rectangulos.end(); ++itRect){
		if((*itRect)->id == unRectangulo->id){
			yaAvance = true;
		}else{
			if(yaAvance){
				if(chocanEstosDos(unRectangulo, (*itRect))){
					if(!unChoque->rectangulo2){
						unChoque->rectangulo2 = (*itRect);
					}else{
						if(chocanEstosDos(unChoque->rectangulo2, (*itRect))){
							unChoque->rectangulo3 = (*itRect);
							calcularDatos(unChoque);
							choques->push_back(unChoque);
							return;
						}
					}
				}
			}
		}
	}

}

std::list<Choque*> calculoChoques(std::list<Rectangulo*> rectangulos){
	std::list<Choque*> choques;
	for(std::list<Rectangulo*>::iterator indiceRect = rectangulos.begin(); indiceRect != rectangulos.end(); ++indiceRect){
		choqueConAlguno((*indiceRect), rectangulos, &choques);
	}
	return choques;
}

std::list<Rectangulo*> cargarRectangulos(){
	std::list<Rectangulo*> rectangulos ;
	FILE* archivo;
	archivo = fopen("/home/jorlando/Documentos/repositorios/finalRectangulos/Source/rectangulos.txt", "r");
	while(!feof(archivo)){
		Rectangulo* nuevoRec = new Rectangulo();
		fscanf(archivo, "%s%f%f%f%f", &nuevoRec->id, &nuevoRec->xInf, &nuevoRec->yInf, &nuevoRec->xSup, &nuevoRec->ySup);
		rectangulos.push_back(nuevoRec);
	}
	fclose(archivo);
	return rectangulos;
}

int main() {
	std::list<Rectangulo*> rectangulos = cargarRectangulos();
	std::list<Choque*> choques = calculoChoques(rectangulos);
	ordernarChoques(&choques);
	std::cout << "Distancia - Area - IdRec 1 - IdRec2 - IdRec3"<<std::endl;
	for(std::list<Choque*>::iterator itChq = choques.begin(); itChq != choques.end(); ++itChq){
		std::cout << (*itChq)->distancia << " " << (*itChq)->area << " " << (*itChq)->rectangulo1->id << " " << (*itChq)->rectangulo2->id << " " << (*itChq)->rectangulo3->id<<std::endl;
	}
	std::cout << "FIN"<<std::endl;


	return 0;
}
