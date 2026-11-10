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
//Valores de N en la tabla de phi y gamma
const int MaximoPhi = 5;
const int MaximoGamma = 4;

//Tiempo total
double tiempoTotal = 0;

//Ejercicio 5: limite de tiempo de BT (ms), si la ultima ejecucion se detuvo y cuantas listas reviso
const double LimiteBT_ms = 60000;
bool ejecucionCortada = false;
long long listasUltimaEjecucion = 0;

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
//Interpretar una matriz desde una fila
vector<vector<int>> leerMatrizUnaLinea(int m, int n) {
    string entrada;
    vector<int> valores;

    cout << "Matriz: ";
    getline(cin, entrada);

    for (char c : entrada) {
        if (c == '0') valores.push_back(0);
        if (c == '1') valores.push_back(1);
    }

    //Los valores que falten cuentan como 1
    vector<vector<int>> matriz(m, vector<int>(n, 1));
    for (int i=0; i<m; i++) {
        for (int j=0; j<n; j++) {
            int posicion = i*n + j;
            if (posicion < static_cast<int>(valores.size())) matriz[i][j] = valores[posicion];
        }
    }
    return matriz;
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
vector<vector<int>> calcularTestoresTipicosYYC(const vector<vector<int>>& MB, vector<vector<vector<int>>>& historial, vector<double>& tiempos) {
    int m = MB.size();
    int n = MB[0].size();

    vector<vector<int>> testoresTipicos;
    tiempoTotal = 0;

    //Columnas con 1 en la primera fila son testores
    auto inicioCronometro = chrono::high_resolution_clock::now();

    for (int j=0; j<n; j++) {
        if (MB[0][j] == 1) {
            testoresTipicos.push_back({j});
        }
    }
    auto finCronometro = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duracion = finCronometro - inicioCronometro;
    tiempoTotal += duracion.count();

    registrarFila(1, testoresTipicos, tiempoTotal, historial, tiempos, m > 1);

    //Recorrer de la segunda hasta la última fila
    for (int i=1; i<m; i++) {
        //Cronometro para cada fila
        auto inicioCronometro = chrono::high_resolution_clock::now();
        vector<vector<int>> nuevosTestores;

        for (const auto& T : testoresTipicos){
            //Si en la siguiente fila también hay un 1 se mantiene el testor
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
                // Si no, se buscan columnas que tengan un 1
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
        testoresTipicos = nuevosTestores;

        //Fin del cronómetro para la fila
        finCronometro = chrono::high_resolution_clock::now();
        duracion = finCronometro - inicioCronometro;
        tiempoTotal += duracion.count();

        //Imprimir datos por fila
        registrarFila(i+1, testoresTipicos, tiempoTotal, historial, tiempos, (i+1) != m);
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
    tiempoTotal = 0;
    ejecucionCortada = false;

    //El cronometro incluye la revision de filas de ceros, como en los Ejercicios 3 y 4
    auto inicioCronometro = chrono::high_resolution_clock::now();

    //Si hay una fila de ceros no hay testores
    for (int i=0; i<m; i++) {
        if (posicionUltimoUno(MB[i]) == 0) {
            chrono::duration<double, milli> duracion = chrono::high_resolution_clock::now() - inicioCronometro;
            tiempoTotal = duracion.count();
            return testoresTipicos;
        }
    }

    //Primera lista (0,...,0,1)
    vector<int> alpha(n, 0);
    alpha[n-1] = 1;

    bool hayMasListas = true;
    while (hayMasListas) {
        //Limite de tiempo del Ejercicio 5 (se revisa cada 2^16 listas)
        if (listasRevisadas > 0 and (listasRevisadas & 0xFFFF) == 0) {
            chrono::duration<double, milli> t = chrono::high_resolution_clock::now() - inicioCronometro;
            if (t.count() > LimiteBT_ms) {
                ejecucionCortada = true;
                break;
            }
        }
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

    //Ordenar para que salgan en el mismo orden que en YYC
    sort(testoresTipicos.begin(), testoresTipicos.end());

    auto finCronometro = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duracion = finCronometro - inicioCronometro;
    tiempoTotal += duracion.count();

    return testoresTipicos;
}

//Construir la matriz B con una fila binaria por cada testor
vector<vector<int>> construirMatrizB(const vector<vector<int>>& testores, int n) {
    vector<vector<int>> B(testores.size(), vector<int>(n, 0));

    for (size_t i=0; i<testores.size(); i++) {
        for (int col : testores[i]) {
            B[i][col] = 1;
        }
    }
    return B;
}

//Convertir cada fila de una matriz en un testor
vector<vector<int>> filasATestores(const vector<vector<int>>& matriz) {
    vector<vector<int>> testores;
    for (size_t i=0; i<matriz.size(); i++) {
        testores.push_back(columnasDeLista(matriz[i]));
    }
    return testores;
}

//Verificar si una matriz es basica
bool esMatrizBasica(const vector<vector<int>>& matriz) {
    return calcularMatrizBasica(matriz) == matriz;
}

//Construir B y analizar las mtrices
void analizarMatrizB(const vector<vector<int>>& matrizA, const vector<vector<int>>& matrizBasicaA, const vector<vector<int>>& testoresTipicosA) {
    int n = matrizA[0].size();

    if (testoresTipicosA.empty()) {
        cout << "\nA no tiene testores tipicos, no se puede construir B." << endl;
        return;
    }

    //Construir B y revisar que sea basica
    vector<vector<int>> matrizB = construirMatrizB(testoresTipicosA, n);

    cout << "\n >> Ejercicio 4" << endl;
    cout << "\nMatriz B " << matrizB.size() << "x" << n << " (una fila por cada testor tipico de A):" << endl;
    imprimirMatriz(matrizB);

    cout << "\nDensidad de la matriz B: " << calcularDensidad(matrizB) << endl;
    cout << "B es una matriz basica: " << (esMatrizBasica(matrizB) ? "Si" : "No") << endl;

    //Obtener los testores típicos de B con BT
    vector<vector<int>> testoresEncontradosB;
    long long listasRevisadasB = 0;

    vector<vector<int>> testoresTipicosB = calcularTestoresTipicosBT(matrizB, testoresEncontradosB, listasRevisadasB);

    cout << "\n >> Algoritmo BT sobre B" << endl;
    cout << "\n> Testores encontrados (" << testoresEncontradosB.size() << "):" << endl;
    imprimirTestores(testoresEncontradosB);
    cout << "\n> Listas revisadas: " << listasRevisadasB << " de ";
    if (n < 64) cout << ((1ULL << n) - 1);
    else cout << "2^" << n << " - 1";
    cout << " posibles" << endl;
    cout << "\n> Testores tipicos de B (" << testoresTipicosB.size() << "):" << endl;
    imprimirTestores(testoresTipicosB);
    cout << "\n> Tiempo de ejecucion total del BT sobre B: " << fixed << setprecision(6) << tiempoTotal << " ms" << endl;

    //Comparar A y B
    vector<vector<int>> filasBasicaA = filasATestores(matrizBasicaA);
    sort(filasBasicaA.begin(), filasBasicaA.end());

    cout << "\n> Comparacion entre A y B:" << endl;
    cout << "Matriz A: " << matrizA.size() << "x" << n << endl;
    cout << "Matriz basica de A: " << matrizBasicaA.size() << "x" << n << ", densidad " << calcularDensidad(matrizBasicaA) << endl;
    cout << "Matriz B: " << matrizB.size() << "x" << n << ", densidad " << calcularDensidad(matrizB) << endl;

    cout << "\n> Testores tipicos de A (" << testoresTipicosA.size() << "):" << endl;
    imprimirTestores(testoresTipicosA);
    cout << "\n> Testores tipicos de B (" << testoresTipicosB.size() << "):" << endl;
    imprimirTestores(testoresTipicosB);

    cout << "\nLos testores tipicos de B son las filas de la matriz basica de A: " << (filasBasicaA == testoresTipicosB ? "Si" : "No") << endl;
}

//Operadores
//Theta: Cada fila de A se une con cada fila de B
vector<vector<int>> operadorTheta(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    vector<vector<int>> resultado;

    for (size_t i=0; i<A.size(); i++) {
        for (size_t k=0; k<B.size(); k++) {
            vector<int> fila = A[i];
            fila.insert(fila.end(), B[k].begin(), B[k].end());
            resultado.push_back(fila);
        }
    }
    return resultado;
}

//Gamma: A va arriba a la izquierda, B va abajo a la derecha y se llena con ceros
vector<vector<int>> operadorGamma(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int nA = A[0].size();
    int nB = B[0].size();
    vector<vector<int>> resultado;

    for (size_t i=0; i<A.size(); i++) {
        vector<int> fila = A[i];
        fila.insert(fila.end(), nB, 0);
        resultado.push_back(fila);
    }
    for (size_t k=0; k<B.size(); k++) {
        vector<int> fila(nA, 0);
        fila.insert(fila.end(), B[k].begin(), B[k].end());
        resultado.push_back(fila);
    }
    return resultado;
}

//Phi: A y B se unen por columnas
vector<vector<int>> operadorPhi(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    vector<vector<int>> resultado = A;

    for (size_t i=0; i<A.size(); i++) {
        resultado[i].insert(resultado[i].end(), B[i].begin(), B[i].end());
    }
    return resultado;
}

//Aplicar phi N veces a la misma matriz
vector<vector<int>> potenciaPhi(const vector<vector<int>>& A, int N) {
    vector<vector<int>> resultado = A;
    for (int k=2; k<=N; k++) resultado = operadorPhi(resultado, A);
    return resultado;
}

//Aplicar gamma N veces a la misma matriz
vector<vector<int>> potenciaGamma(const vector<vector<int>>& A, int N) {
    vector<vector<int>> resultado = A;
    for (int k=2; k<=N; k++) resultado = operadorGamma(resultado, A);
    return resultado;
}

//Contar los 1 de una fila
int contarUnos(const vector<int>& fila) {
    int unos = 0;
    for (int valor : fila) {
        if (valor == 1) unos++;
    }
    return unos;
}

//Comparar dos filas segun su cantidad de unos
bool tieneMenosUnos(const vector<int>& a, const vector<int>& b) {
    return contarUnos(a) < contarUnos(b);
}

//Ordenar las filas de forma ascendente segun su cantidad de unos
vector<vector<int>> ordenarFilasAscendente(vector<vector<int>> matriz) {
    stable_sort(matriz.begin(), matriz.end(), tieneMenosUnos);
    return matriz;
}

//Cantidad de testores tipicos de phi^N
long long testoresPhi(const vector<int>& longitudes, int N) {
    long long total = 0;

    for (size_t i=0; i<longitudes.size(); i++) {
        long long potencia = 1;
        for (int k=0; k<longitudes[i]; k++) potencia *= N;
        total += potencia;
    }
    return total;
}

//Cantidad de testores tipicos de gamma^N
long long testoresGamma(long long cantidad, int N) {
    long long total = 1;
    for (int k=0; k<N; k++) total *= cantidad;
    return total;
}

//Medir el tiempo de un algoritmo
double medirAlgoritmo(const vector<vector<int>>& MB, int algoritmo, long long& cantidadTestores) {
    vector<vector<int>> testores;
    vector<vector<vector<int>>> historial;
    vector<double> tiempos;
    vector<vector<int>> testoresEncontrados;
    long long listasRevisadas = 0;

    //Se apaga la salida para que no se impriman las filas
    ejecucionCortada = false;
    cout.setstate(ios::badbit);
    if (algoritmo == 1) testores = calcularTestoresTipicosYYC(MB, historial, tiempos);
    else testores = calcularTestoresTipicosBT(MB, testoresEncontrados, listasRevisadas);
    cout.clear();
    listasUltimaEjecucion = listasRevisadas;

    cantidadTestores = testores.size();

    return tiempoTotal;
}

//Imprimir un tiempo de la tabla
void imprimirTiempoTabla(double tiempo, int ancho) {
    cout << setw(ancho) << tiempo;
}

//Imprimir el encabezado de la tabla
void imprimirEncabezadoExperimento() {
    cout << left << fixed << setprecision(6);
    cout << setw(4) << "N" << setw(24) << "Matriz" << setw(8) << "Filas" << setw(10) << "Columnas" << setw(12) << "Testores"
         << setw(18) << "YYC original" << setw(18) << "YYC ascendente" << setw(18) << "BT original" << setw(18) << "BT ascen" << endl;
    cout << string(120, '-') << endl;
}

//Imprimir una fila de tiempos de YYC y BT con las filas en orden original y ascendente
void imprimirFilaExperimento(int N, const string& nombre, const vector<vector<int>>& matriz, long long esperados, bool& conteoCorrecto) {
    vector<vector<int>> matrizAscendente = ordenarFilasAscendente(matriz);
    long long cantidad = 0;
    double tiemposEjecucion[4];
    bool cortada[4];
    long long listas[4];

    for (int k=0; k<4; k++) {
        int algoritmo = (k < 2) ? 1 : 2;
        const vector<vector<int>>& usada = (k%2 == 0) ? matriz : matrizAscendente;

        tiemposEjecucion[k] = medirAlgoritmo(usada, algoritmo, cantidad);
        cortada[k] = ejecucionCortada;
        listas[k] = listasUltimaEjecucion;
        //Las ejecuciones detenidas por el limite no se comparan
        if (!cortada[k] and cantidad != esperados) conteoCorrecto = false;
    }

    cout << left << setw(4) << N << setw(24) << nombre << setw(8) << matriz.size() << setw(10) << matriz[0].size() << setw(12) << esperados;

    for (int k=0; k<4; k++){
    	if (cortada[k]) cout << setw(18) << "-";
    	else imprimirTiempoTabla(tiemposEjecucion[k], 18);
	}
    cout << endl;

    //Listas revisadas por BT (orden original / ascendente)
    cout << "    BT listas revisadas: " << listas[2] << (cortada[2] ? " (detenido)" : "")
         << " / " << listas[3] << (cortada[3] ? " (detenido)" : "") << endl;
}

//Generar matrices de prueba y medir los algoritmos
void experimentoOperadores(const vector<vector<int>>& matrizBasicaA, const vector<vector<int>>& testoresTipicosA) {
    if (testoresTipicosA.empty()) {
        cout << "\nA no tiene testores tipicos, no se puede construir B." << endl;
        return;
    }

    int n = matrizBasicaA[0].size();

    //Matriz B
    vector<vector<int>> matrizB = construirMatrizB(testoresTipicosA, n);
    vector<vector<int>> testoresEncontradosB;
    long long listasRevisadasB = 0;
    vector<vector<int>> testoresTipicosB = calcularTestoresTipicosBT(matrizB, testoresEncontradosB, listasRevisadasB);

    //Longitud testores típicos de A y de B
    vector<int> longitudes;
    for (size_t i=0; i<testoresTipicosA.size(); i++){
    	longitudes.push_back(testoresTipicosA[i].size());
	}
    for (size_t i=0; i<testoresTipicosB.size(); i++) {
    	longitudes.push_back(testoresTipicosB[i].size());
	}

    //Numero de testores de theta(A,B)
    vector<vector<int>> matrizTheta = operadorTheta(matrizBasicaA, matrizB);
    long long testoresTheta = testoresTipicosA.size() + testoresTipicosB.size();

    cout << "\nTestores tipicos de A: " << testoresTipicosA.size() << ", de B: " << testoresTipicosB.size() << endl;
    cout << "\nMatriz theta(A,B) " << matrizTheta.size() << "x" << matrizTheta[0].size() << ":" << endl;
    imprimirMatriz(matrizTheta);

    bool conteoCorrecto = true;

    cout << "\n> Phi^N(theta(A,B)), tiempos en ms\n" << endl;
    imprimirEncabezadoExperimento();
    for (int N=1; N<=MaximoPhi; N++) {
        imprimirFilaExperimento(N, "phi^" + to_string(N) + "(theta(A,B))", potenciaPhi(matrizTheta, N), testoresPhi(longitudes, N), conteoCorrecto);
    }

    cout << "\n> Gamma^N(theta(A,B)), tiempos en ms\n" << endl;
    imprimirEncabezadoExperimento();
    for (int N=1; N<=MaximoGamma; N++) {
        imprimirFilaExperimento(N, "gamma^" + to_string(N) + "(theta(A,B))", potenciaGamma(matrizTheta, N), testoresGamma(testoresTheta, N), conteoCorrecto);
    }

    cout << "\n'-': BT se detuvo al superar el limite de " << static_cast<int>(LimiteBT_ms/1000) << " s" << endl;
    cout << "Los testores encontrados coinciden con los esperados (ejecuciones terminadas): " << (conteoCorrecto ? "Si" : "No") << endl;
}


int main() {
    random_device rd;
    mt19937 gen(rd());

    cout << " >> Obtener los testores tipicos de una matriz" << endl;

    //Elegir opción de generación
    cout << "\nElige una opcion:" << endl;
    cout << "1. Generar matriz aleatoria" << endl;
    cout << "2. Ingresar matriz fila por fila" << endl;
    cout << "3. Ingresar matriz en una sola linea" << endl;
    int opcionMenu = leerOpcion(1, 3);

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
    	//Opcion 1 Generar una
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
        //Insertar manualmente
        cout << "\nIngrese matriz A " << filasMD << "x" << columnasMD << ":" << endl;

		if (opcionMenu == 2) {
            //Opcion 2 Fila por fila
            cout << "Escriba cada fila sin espacios." << endl;

            for (int i=0; i<filasMD; i++) {
                matrizA.push_back(leerFilaManual(columnasMD, i+1));
            }
        } else {
            //Opcion 3 Una sola linea
            cout << "Escriba todos los valores en una sola linea (se ignoran los espacios)." << endl;
            matrizA = leerMatrizUnaLinea(filasMD, columnasMD);
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

    //Se buscan los testores típicos
    if (opcionAlgoritmo == 1) {
        cout << "\n >> Algoritmo YYC" << endl;
        testoresTipicos = calcularTestoresTipicosYYC(matrizBasicaA, historial, tiempos);
    } else {
        cout << "\n >> Algoritmo BT" << endl;
        testoresTipicos = calcularTestoresTipicosBT(matrizBasicaA, testoresEncontrados, listasRevisadas);
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

    //Construir B
    cout << "\nConstruir la matriz B con los testores tipicos de A?" << endl;
    cout << "1. Si" << endl;
    cout << "2. No" << endl;
    int opcionB = leerOpcion(1, 2);

    if (opcionB == 1) analizarMatrizB(matrizA, matrizBasicaA, testoresTipicos);

    //Generar matrices de prueba con los operadores
    cout << "\nGenerar las matrices de prueba con A y B?" << endl;
    cout << "1. Si" << endl;
    cout << "2. No" << endl;
    int opcionExperimento = leerOpcion(1, 2);

    if (opcionExperimento == 1) experimentoOperadores(matrizBasicaA, testoresTipicos);

    return 0;
}