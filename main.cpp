#include "includes.h"
#include "globales.h"

void leer_archivo_configuracion(char* instancia, int *cant_aviones, int **Sij, vector<Avion> &aviones){
	FILE *archivo;
	int i, j, Ei, Ti, Li;
	float gi, hi;

	archivo = fopen(instancia, "r");

	fscanf(archivo, "%d", cant_aviones);
	if (debug) printf("Cantidad de aviones: %d\n", *cant_aviones);

	if (debug) printf("Iniciando leer_archivo_configuracion ... \n");
	
	if (*cant_aviones < 1){
		printf("Cantidad de objetos invalida");
		fclose(archivo);
		exit(1);
	}

	int *sij = (int *)malloc(sizeof(int)*((*cant_aviones)*(*cant_aviones)));

	for (i = 0; i < *cant_aviones; i++) {
		Avion un_avion;
		fscanf(archivo, "%d %d %d %f %f", &Ei, &Ti, &Li, &gi, &hi);
		un_avion.pos = i;
		un_avion.Ei = Ei;
		un_avion.Ti = Ti;
		un_avion.Li = Li;
		un_avion.gi = gi;
		un_avion.hi = hi;
		for (j = 0; j < *cant_aviones; j++) {
			fscanf(archivo, "%d", &sij[(*cant_aviones)*i + j]);
		}
		aviones.push_back(un_avion);
	}
	fclose(archivo);
	printf("leer_archivo_configuracion done!\n");
	*Sij = sij;
}

int *generar_orden_aleatorio(vector<Avion> aviones, int **lista, int cant_aviones) {
	
}

int leer_entradas(int argc, char **argv){
	//archivo con la instancia del problema
	archivo_configuracion=(char *)(argv[1]);
	
	//archivo donde escribir los resultados de la ejecucion
	archivo_resultados=(char *)(argv[2]);
	
	//SEMILLA
	semilla=atoi(argv[3]);

	debug=atoi(argv[4]);

	return 1;
}

int main(int argc, char *argv[]) {
	int cant_aviones, i;
	int *Sij;
	vector<Avion> aviones;
    if(!leer_entradas(argc,argv)){
        cout<<"Problemas en la lectura de los parametros";
        exit(1);
    }
	leer_archivo_configuracion(archivo_configuracion, &cant_aviones, &Sij, aviones);
	

    return 0;
}
