//Proyecto Matematicas discretas

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <string>
#include <fstream>
#include <iomanip>
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

// > Archivos
//Guardar una matriz en un archivo de texto: primera linea "m n", luego una fila por linea
void guardarMatriz(const vector<vector<int>>& matriz, const string& nombreArchivo) {
    ofstream archivo(nombreArchivo);
    archivo << matriz.size() << " " << matriz[0].size() << endl;
    for (const auto& fila : matriz) {
        for (size_t j=0; j<fila.size(); j++) {
            archivo << fila[j];
            if (j+1 < fila.size()) archivo << " ";
        }
        archivo << endl;
    }
}

//Leer un entero mayor o igual a un minimo
int leerEntero(const string& mensaje, int minimo) {
    int valor;
    while (true) {
        cout << mensaje;
        if (!(cin >> valor) or valor < minimo) {
            cout << "Entrada invalida. Ingrese un entero mayor o igual a " << minimo << "." << endl;
            cin.clear();
            cin.ignore(1000, '\n');
        } else {
            cin.ignore(1000, '\n');
            return valor;
        }
    }
}

//Imprimir la lista de testores típicos
void imprimirTestoresTipicos(const vector<vector<int>>& testoresTipicos) {
    if (testoresTipicos.empty()) {
        cout << "No se encontraron testores." << endl;
        return;
    }

    for (size_t i=0; i<testoresTipicos.size(); i++) {
        cout << i + 1 << ": { ";
        for (size_t j = 0; j < testoresTipicos[i].size(); j++) {
            cout << "x" << testoresTipicos[i][j] + 1 << " ";
        }
        cout << "}" << endl;
    }
}

// > YYC
//Busqueda del conjunto compatible (Algoritmo 2 de YYC):
//recorre las filas 0..filaActual; cada fila con un unico 1 dentro de S se cuenta
//y marca la columna donde esta ese 1. Condicion 1: filas con suma 1 >= |S|.
//Condicion 2: todas las columnas de S quedan marcadas.
bool FindCompatibleSet(const vector<vector<int>>& MB, const vector<int>& tau, int nuevaColumna, int filaActual) {
    vector<int> S = tau;
    S.push_back(nuevaColumna);

    vector<bool> marcada(S.size(), false);
    int filasConSumaUno = 0;

    for (int k=0; k<=filaActual; k++) {
        int sumaFila = 0;
        int posicionUno = -1;
        for (size_t c=0; c<S.size(); c++) {
            if (MB[k][S[c]] == 1) {
                sumaFila++;
                posicionUno = c;
            }
        }
        if (sumaFila == 1) {
            filasConSumaUno++;
            marcada[posicionUno] = true;
        }
    }

    bool condicion1 = filasConSumaUno >= static_cast<int>(S.size());
    bool condicion2 = true;
    for (bool m : marcada) {
        if (!m) {
            condicion2 = false;
            break;
        }
    }
    return condicion1 and condicion2;
}

//Imprimir el estado despues de procesar una fila
void imprimirFila(int fila, const vector<vector<int>>& psi, double tiempoAcumulado) {
    cout << "\nFila " << fila << ":" << endl;
    cout << "Testores tipicos identificados (" << psi.size() << "):" << endl;
    imprimirTestoresTipicos(psi);
    cout << "> Tiempo acumulado: " << fixed << setprecision(6) << tiempoAcumulado << " ms" << endl;
    cout.unsetf(ios::fixed);
}

//Algoritmo YYC. Despues de cada fila muestra los testores y el tiempo acumulado.
//El cronometro solo mide el procesamiento: se detiene antes de imprimir.
vector<vector<int>> calcularTestoresTipicosYYC(const vector<vector<int>>& MB, double& tiempoTotal) {
    int m = MB.size();
    int n = MB[0].size();

    chrono::duration<double, milli> acumulado(0);
    vector<vector<int>> psi;

    //Fila 1: cada columna con un 1 es un testor tipico
    auto inicio = chrono::high_resolution_clock::now();
    for (int j=0; j<n; j++) {
        if (MB[0][j] == 1) {
            psi.push_back({j});
        }
    }
    auto fin = chrono::high_resolution_clock::now();
    acumulado += fin - inicio;
    imprimirFila(1, psi, acumulado.count());

    //Filas 2..m
    for (int i=1; i<m; i++) {
        inicio = chrono::high_resolution_clock::now();
        vector<vector<int>> psiAux;

        for (const auto& tau : psi) {
            //Caso 1: la fila tiene un 1 en alguna columna de tau -> se conserva
            bool seMantiene = false;
            for (int col : tau) {
                if (MB[i][col] == 1) {
                    seMantiene = true;
                    break;
                }
            }
            if (seMantiene) {
                psiAux.push_back(tau);
            } else {
                //Caso 2: se amplia con cada columna que tenga 1 en la fila,
                //solo si hay un conjunto compatible
                for (int p=0; p<n; p++) {
                    if (MB[i][p] == 1 and FindCompatibleSet(MB, tau, p, i)) {
                        vector<int> nuevo = tau;
                        nuevo.push_back(p);
                        sort(nuevo.begin(), nuevo.end());
                        psiAux.push_back(nuevo);
                    }
                }
            }
        }
        psi = psiAux;

        fin = chrono::high_resolution_clock::now();
        acumulado += fin - inicio;
        imprimirFila(i+1, psi, acumulado.count());
    }

    tiempoTotal = acumulado.count();
    return psi;
}

