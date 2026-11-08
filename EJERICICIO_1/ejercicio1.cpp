//Proyecto Matematicas discretas

#include <iostream>
#include <vector>
#include <random>
#include <ctime>
using namespace std;

//Generar una matriz con elementos aleatorios
vector<vector<int>> generarMatrizAleatoria(int filas, int columnas, mt19937 &gen) {
	uniform_int_distribution<int> distrib(0, 1);
    vector<vector<int>> matriz(filas, vector<int>(columnas));
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            matriz[i][j] = distrib(gen);
        }
    }
    return matriz;
}

//Imprimir una matriz en orden
void imprimirMatriz(const vector<vector<int>>& matriz) {
    int filas = matriz.size();
    int columnas = matriz[0].size();
    for (int i=0; i<filas; i++) {
        for (int j=0; j<columnas; j++) {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }
}

//Apoyo para obtener la básica - revisar si una fila p es subfila de t
bool esSubfila(const vector<int>& p, const vector<int>& t) {
    int n=p.size();
    bool Subfila=false;
    
    for (int j=0; j<n; j++) {
        //condiciones de subfila
        if (p[j] == 1 && t[j] == 0) return false;        
        if (p[j] == 0 && t[j] == 1) Subfila=true;
    }
    
    return Subfila;
}

//Calcular la matriz basica
vector<vector<int>> calcularMatrizBasica(const vector<vector<int>>& MD) {
	int m=MD.size();
    vector<bool> sobrante(m, false);
    
    for (int i=0; i<m; i++) {
    	if (sobrante[i]) continue;
    	
        for (int j=0; j<m; j++) {
            if (i == j or sobrante[j]) continue;
            if (MD[i] == MD[j]) {
            	sobrante[j] = true;
            	continue;
			}
            if (esSubfila(MD[i], MD[j])) {
                sobrante[j] = true;
        	}
        }
    }
    
    vector<vector<int>> MB;
    for (int i=0; i<m; i++) {
        if (!sobrante[i]) MB.push_back(MD[i]);
    }
    return MB;
}

//Calcular la densidad
double calcularDensidad(const vector<vector<int>>& matriz) {
    int filas = matriz.size();
    int columnas = matriz[0].size();
    double totalElementos = filas * columnas;
    int totalUnos = 0;

    for (int i=0; i<filas; i++) {
        for (int j=0; j<columnas; j++) {
            if (matriz[i][j] == 1) {
                totalUnos++;
            }
        }
    }
    if (totalElementos == 0) return 0.0;
    return totalUnos/totalElementos;
}

int main() {
	random_device rd;
	mt19937 gen(rd());


    int filasMD;
    int columnasMD;
    
    cout << "> Generar matriz mxn" << endl;
    cout << "m: ";
    cin >> filasMD;
    cout << "n: ";
    cin >> columnasMD;

    cout << "Generando matriz aleatoria " << filasMD << "x" << columnasMD << "..." << endl;
    vector<vector<int>> matrizDiferencias = generarMatrizAleatoria(filasMD, columnasMD, gen);
    
    cout << "\nMatriz de diferencias " << filasMD << "x" << columnasMD << ":" << endl;
    imprimirMatriz(matrizDiferencias);

	vector<vector<int>> matrizBasica;
	cout << "\nCalculando matriz basica..." << endl;
	matrizBasica = calcularMatrizBasica(matrizDiferencias);
	
	int filasMB=matrizBasica.size();
	
	cout << "\nMatriz basica " << filasMB << "x" << columnasMD << ":" << endl;
    imprimirMatriz(matrizBasica);
	
    double densidadMD = calcularDensidad(matrizBasica);
    cout << "\nDensidad de la matriz: " << densidadMD << endl;

    return 0;
}