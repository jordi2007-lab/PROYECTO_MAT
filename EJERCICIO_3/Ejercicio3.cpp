//Proyecto Matematicas discretas

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <string>
#include <cmath>
using namespace std;

//Parámetros
const double DensidadMinima = 0.4;
const int MaxIntentos = 1000;
const int TestoresPorLinea = 8;

//Generar una matriz con elementos aleatorios
vector<vector<int>> generarMatrizAleatoria(int filas, int columnas, mt19937 &gen) {
    uniform_int_distribution<int> distrib(0, 1);
    vector<vector<int>> matriz(filas, vector<int>(columnas));
    for (int i=0; i<filas; i++) {
        for (int j=0; j<columnas; j++) {
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
    int n = p.size();
    bool Subfila = false;

    for (int j=0; j<n; j++) {
        //condiciones de subfila
        if (p[j] == 1 and t[j] == 0) return false;
        if (p[j] == 0 and t[j] == 1) Subfila=true;
    }
    return Subfila;
}

//Calcular la matriz basica
vector<vector<int>> calcularMatrizBasica(const vector<vector<int>>& MD) {
    int m = MD.size();
    vector<bool> sobrante(m, false);

    for (int i=0; i<m; i++) {
        if (sobrante[i]) continue;

        for (int j=0; j<m; j++) {
            if (i == j or sobrante[j]) continue;
            //Filas repetidas o filas que contienen a otra se eliminan
            if (MD[i] == MD[j] or esSubfila(MD[i], MD[j])) {
                sobrante[j] = true;
                continue;
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
            if (matriz[i][j] == 1) totalUnos++;
        }
    }
    return totalUnos/totalElementos;
}

//Imprimir la lista de testores típicos
void imprimirTestores(const vector<vector<int>>& testoresTipicos) {
    if (testoresTipicos.empty()) {
        cout << "No se encontraron testores." << endl;
        return;
    }

    for (size_t i=0; i<testoresTipicos.size(); i++) {
        cout << i+1 << ": { ";
        for (size_t j=0; j<testoresTipicos[i].size(); j++) {
            cout << testoresTipicos[i][j] + 1 << " ";
        }
        cout << "}" << endl;
    }
}

//Leer un entero positivo
int leerEnteroPositivo(const string& nombre) {
    int valor;
    while (true) {
        cout << nombre << ": ";
        if (cin >> valor and valor > 0) {
            cin.ignore(1000, '\n');
            return valor;
        }
        cout << "Entrada no valida. Ingrese un numero positivo." << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

//Leer una opcion de menu
int leerOpcion(int minima, int maxima) {
    int opcion;
    while (true){
        cout << "\nOpcion: ";
        if (cin >> opcion and opcion >= minima and opcion <= maxima) {
            cin.ignore(1000, '\n');
            return opcion;
        }
        cout << "Entrada no valida." << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

//Leer una fila insertada manualmente
vector<int> leerFilaManual(int n, int nFila) {
    string entrada;
    vector<int> fila;

    cout << "Fila " << nFila << ": ";
    getline(cin, entrada);

    for (int j=0; j<n; j++) {
        if (j < static_cast<int>(entrada.size()) and entrada[j] == '0') fila.push_back(0);
        else fila.push_back(1);
    }
    return fila;
}

// > YYC
// ================================================

//Mostrar los resultados de cada fila
void registrarFila(int fila, const vector<vector<int>>& testoresTipicos, double tiempoAcumulado, vector<vector<vector<int>>>& historial, vector<double>& tiempos, bool mostrar) {
    historial.push_back(testoresTipicos);
    tiempos.push_back(tiempoAcumulado);

    if (!mostrar) return;

    cout << "\nFila " << fila << ":" << endl;
    cout << "Testores encontrados:" << endl;
    imprimirTestores(testoresTipicos);
    cout << "\n> Tiempo de ejecucion acumulado: " << fixed << setprecision(6) << tiempoAcumulado << " ms" << endl;
}

//Busqueda del conjunto compatible (Algoritmo 2 de YYC), igual que en Ejercicio2.cpp:
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

//Obtener el conjunto de testores típicos de una matriz basica con el algoritmo YYC
//El cronometro solo mide el procesamiento de cada fila: se detiene antes de imprimir.
vector<vector<int>> calcularTestoresTipicosYYC(const vector<vector<int>>& MB, vector<vector<vector<int>>>& historial, vector<double>& tiempos) {
    int m = MB.size();
    int n = MB[0].size();

    vector<vector<int>> testoresTipicos;
    double tiempoAcumulado = 0;

    //Columnas con 1 en la primera fila son testores
    auto inicioCronometro = chrono::high_resolution_clock::now();

    for (int j=0; j<n; j++) {
        if (MB[0][j] == 1) {
            testoresTipicos.push_back({j});
        }
    }
    auto finCronometro = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duracion = finCronometro - inicioCronometro;
    tiempoAcumulado += duracion.count();

    registrarFila(1, testoresTipicos, tiempoAcumulado, historial, tiempos, m > 1);

    //Recorrer de la segunda hasta la última fila
    for (int i=1; i<m; i++) {
        //Cronometro para cada fila
        auto inicioCronometro = chrono::high_resolution_clock::now();
        vector<vector<int>> nuevosTestores;

        for (const auto& T : testoresTipicos){
            //Caso 1: si la fila tiene un 1 en alguna columna de T, se conserva
            bool seMantiene = false;
            for (int col : T) {
                if (MB[i][col] == 1) {
                    seMantiene = true;
                    break;
                }
            }
            if (seMantiene) {
                nuevosTestores.push_back(T);

            } else {
                //Caso 2: se amplia con cada columna que tenga 1 en la fila,
                //solo si hay un conjunto compatible
                for (int j=0; j<n; j++) {
                    if (MB[i][j] == 1 and FindCompatibleSet(MB, T, j, i)) {
                        vector<int> nuevoT = T;
                        nuevoT.push_back(j);
                        sort(nuevoT.begin(), nuevoT.end());
                        nuevosTestores.push_back(nuevoT);
                    }
                }
            }
        }

        //No se eliminan repetidos: no pueden aparecer (Proposicion del reporte, Ejercicio 2)
        testoresTipicos = nuevosTestores;

        //Fin del cronómetro para la fila
        finCronometro = chrono::high_resolution_clock::now();
        duracion = finCronometro - inicioCronometro;
        tiempoAcumulado += duracion.count();

        //Imprimir datos por fila
        registrarFila(i+1, testoresTipicos, tiempoAcumulado, historial, tiempos, (i+1) != m);
    }

    return testoresTipicos;
}

//Convertir testores a texto
string testorATexto(const vector<int>& testor) {
    string texto = "{";
    for (size_t j=0; j<testor.size(); j++) {
        if (j > 0) texto += ",";
        texto += to_string(testor[j] + 1);
    }
    return texto + "}";
}

//Imprimir la tabla de resultados
void imprimirTabla(const vector<vector<vector<int>>>& historial, const vector<double>& tiempos) {
    for (size_t i=0; i<historial.size(); i++) {
        cout << left << setw(6) << i+1 << fixed << setprecision(6) << tiempos[i] << " ms" << endl;

        cout << "      ";
        if (historial[i].empty()) cout << "---";
        for (size_t t=0; t<historial[i].size(); t++) {
            if (t > 0 and t % TestoresPorLinea == 0) cout << "\n      ";
            cout << testorATexto(historial[i][t]) << " ";
        }
        cout << "\n" << endl;
    }
}

// > BT
// ================================================

//Posicion del ultimo 1 de un vector
int posicionUltimoUno(const vector<int>& v) {
    for (int j=v.size()-1; j>=0; j--) {
        if (v[j] == 1) return j+1;
    }
    return 0;
}

//3Buscar una fila de MB que impida que alpha sea lista testor
int filaResponsable(const vector<vector<int>>& MB, const vector<int>& alpha) {
    int mejorFila = -1;
    int mejorPosicion = 0;

    for (size_t i=0; i<MB.size(); i++) {
        bool hayCoincidencia = false;
        for (size_t j=0; j<alpha.size(); j++) {
            if (alpha[j] == 1 and MB[i][j] == 1) {
                hayCoincidencia = true;
                break;
            }
        }

        if (!hayCoincidencia) {
            int posicion = posicionUltimoUno(MB[i]);
            if (mejorFila == -1 or posicion < mejorPosicion) {
                mejorFila = i;
                mejorPosicion = posicion;
            }
        }
    }
    return mejorFila;
}

//Convertir una lista alpha en columnas
vector<int> columnasDeLista(const vector<int>& alpha) {
    vector<int> columnas;
    for (size_t j=0; j<alpha.size(); j++) {
        if (alpha[j] == 1) columnas.push_back(j);
    }
    return columnas;
}

//4Salto al encontrar un testor
bool saltoTestor(vector<int>& alpha, int k) {
    int n = alpha.size();
    for (int j=k; j<n; j++) alpha[j] = 0;

    int j = k-1;
    while (j >= 0 and alpha[j] == 1) {
        alpha[j] = 0;
        j--;
    }

    if (j < 0) return false;
    alpha[j] = 1;
    return true;
}

//5Salto al encontrar un no testor
void saltoNoTestor(vector<int>& alpha, int k) {
    int n = alpha.size();
    alpha[k-1] = 1;
    for (int j=k; j<n; j++) alpha[j] = 0;
}

//Obtener los testores típicos de una matriz basica con el algoritmo BT
vector<vector<int>> calcularTestoresTipicosBT(const vector<vector<int>>& MB, vector<vector<int>>& testoresEncontrados, long long& listasRevisadas) {
    int m = MB.size();
    int n = MB[0].size();

	//Testores encontrados
    vector<vector<int>> testoresTipicos;
    //Cuantos se revisaron
    listasRevisadas = 0;

    //Si hay una fila de ceros no hay testores
    for (int i=0; i<m; i++) {
        if (posicionUltimoUno(MB[i]) == 0) return testoresTipicos;
    }

    //Primera lista (0,...,0,1)
    vector<int> alpha(n, 0);
    alpha[n-1] = 1;

    bool hayMasListas = true;
    while (hayMasListas) {
        listasRevisadas++;

        //Revisar si alpha es lista testor
        int fila = filaResponsable(MB, alpha);

        if (fila == -1) {
            //Si es testor se guarda
            vector<int> columnas = columnasDeLista(alpha);
            testoresEncontrados.push_back(columnas);

            //Es tipico si no hay un testor contenido en este
            bool esTipico = true;
            for (const auto& T : testoresTipicos) {
                if (includes(columnas.begin(), columnas.end(), T.begin(), T.end())) {
                    esTipico = false;
                    break;
                }
            }
            if (esTipico) testoresTipicos.push_back(columnas);

            //1.4
            hayMasListas = saltoTestor(alpha, posicionUltimoUno(alpha));

        } else {
            //1.5
            saltoNoTestor(alpha, posicionUltimoUno(MB[fila]));
        }
    }

    //Ordenar lexicograficamente por indices de columna (la comparacion con YYC es entre conjuntos)
    sort(testoresTipicos.begin(), testoresTipicos.end());

    return testoresTipicos;
}


int main() {
    random_device rd;
    mt19937 gen(rd());

    cout << " >> Obtener los testores tipicos de una matriz" << endl;

    //Elegir opción de generación
    cout << "\nElige una opcion:" << endl;
    cout << "1. Generar matriz aleatoria" << endl;
    cout << "2. Ingresar matriz manualmente" << endl;
    int opcionMenu = leerOpcion(1, 2);

    //Ingreso del tamaño de la matriz
    cout << "> Ingresar tamano de la matriz mxn" << endl;
    int filasMD = leerEnteroPositivo("m");
    int columnasMD = leerEnteroPositivo("n");

    vector<vector<int>> matrizA;
    vector<vector<int>> matrizBasicaA;
    int filasMBA;
    double densidadMBA = 0;

    //Obtener una matriz basica con un minimo de densidad del tamaño establecido
    if(opcionMenu==1) {
        cout << "\nGenerando matriz aleatoria " << filasMD << "x" << columnasMD << "..." << endl;

        int intentos = 0;
        bool encontrada = false;

        while (!encontrada and intentos < MaxIntentos) {
            matrizA = generarMatrizAleatoria(filasMD, columnasMD, gen);
            matrizBasicaA = calcularMatrizBasica(matrizA);
            densidadMBA = calcularDensidad(matrizBasicaA);
            intentos++;
            if(densidadMBA >= DensidadMinima) encontrada = true;
        }
        if (!encontrada) cout << "No se encontro una matriz basica con densidad >= " << DensidadMinima << " en " << MaxIntentos << " intentos." << endl;

    }else{
        //Opcion 2 Insertar manualmente
        cout << "\nIngrese matriz A " << filasMD << "x" << columnasMD << ":" << endl;
        cout << "Escriba cada fila sin espacios." << endl;

        for (int i=0; i<filasMD; i++) {
            matrizA.push_back(leerFilaManual(columnasMD, i+1));
        }

        matrizBasicaA = calcularMatrizBasica(matrizA);
        densidadMBA = calcularDensidad(matrizBasicaA);
    }

    cout << "\nMatriz A " << filasMD << "x" << columnasMD << ":" << endl;
    imprimirMatriz(matrizA);

    filasMBA = matrizBasicaA.size();

    cout << "\nCalculando matriz basica..." << endl;
    cout << "\nMatriz basica A " << filasMBA << "x" << columnasMD << ":" << endl;
    imprimirMatriz(matrizBasicaA);

    cout << "\nDensidad de la matriz basica A: " << densidadMBA << endl;

    //Elegir el algoritmo
    cout << "\nElige el algoritmo:" << endl;
    cout << "1. YYC" << endl;
    cout << "2. BT" << endl;
    int opcionAlgoritmo = leerOpcion(1, 2);

    vector<vector<int>> testoresTipicos;
    vector<vector<vector<int>>> historial;
    vector<double> tiempos;

    vector<vector<int>> testoresEncontrados;
    long long listasRevisadas = 0;

    double tiempoTotal = 0;

    if (opcionAlgoritmo == 1) {
        cout << "\n >> Algoritmo YYC" << endl;
        testoresTipicos = calcularTestoresTipicosYYC(matrizBasicaA, historial, tiempos);
        //Tiempo total = tiempo acumulado tras la ultima fila (no incluye la impresion en consola)
        tiempoTotal = tiempos.back();
    } else {
        cout << "\n >> Algoritmo BT" << endl;
        //BT no imprime durante la ejecucion: se cronometra la llamada completa
        auto inicioCronometro = chrono::high_resolution_clock::now();
        testoresTipicos = calcularTestoresTipicosBT(matrizBasicaA, testoresEncontrados, listasRevisadas);
        auto finCronometro = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> duracion = finCronometro - inicioCronometro;
        tiempoTotal = duracion.count();
    }

    //Imprimir resultados
    cout << "\n >> Resultados" << endl;

    if (opcionAlgoritmo == 1) {
        cout << "\n >> Algoritmo YYC" << endl;
        cout << "\n> Tiempo y testores encontrados por fila:\n" << endl;
        imprimirTabla(historial, tiempos);
        cout << "\n> Testores tipicos (" << testoresTipicos.size() << "):" << endl;
        imprimirTestores(testoresTipicos);
    }
    if (opcionAlgoritmo == 2) {
        cout << "\n> Testores encontrados (" << testoresEncontrados.size() << "):" << endl;
        imprimirTestores(testoresEncontrados);
        cout << "\n> Listas revisadas: " << listasRevisadas << " de ";
        if (columnasMD < 64) cout << ((1ULL << columnasMD) - 1);
        else cout << "2^" << columnasMD << " - 1";
        cout << " posibles" << endl;
        cout << "\n> Testores tipicos (" << testoresTipicos.size() << "):" << endl;
        imprimirTestores(testoresTipicos);
    }

    cout << "\n> Tiempo de ejecucion total del " << (opcionAlgoritmo == 1 ? "YYC" : "BT") << ": " << fixed << setprecision(6) << tiempoTotal << " ms" << endl;

    return 0;
}