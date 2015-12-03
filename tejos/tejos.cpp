//============================================================================
// Name        : final.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
using namespace std;

typedef struct {
	char id[20];
	float x;
	float y;
	float alto;
	float ancho;
}Rectangulo;

typedef struct {
	char id[20];
	float x;
	float y;
	float radio;
	bool controlable;
}Tejo;

typedef struct{
	char rectangulo[20];
	char tejo[20];
	float distancia;
}Choque;

int procesarRectangulos(Rectangulo* rectangulos){
	FILE* archivo;
	archivo = fopen("/home/jorlando/Documentos/repositorios/final/Source/ejemplo.txt","r");
	int index = 0;
	while(!feof(archivo)){
		fscanf(archivo, "%s%f%f%f%f",
				&rectangulos[index].id,
				&rectangulos[index].x,
				&rectangulos[index].y,
				&rectangulos[index].alto,
				&rectangulos[index].ancho);
		index++;
	}
	fclose(archivo);
	return index-1;
}

int procesarTejos(Tejo* tejos){
	FILE* archivo;
	archivo = fopen("/home/jorlando/Documentos/repositorios/final/Source/circulos.txt","r");
	int index = 0;
	while(!feof(archivo)){
		fscanf(archivo, "%s%f%f%f",
				&tejos[index].id,
				&tejos[index].x,
				&tejos[index].y,
				&tejos[index].radio);
		tejos[index].controlable = true;
		index++;
	}
	fclose(archivo);
	return index;
}
int compararRectangulos(const void* a, const void*b){
	return (((Rectangulo*)a)->x - ((Rectangulo*)b)->x);
}

void ordenarRectangulos(Rectangulo* rectangulos, int cantidadRectangulos){
	qsort(rectangulos,cantidadRectangulos, sizeof(Rectangulo),compararRectangulos);
}

bool puntoPerteneceACirculo(int* indiceDesplazado, Tejo* unTejo, int x, int y){
	return (pow((x-(*indiceDesplazado)-unTejo->x),2)+pow((y-unTejo->y),2)<=pow(unTejo->radio,2));
}

bool verificoChoqueEntreTejoYRectangulo(int* indiceDesplazado, Tejo* unTejo, Rectangulo unRectangulo){
	if((unTejo->x+(*indiceDesplazado)- unTejo->radio)<= unRectangulo.x){
		for(int desde= unRectangulo.y; desde<= (unRectangulo.y+unRectangulo.alto); ++desde){
			if(puntoPerteneceACirculo(indiceDesplazado, unTejo, unRectangulo.x, desde)){
				return true;
			}
		}
	}
	return false;
}

void verificoChoque(int* indiceDesplazado, Tejo* unTejo, Rectangulo* rectangulos, int cantidadRectangulos, Choque* choques, int* cantidadChoques){
	for(int indice=0; indice<cantidadRectangulos; ++indice){
		if(verificoChoqueEntreTejoYRectangulo(indiceDesplazado, unTejo, rectangulos[indice])){
			unTejo->controlable = false;
			strcpy(choques[*cantidadChoques].rectangulo, rectangulos[indice].id);
			strcpy(choques[*cantidadChoques].tejo, unTejo->id);
			choques[*cantidadChoques].distancia = (*indiceDesplazado) ;
			(*cantidadChoques)++;
			return;
		}
	}
}

bool estaDetrasDeRectangulos(int* indiceDesplazado, Tejo unTejo, Rectangulo* rectangulos,int cantidadRectangulos){
	for(int indice=0; indice<cantidadRectangulos; ++indice){
		if((unTejo.x+(*indiceDesplazado)-unTejo.radio)<= rectangulos[indice].x){
			return true;
		}
	}
	return false;
}

bool sigoControlandoColisiones(int* indiceDesplazado, Choque* choques, int* cantidadChoques, Tejo* tejos, int cantidadTejos, Rectangulo* rectangulos, int cantidadRectangulos){

	if((*cantidadChoques) == cantidadTejos){
		return false;
	}
	bool algunoControlable = false;
	for(int indTejo=0; indTejo<cantidadTejos; ++indTejo){
		if(tejos[indTejo].controlable){
			if(estaDetrasDeRectangulos(indiceDesplazado, tejos[indTejo], rectangulos, cantidadRectangulos)){
				verificoChoque(indiceDesplazado, &tejos[indTejo], rectangulos, cantidadRectangulos, choques, cantidadChoques);
			}else{
				tejos[indTejo].controlable = false;
			}
			if(tejos[indTejo].controlable){
				algunoControlable = true;
			}
		}
	}
	return algunoControlable;
}

int controlarColisiones(Choque* choques, Tejo* tejos, int cantidadTejos, Rectangulo* rectangulos, int cantidadRectangulos){
	int cantidadChoques = 0;
	int indiceDesplazado = 0;
	bool sigoControlando = true;
	while(sigoControlando){
		sigoControlando = sigoControlandoColisiones(&indiceDesplazado, choques,&cantidadChoques,tejos, cantidadTejos, rectangulos, cantidadRectangulos);
		indiceDesplazado++;
	}
	return cantidadChoques;
}

int comparoChoque(const void* a, const void*b){
	if(((Choque*)a)->distancia>((Choque*)b)->distancia){
		return 1;
	}else if(((Choque*)a)->distancia<((Choque*)b)->distancia){
		return -1;
	}else{
		return 0;
	}
}
void ordenoChoques(Choque* choques, int cantidad){
	qsort(choques,cantidad,sizeof(Choque), comparoChoque);

}

int main() {
	cout << "ARRANCO ..." << endl;
	Rectangulo rectangulos[20];
	Tejo tejos[20];
	Choque choques[20];

	int cantidadRectangulos = procesarRectangulos(rectangulos);
	/*
	 * for(int indice = 0; indice<cantidadRectangulos; ++indice){
	 * 	cout << "RECTANGULO "<< rectangulos[indice].id<<" " << rectangulos[indice].ancho<<endl;
	 * }*/
	int cantidadTejos = procesarTejos(tejos);
	/*for(int indice = 0; indice<cantidadTejos; ++indice){ cout << "TEJO "<< tejos[indice].id<<" " << tejos[indice].radio<<endl; }*/
	ordenarRectangulos(rectangulos, cantidadRectangulos);
	int cantidadChoque = controlarColisiones(choques, tejos, cantidadTejos, rectangulos, cantidadRectangulos);

	ordenoChoques(choques, cantidadChoque);
	cout << "ID tejo - ID Disper - Distancia " <<endl;
	for(int indice = 0; indice<cantidadChoque; ++indice){
			cout << choques[indice].tejo << " "<< choques[indice].rectangulo<< " "<<  choques[indice].distancia<<endl;
		}

	cout << "... FIN" << endl;
	return 0;
}
