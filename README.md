# Proyecto: Algoritmos para el cálculo de todos los testores típicos de una matriz básica

Matemáticas Discretas (MAT 2004) – Semestre 202610 – Universidad San Francisco de Quito USFQ
Profesor: Eduardo Alba

Integrantes:
- Jordi Yungan – 00346350
- Miguel Carrera – 00343860

## Estructura del repositorio

Cada ejercicio tiene su propia carpeta con su código fuente:

```
PROYECTO_MAT/
├── EJERCICIO_1/
│   └── Ejercicio1.cpp
├── EJERCICIO_2/
│   ├── Ejercicio2.cpp
│   └── matrizA.txt
├── EJERCICIO_3/
├── EJERCICIO_4/
├── EJERCICIO_5/
└── README.md
```

| Carpeta | Archivos | Descripción |
|---|---|---|
| `EJERCICIO_1` | `Ejercicio1.cpp` | Genera una matriz booleana aleatoria de m × n, calcula su matriz básica y la densidad de esta. |
| `EJERCICIO_2` | `Ejercicio2.cpp` | Genera la matriz básica A (n ≥ 7, densidad ≥ 0.4) y la guarda en un archivo, o permite ingresar una matriz fila por fila. Aplica YYC y muestra, tras cada fila, los testores típicos identificados y el tiempo acumulado. |
| `EJERCICIO_2` | `matrizA.txt` | Matriz básica A usada en el reporte (4 × 7, densidad 3/7). Se usa también en los Ejercicios 3 a 5. |
| `EJERCICIO_3` | *Pendiente* | Algoritmo BT. |
| `EJERCICIO_4` | *Pendiente* | Matriz B de testores típicos de A. |
| `EJERCICIO_5` | *Pendiente* | Operadores φ, θ, γ y experimentos. |

## Compilación y ejecución

Se requiere un compilador de C++ compatible con C++11 o superior.

### Cómo compilar y ejecutar

Desde la carpeta raíz del repositorio, entrar a la carpeta del ejercicio, compilar y ejecutar:

```bash
cd EJERCICIO_2
g++ -g Ejercicio2.cpp -o Ejercicio2.exe
./Ejercicio2.exe
```

y de forma análoga para los demás ejercicios. Si `g++` no se reconoce como comando, usar la ruta completa del compilador (por ejemplo `C:\msys64\ucrt64\bin\g++.exe`).

### Condiciones en que se midieron los tiempos del reporte

Los programas se desarrollaron y ejecutaron en **Visual Studio Code** (extensión C/C++ de Microsoft) con **GCC 16.2.0 de MSYS2 (UCRT64)**, usando el botón *Run* de la extensión. Ese botón compila con la opción `-g`, **sin optimización**, y ejecuta el programa bajo el depurador `gdb`. Los tiempos reportados en el reporte se obtuvieron en estas condiciones. El comando de la sección anterior compila el programa con las mismas opciones.

## Uso del Ejercicio 2

Al iniciar, el programa muestra un menú:

1. **Generar matriz A:** se ingresan m y n (n ≥ 7). El programa repite la generación hasta que la matriz básica tenga densidad ≥ 0.4, pide un nombre de archivo y la guarda. Después aplica YYC.
2. **Ingresar matriz manualmente:** se ingresan m y n, y luego cada fila con 0 y 1 (con o sin espacios). El programa calcula la matriz básica y aplica YYC.

La semilla del generador aleatorio no es fija, así que cada ejecución de la opción 1 produce otra matriz. Para reproducir los resultados del reporte, use la opción 2 y copie las filas de `EJERCICIO_2/matrizA.txt` (sin la primera línea).

## Formato de `matrizA.txt`

La primera línea contiene el número de filas y de columnas; después va una fila de la matriz por línea, con los valores separados por espacios:

```
4 7
0 1 1 0 0 0 0
0 1 0 0 0 1 1
1 0 0 1 1 0 1
0 0 0 0 1 1 1
```

El programa guarda el archivo en la carpeta desde donde se ejecuta y con el nombre exacto que se escriba; no agrega la extensión `.txt`. Si se ejecuta desde `EJERCICIO_2`, el archivo queda en esa carpeta.