int main() {
	random_device rd;
	mt19937 gen(rd());

    cout << " >> Obtener los testores tipicos de una matriz con YYC" << endl;

    int opcionMenu = 0;
    do {
        cout << "\nElige una opcion:" << endl;
        cout << "1. Generar matriz A aleatoria (n >= 7, densidad de la basica >= 0.4) y guardarla" << endl;
        cout << "2. Ingresar matriz manualmente fila por fila" << endl;
        cout << "\nOpcion: ";

        if (!(cin >> opcionMenu)) {
            cout << "Entrada no valida." << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            opcionMenu = 0;
        } else {
            cin.ignore(1000, '\n');
        }
    } while (opcionMenu < 1 or opcionMenu > 2);

	vector<vector<int>> matrizIngresada;
	vector<vector<int>> matrizBasica;

    if (opcionMenu == 1) {
        cout << "> Ingresar tamano de la matriz mxn" << endl;
        int filas = leerEntero("m: ", 1);
        int columnas = leerEntero("n (minimo 7): ", 7);

    	cout << "\nGenerando matriz aleatoria " << filas << "x" << columnas << "..." << endl;
        double densidad;
        do {
            matrizIngresada = generarMatrizAleatoria(filas, columnas, gen);
            matrizBasica = calcularMatrizBasica(matrizIngresada);
            densidad = calcularDensidad(matrizBasica);
        } while (densidad < 0.4); //Densidad minima de la basica: 0.4

        string nombreArchivo;
        cout << "Nombre del archivo para guardar la matriz basica A (ej. matrizA.txt): ";
        getline(cin, nombreArchivo);
        guardarMatriz(matrizBasica, nombreArchivo);
        cout << "Matriz basica A guardada en " << nombreArchivo << endl;

    } else {
        cout << "> Ingresar tamano de la matriz mxn" << endl;
        int filas = leerEntero("m: ", 1);
        int columnas = leerEntero("n: ", 1);

		cout << "\nIngrese la matriz " << filas << "x" << columnas << ":" << endl;
		cout << "Escriba cada fila con 0 y 1 (con o sin espacios)." << endl;

		for (int i=0; i<filas; i++) {
			vector<int> fila;
            bool valida;
            do {
                string entrada;
                cout << "Fila " << i+1 << ": ";
                getline(cin, entrada);
                fila.clear();
                valida = true;
                for (char c : entrada) {
                    if (c == '0' or c == '1') fila.push_back(c - '0');
                    else if (c != ' ') valida = false;
                }
                if (static_cast<int>(fila.size()) != columnas) valida = false;
                if (!valida) {
                    cout << "Fila invalida: debe tener exactamente " << columnas << " valores 0 o 1." << endl;
                }
            } while (!valida);

			matrizIngresada.push_back(fila);
        }
        matrizBasica = calcularMatrizBasica(matrizIngresada);
	}

	int columnas = matrizIngresada[0].size();
	cout << "\nMatriz ingresada " << matrizIngresada.size() << "x" << columnas << ":" << endl;
    imprimirMatriz(matrizIngresada);

    //YYC requiere una matriz basica: si la ingresada no lo es, se reduce y se avisa
    if (matrizBasica.size() != matrizIngresada.size()) {
        cout << "\nAviso: la matriz no es basica. Se usa su matriz basica." << endl;
    }
	cout << "\nMatriz basica " << matrizBasica.size() << "x" << columnas << ":" << endl;
    imprimirMatriz(matrizBasica);
    cout << "\nDensidad de la matriz basica: " << calcularDensidad(matrizBasica) << endl;

    cout << "\n >> YYC fila por fila" << endl;
    double tiempoTotal;
	vector<vector<int>> testoresTipicos = calcularTestoresTipicosYYC(matrizBasica, tiempoTotal);

	cout << "\n >> Resultados" << endl;
	cout << "\n> Testores tipicos (" << testoresTipicos.size() << "):" << endl;
	imprimirTestoresTipicos(testoresTipicos);
	cout << "\n> Tiempo total del YYC: " << fixed << setprecision(6) << tiempoTotal << " ms" << endl;

    return 0;
}