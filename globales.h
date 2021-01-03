struct Avion {
    int pos;
    int Ei;
    int Ti;
    int Li;
    float gi;
    float hi;
    int cot_inf;
    int cot_sup;
    int instanciado;
    int tpo_actual;
};

struct Solucion {
    int *tiempos;
    float aptitud;
    long double cant_chequeos;
    long double cant_instanciaciones;
    long double cant_retornos;
    double tiempo_sol;
};

//Parámetros de ejecución del programa
char* archivo_configuracion;
char* archivo_resultados;
int semilla;
int debug;

//Variables de medicion
int cant_aviones;
int *orden_instanciacion;
int *Sij;
Solucion mejor_solucion;
Solucion solucion;

clock_t start, end_time;
double time_taken;