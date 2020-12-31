struct Avion {
    int pos;
    int Ei;
    int Ti;
    int Li;
    float gi;
    float hi;
};

int leer_entradas(int,char **);
int int_rand (int,int);

//Parámetros de ejecución del programa
char* archivo_configuracion;
char* archivo_resultados;
int semilla;
int debug;