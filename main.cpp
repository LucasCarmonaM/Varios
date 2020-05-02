#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <random>
#include <omp.h>
#include <time.h>
#include <ctime>

using namespace std;

// Funciones
float avgPuntajes(std::vector<std::string> miVector);
std::vector<std::string> splitLinea(std::string linea);
float redondearAvg(float promedio);

int main(){
    // Se inicializa reloj para poder imprimir tiempo de ejecucion del programa
    clock_t tStart = clock();
    // Se inicializa el rut más pequeño
    unsigned long rut = 14916641;
    // Se inicializa 'linea' para almacenar cada linea leida del fichero
    std::string linea, ruta;
    std::cout << "Ingrese ruta del archivo a procesar: ";
    std::cin >> ruta;
    // Archivo de lectura (Se debe indicar correctamente la ruta de este archivo)
    ifstream fileIn(ruta);
    // Se inicializa 'miVector' para almacenar los ptjes por cada rut
    std::vector<std::string> miVector;
    
    // Imprime un error si la operacion de lectura del fichero falla
    if(fileIn.fail()){
        std::cerr << "No se pudo abrir el archivo" << std::endl;
        exit(1);
    }else{
        // Archivo de escritura
        ofstream fileOut("fichero-salida.csv", ios::trunc);
        // Imprime un error si la operacion de escritura del fichero falla
        if(fileOut.fail()) {
            std::cerr << "Error al crear el archivo: fichero-salida.csv" << std::endl;
            exit(1);
        }else {
            // Mientres no se encuentre al final del fichero,  entra al bucle
        #pragma omp parallel 
        {
            #pragma omp while
            while (!fileIn.eof()) {
                // Guarda la linea del fichero 'fileIn' en 'linea'
                getline(fileIn, linea);
                // Se guardan los puntajes de la linea en el vector
                miVector = splitLinea(linea);
                // Se escriben en el fichero de salida los ruts junto con su promedio respectivo
                #pragma omp critical
                fileOut << rut << ";" << avgPuntajes(miVector) << std::endl;
                // Se le suma 1 a rut
                rut++;
                // Se vacía el vector
                #pragma omp critical
                miVector.clear();
            }
        }
        }
        // Cierre del fichero de escritura 
        fileOut.close();
    }
    // Cierre del fichero de lectura
    fileIn.close(); 
    cout << "tiempo de ejecucion: " << ((double)(clock() - tStart)/CLOCKS_PER_SEC) << endl;
    return 0;
}

// Funcion que retorna un vector con los puntajes de un rut
std::vector<std::string> splitLinea(std::string linea) {
    // Se inicializa vector de tipo string, el cual será el que retornará la función
    std::vector<std::string> resultado;
    // Se inicializa variable tipo string en la cual se almacenarán cada puntaje
    std::string valor;
    // Bucle for que recorrerá la linea leída desde el archivo
    for(int i = 0; i < linea.length(); i++){
        // Se evalúa si la posicion i de linea es igual al caracter ';'
        if(linea[i] == ';' ){
            // Si la condición de cumple, se añade el elemento 'valor' al final del vector 'resultado'
            resultado.push_back(valor);
            // Se vacia el contenido del string 'valor'
            valor.clear();      
        }else{
            // Se concatenan digitos. ejem: "1", "12", "123"
            valor += linea[i];
        }
    }
    resultado.push_back(valor);
    // Retorna el vector con los puntajes
    return resultado;
}
// Funcion que recibe un vector de tipo string y retorna un promedio de tipo float
float avgPuntajes(std::vector<std::string> miVector) {
    // Se inicializan variables para el calculo del promedio
    float sumatoria=0.0, avg=0.0, contadorPuntajes=0.0;
    // Bucle for que recorre el vector que contiene los puntajes
    for(int i = 1; i<miVector.size(); i++){
        // Convierte el puntaje de tipo string a int para ir sumandolos
        sumatoria += stoi(miVector[i]);
        // Cuenta la cantidad de puntajes
        contadorPuntajes+=1.0;
    }
    // Saca el promedio
    avg = sumatoria/contadorPuntajes;
    // Retorna el promedio aproximado con dos decimales
    return redondearAvg(avg);
}
// Funcion que recibe un promedio de tipo float y retorna el mismo, pero aproximado con dos decimales
float redondearAvg(float promedio) {
    // Se multiplica el promedio por 1000 y se castea a int
    int promedioInt = int(promedio * 1000);
    // Se le aplica la operacion de modulo para obtener el ultimo digito
    int ultimaCifra = promedioInt % 10;
    // Se multiplica por 100 para obtener un numero que no contenga la ultima cifra
    int numero = int(promedio * 100);
    // Se evalua si la ultima cifra es >= que 5
    if(ultimaCifra >= 5){
        // Si es mayor, se aproxima
        numero+=1;
    }else{
        // Si no, no se aproxima
        numero+=0;
    }
    // Se retorna el numero aproximado 
    return numero / 100.00;
}
