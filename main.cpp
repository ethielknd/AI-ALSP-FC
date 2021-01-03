#include "includes.h"
#include "globales.h"

void leer_archivo_configuracion(char* instancia, vector<Avion> &aviones){
	FILE *archivo;
	int i, j, Ei, Ti, Li;
	float gi, hi;

	archivo = fopen(instancia, "r");

	fscanf(archivo, "%d", &cant_aviones);
	if (debug) printf("Cantidad de aviones: %d\n", cant_aviones);

	if (debug) printf("Iniciando leer_archivo_configuracion ... \n");
	
	if (cant_aviones < 1){
		printf("Cantidad de objetos invalida");
		fclose(archivo);
		exit(1);
	}

	Sij = (int *)malloc(sizeof(int)*(cant_aviones*cant_aviones));

	for (i = 0; i < cant_aviones; i++) {
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
		for (j = 0; j < cant_aviones; j++) {
			fscanf(archivo, "%d", &Sij[cant_aviones*i + j]);
		}
		aviones.push_back(un_avion);
	}
	if (debug) {
		printf("leer_archivo_configuracion done!\n");
		printf("\n");
	}
	fclose(archivo);
}

void escribir_archivo_out() {
	int i;
	ofstream file;
	file.open(archivo_resultados);
	file << "La mejor solución que se pudo encontrar hasta ahora es:\n";
	file << endl;
	file << "    ";
	for (i = 0; i < cant_aviones; i++) {
		file << mejor_solucion.tiempos[i] << " ";
		//file << "Avion " << i << ": " << mejor_solucion.tiempos[i] << endl;
	}
	file << endl;
	file << "\nCon la siguiente información:" << endl;
	file << "    Costo: " << mejor_solucion.aptitud << endl;
	file << "    Tiempo de ejecucion: " << mejor_solucion.tiempo_sol << endl;
	file << "    Cantidad de chequeos: " << mejor_solucion.cant_chequeos << endl;
	file << "    Cantidad de instanciaciones: " << mejor_solucion.cant_instanciaciones << endl;
	file << "    Cantidad de retornos: " << mejor_solucion.cant_retornos << endl;
	file << "\nAdemás, los aviones que se alcanzaron a instanciar para la solucion actual son:\n";
	for (i = 0; i < cant_aviones; i++) {
		if (solucion.tiempos[i] != -1) {
			file << "    Avion " << i << ": " << solucion.tiempos[i] << endl;
		}
	}
	file << "Y un tiempo total de ejecucion del programa de " << time_taken << " segundos" << endl;
	file.close();
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

void generar_orden_aleatorio() {
	int i, pos, pos_lista = 0, seleccion;

	orden_instanciacion = (int *)malloc(sizeof(int)*cant_aviones); 

	vector<int> inicial;
	for (i = 0; i < cant_aviones; i++) {
		inicial.push_back(i);
	}

	seleccion = cant_aviones;
	while (inicial.size() > 0) {
		pos = rand() % seleccion;
		orden_instanciacion[pos_lista] = inicial[pos];
		inicial.erase(inicial.begin() + pos);
		pos_lista++;
		seleccion--;
	}
}

void filtrar_dominios(Avion activo, vector<Avion> &aviones) {
	int i;
	for (i = 0; i < cant_aviones; i++) {
		if (aviones.at(i).instanciado != 1) { //Aviones no instanciados
			if (aviones.at(i).cot_inf <= activo.tpo_actual + Sij[activo.pos*cant_aviones + i]) {
				aviones.at(i).cot_inf = activo.tpo_actual + Sij[activo.pos*cant_aviones + i] + 1;
				if (debug) printf("El nuevo intervalo del Avion %d es: [%d, %d]\n", aviones.at(i).pos, aviones.at(i).cot_inf, aviones.at(i).cot_sup);
			}
			else {
				if (debug) printf("El intervalo del Avion %d no cambió y es: [%d, %d]\n", aviones.at(i).pos, aviones.at(i).cot_inf, aviones.at(i).cot_sup);
			}
		}
	}
}

void generar_solucion(vector<Avion> &aviones) {
	int i, max = 0;
	solucion.aptitud = 0.0;
	for (i = 0; i < cant_aviones; i++) {
		solucion.tiempos[i] = aviones.at(i).tpo_actual;
		if (aviones.at(i).tpo_actual < aviones.at(i).Ti) {
			solucion.aptitud += (aviones.at(i).Ti - aviones.at(i).tpo_actual)*aviones.at(i).gi;
		}
		else if (aviones.at(i).tpo_actual > aviones.at(i).Ti) {
			solucion.aptitud += (aviones.at(i).tpo_actual - aviones.at(i).Ti)*aviones.at(i).hi;
		}
		else {
		}
	}
	if (mejor_solucion.aptitud < 0) {
		end_time = clock();
		mejor_solucion.aptitud = solucion.aptitud;
		mejor_solucion.aptitud = solucion.aptitud;
		mejor_solucion.cant_chequeos = solucion.cant_chequeos;
		mejor_solucion.cant_instanciaciones = solucion.cant_instanciaciones;
		mejor_solucion.cant_retornos = solucion.cant_retornos;
		for (i = 0; i < cant_aviones; i++) {
			mejor_solucion.tiempos[i] = solucion.tiempos[i];
		}
		time_taken = double(end_time - start) / double(CLOCKS_PER_SEC);
		mejor_solucion.tiempo_sol = time_taken;
	}
	else {
		if (solucion.aptitud < mejor_solucion.aptitud) {
			end_time = clock();
			mejor_solucion.aptitud = solucion.aptitud;
			mejor_solucion.cant_chequeos = solucion.cant_chequeos;
			mejor_solucion.cant_instanciaciones = solucion.cant_instanciaciones;
			mejor_solucion.cant_retornos = solucion.cant_retornos;
			for (i = 0; i < cant_aviones; i++) {
				mejor_solucion.tiempos[i] = solucion.tiempos[i];
			}
			time_taken = double(end_time - start) / double(CLOCKS_PER_SEC);
			mejor_solucion.tiempo_sol = time_taken;
		}
	}
	if (debug) {
		printf("\nAptitud actual: %f\n", solucion.aptitud);
		printf("Tiempos de aterrizaje actuales:\n");
		for (i = 0; i < cant_aviones; i++) {
			printf("    Avion %d: %d\n", i, solucion.tiempos[i]);
		}
		printf("    Cantidad de chequeos realizados: %Lf\n", solucion.cant_chequeos);
		printf("    Cantidad de instanciaciones realizadas: %Lf\n", solucion.cant_instanciaciones);
		printf("    Cantidad de retornos realizados: %Lf\n", solucion.cant_retornos);
		printf("\nMejor aptitud actual: %f\n", mejor_solucion.aptitud);
		printf("Tiempos de mejor aterrizaje actual:\n");
		for (i = 0; i < cant_aviones; i++) {
			printf("    Avion %d: %d\n", i, mejor_solucion.tiempos[i]);
		}
		printf("    Cantidad de chequeos realizados en la mejor solucion: %Lf\n", mejor_solucion.cant_chequeos);
		printf("    Cantidad de instanciaciones realizadas en la mejor solucion: %Lf\n", mejor_solucion.cant_instanciaciones);
		printf("    Cantidad de retornos realizados en la mejor solucion: %Lf\n", mejor_solucion.cant_retornos);
	}
}

void forward_checking(vector<Avion> &aviones, int nivel) {
	int i, max_interval;
	if (nivel < cant_aviones) { //Quedan aviones por instanciar
		solucion.cant_chequeos += 1; //Chequeo: Aun quedan aviones
		aviones.at(orden_instanciacion[nivel]).instanciado = 1; //Instancio un avión para determinar un valor en su dominio y que dentro del backtracking no se trate de filtrar su dominio
		i = aviones.at(orden_instanciacion[nivel]).cot_inf;
		max_interval = aviones.at(orden_instanciacion[nivel]).cot_sup;
		if (max_interval - i >= 0) {
			solucion.cant_chequeos += 1; //Chequeo: Aun queda dominio valido (Si es negativo el valor del if, entonces el dominio no existe o ya no queda alguno)
			for (i; i <= max_interval; i++) { //Instancio el tiempo desde el menor en el intervalo e itero en este
				aviones.at(orden_instanciacion[nivel]).tpo_actual = i;
				solucion.cant_instanciaciones += 1;
				if (debug) printf("\nSe ha instanciado el avion %d ", orden_instanciacion[nivel]);
				if (debug) printf("con el tpo_actual = %d ", aviones.at(orden_instanciacion[nivel]).tpo_actual);
				if (debug) printf("e intervalo [%d, %d]\n", i, max_interval);
				filtrar_dominios(aviones.at(orden_instanciacion[nivel]), aviones); //Filtro de dominios
				forward_checking(aviones, nivel + 1);
			}
		}
		if (debug) printf("\n\nNo queda dominio por instanciar para el avion %d\n\n", orden_instanciacion[nivel]);
		solucion.tiempos[orden_instanciacion[nivel]] = -1;
		aviones.at(orden_instanciacion[nivel]).instanciado = 0;
		aviones.at(orden_instanciacion[nivel]).cot_inf = aviones.at(orden_instanciacion[nivel]).Ei;
		aviones.at(orden_instanciacion[nivel]).cot_sup = aviones.at(orden_instanciacion[nivel]).Li;
	}
	else { //Todos los aviones se instanciaron
		generar_solucion(aviones);
	}
	solucion.cant_retornos += 1;
	//if (debug) exit(1);
}

void interrupcion(){
	end_time = clock();
	int i;
	printf("\n\n\nInformación de la mejor solución que se pudo encontrar:\n");
	for (i = 0; i < cant_aviones; i++) {
		printf("    Avion %d: %d\n", i, mejor_solucion.tiempos[i]);
	}
	printf("    Costo de: %f\n", mejor_solucion.aptitud);
	printf("    Cantidad de instancias: %Lf\n", mejor_solucion.cant_instanciaciones);
	printf("    Cantidad de chequeos: %Lf\n", mejor_solucion.cant_chequeos);
	printf("    Cantidad de retornos: %Lf\n", mejor_solucion.cant_retornos);
	printf("    Tiempo de ejecución usado: %f\n", mejor_solucion.tiempo_sol);
	printf("\nAdemás, los aviones que se alcanzaron a instanciar para la solucion actual son:\n");
	for (i = 0; i < cant_aviones; i++) {
		if (solucion.tiempos[i] != -1) printf("Avion %d: %d\n", i, solucion.tiempos[i]);
	}
	time_taken = double(end_time - start) / double(CLOCKS_PER_SEC);
	cout << "\nEl tiempo de ejecucion ha sido de: " << fixed  
         << time_taken;
    cout << " segundos " << endl; 
	escribir_archivo_out();
	free(mejor_solucion.tiempos);
	free(solucion.tiempos);
	free(orden_instanciacion);
	free(Sij);
}

void my_handler(int s){
	printf("\nCaught signal %d\n",s);
	interrupcion();
	exit(1);
}

int main(int argc, char *argv[]) {

	//Manejo de excepcion ctrl+c
	struct sigaction sigIntHandler;
	sigIntHandler.sa_handler = my_handler;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;
	sigaction(SIGINT, &sigIntHandler, NULL);

	//Codigo
    if(!leer_entradas(argc,argv)){
        cout<<"Problemas en la lectura de los parametros";
        exit(1);
    }
	srand(semilla);

	int i, j, nivel = 0;
	vector<Avion> aviones;

	leer_archivo_configuracion(archivo_configuracion, aviones);

	mejor_solucion.tiempos = (int *)malloc(sizeof(int)*cant_aviones);
	for (i = 0; i < cant_aviones; i++) {
		mejor_solucion.tiempos[i] = -1;
	}
	mejor_solucion.aptitud = -1.0;
	mejor_solucion.cant_chequeos = 0;
	mejor_solucion.cant_instanciaciones = 0;
	mejor_solucion.cant_retornos = 0;

	if (!debug) {
		printf("\nMatriz Sij:\n");
		for (i = 0; i < cant_aviones; i++) {
			for (j = 0; j < cant_aviones; j++) {
				printf("%d ", Sij[cant_aviones*i + j]);
			}
			printf("\n");
		}
	}
	generar_orden_aleatorio();
	printf("\nOrden de instanciacion aleatorio generado: ");

	for (i = 0; i < cant_aviones; i++) {
		printf("%d ", orden_instanciacion[i]);
	}
	printf("\n");
	
	solucion.tiempos = (int *)malloc(sizeof(int)*cant_aviones);
	for (i = 0; i < cant_aviones; i++) {
		solucion.tiempos[i] = -1;
	}
	solucion.aptitud = -0.0;
	solucion.cant_chequeos = 0;
	solucion.cant_instanciaciones = 0;
	solucion.cant_retornos = 0;

	start = clock();
	forward_checking(aviones, nivel);
	end_time = clock();
	time_taken = double(end_time - start) / double(CLOCKS_PER_SEC);
	int aux_inst = 0;
	for (i = 0; i < cant_aviones; i++) {
		if (solucion.tiempos[i] != -1) {
			aux_inst += 1;
		}
	}

	if (aux_inst == cant_aviones) {
		printf("El óptimo global es el siguiente:\n");
		printf("	Tiempos de aterrizaje: \n");
		for (i = 0; i < cant_aviones; i++) {
			printf("		Avion %d: %d\n", i, mejor_solucion.tiempos[i]);
		}
		printf("	Costo final: %f\n", mejor_solucion.aptitud);
		printf("Tiempo de ejecución de programa: %lf\n", time_taken);
	}
	else {
		printf("El problema no tiene solucion\n");
	}
	free(mejor_solucion.tiempos);
	free(solucion.tiempos);
	free(orden_instanciacion);
	free(Sij);
    return 0;
}
