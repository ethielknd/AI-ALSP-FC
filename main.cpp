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
		un_avion.cot_inf = Ei;
		un_avion.cot_sup = Li;
		un_avion.instanciado = 0;
		un_avion.tpo_actual = Ei;
		for (j = 0; j < *cant_aviones; j++) {
			fscanf(archivo, "%d", &sij[(*cant_aviones)*i + j]);
		}
		aviones.push_back(un_avion);
	}
	fclose(archivo);
	printf("leer_archivo_configuracion done!\n");
	printf("\n");
	*Sij = sij;
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

void generar_orden_aleatorio(int **lista, int cant_aviones) {
	int i, pos, pos_lista = 0, seleccion;

	int *aux = (int *)malloc(sizeof(int)*cant_aviones); 

	vector<int> inicial;
	for (i = 0; i < cant_aviones; i++) {
		inicial.push_back(i);
	}

	seleccion = cant_aviones;
	while (inicial.size() > 0) {
		pos = rand() % seleccion;
		aux[pos_lista] = pos;
		inicial.erase(inicial.begin() + pos);
		pos_lista++;
		seleccion--;
	}

	*lista = aux;
}

void filtrar_dominios(Avion activo, vector<Avion> &aviones, int *Sij, int cant_aviones) {
	int i;
	for (i = 0; i < cant_aviones; i++) {
		if (aviones.at(i).instanciado != 1) { //Aviones no instanciados
			if (aviones.at(i).cot_inf <= activo.tpo_actual + Sij[activo.pos*i + aviones.at(i).pos]) {
				aviones.at(i).cot_inf = activo.tpo_actual + Sij[activo.pos*i + aviones.at(i).pos] + 1;
			}
		}
	}
}

void generar_solucion(Solucion &solucion, vector<Avion> &aviones, int cant_aviones) {
	int i;
	if (solucion.aptitud_actual < 0) {
		solucion.aptitud_actual = 0;
	}
	for (i = 0; i < cant_aviones; i++) {
		solucion.tpos_actual[i] = aviones.at(i).tpo_actual;
		if (aviones.at(i).tpo_actual < aviones.at(i).Ti) {
			solucion.aptitud_actual += (aviones.at(i).Ti - aviones.at(i).tpo_actual)*aviones.at(i).gi;
		}
		else if (aviones.at(i).tpo_actual > aviones.at(i).Ti) {
			solucion.aptitud_actual += (aviones.at(i).tpo_actual - aviones.at(i).Ti)*aviones.at(i).hi;
		}
		else {
			solucion.aptitud_actual += 0;
		}
	}
	if (solucion.aptitud_mejor < 0) {
		solucion.aptitud_mejor = solucion.aptitud_actual;
		for (i = 0; i < cant_aviones; i++) {
			solucion.tpos_mejor[i] = solucion.tpos_actual[i];
		}
	}
	else {
		if (solucion.aptitud_actual < solucion.aptitud_mejor) {
			solucion.aptitud_mejor = solucion.aptitud_actual;
			for (i = 0; i < cant_aviones; i++) {
				solucion.tpos_mejor[i] = solucion.tpos_actual[i];
			}
		}
	}
	if (debug) {
		printf("Aptitud actual: %f\n", solucion.aptitud_actual);
		printf("Tiempos de aterrizaje actuales:\n");
		for (i = 0; i < cant_aviones; i++) {
			printf("    Avion %d: %d\n", i + 1, solucion.tpos_actual[i]);
		}
		printf("Mejor aptitud: %f\n", solucion.aptitud_mejor);
		printf("Tiempos de mejor aterrizaje:\n");
		for (i = 0; i < cant_aviones; i++) {
			printf("    Avion %d: %d\n", i + 1, solucion.tpos_mejor[i]);
		}
		printf("\n");
	}
}

void forward_checking(vector<Avion> &aviones, int *Sij, int *instancias_aviones, int nivel, int cant_aviones, Solucion &solucion) {
	int i, max_interval;
	if (nivel < cant_aviones) {
		aviones.at(instancias_aviones[nivel]).instanciado = 1; //Instancio un avión
		i = aviones.at(instancias_aviones[nivel]).cot_inf;
		max_interval = aviones.at(instancias_aviones[nivel]).cot_sup;
		for (i; i < max_interval; i++) { //Instancio el tiempo desde el menor en el intervalo e itero en este
			aviones.at(instancias_aviones[nivel]).tpo_actual = i;
			filtrar_dominios(aviones.at(instancias_aviones[nivel]), aviones, Sij, cant_aviones); //Filtro de dominios
			nivel += 1;
			forward_checking(aviones, Sij, instancias_aviones, nivel, cant_aviones, solucion);
		}
	}
	//Si llego acá encontré una solución
	generar_solucion(solucion, aviones, cant_aviones);
}

int main(int argc, char *argv[]) {
    if(!leer_entradas(argc,argv)){
        cout<<"Problemas en la lectura de los parametros";
        exit(1);
    }
	srand(semilla);

	int cant_aviones, i, nivel = 0;
	int *Sij, *orden_instanciacion;
	vector<Avion> aviones;

	leer_archivo_configuracion(archivo_configuracion, &cant_aviones, &Sij, aviones);
	generar_orden_aleatorio(&orden_instanciacion, cant_aviones);
	
	Solucion solucion;
	solucion.tpos_actual = (int *)malloc(sizeof(int)*cant_aviones);
	solucion.aptitud_actual = -1.0;
	solucion.tpos_mejor = (int *)malloc(sizeof(int)*cant_aviones);
	solucion.aptitud_mejor = -1.0;

	forward_checking(aviones, Sij, orden_instanciacion, nivel, cant_aviones, solucion);


    return 0;
}
