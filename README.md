# Proyecto: Algoritmos para el cálculo de todos los testores típicos de una matriz básica

Matemáticas Discretas (MAT 2004) – Semestre 202610 – Universidad San Francisco de Quito USFQ
Profesor: Eduardo Alba

Integrantes:
- Jordi Yungan – 00346350
- Miguel Carrera – 00343860

## Contenido

| Archivo | Ejercicio | Descripción |
|---|---|---|
| `Ejercicio1.cpp` | 1 | Genera una matriz booleana aleatoria de m × n, calcula su matriz básica y la densidad de esta. |
| `Ejercicio2.cpp` | 2 | Genera la matriz básica A (n ≥ 7, densidad ≥ 0.4) y la guarda en un archivo, o permite ingresar una matriz fila por fila. Aplica YYC y muestra, tras cada fila, los testores típicos identificados y el tiempo acumulado. |
| `datos/matrizA.txt` | 2–5 | Matriz básica A usada en el informe (4 × 7, densidad 3/7). |
| `Ejercicio3.cpp` | 3 | *Pendiente:* algoritmo BT. |
| `Ejercicio5.cpp` | 4–5 | *Pendiente:* operadores φ, θ, γ y experimentos. |

## Compilación y ejecución

Se requiere un compilador de C++ compatible con C++11 o superior (se usó GCC).

```bash
g++ -std=c++11 Ejercicio1.cpp -o Ejercicio1
./Ejercicio1

g++ -std=c++11 Ejercicio2.cpp -o Ejercicio2
./Ejercicio2
```

En Windows el ejecutable es `Ejercicio1.exe` / `Ejercicio2.exe`. También pueden compilarse desde Dev-C++ u otro IDE.

## Uso del Ejercicio 2

Al iniciar, el programa muestra un menú:

1. **Generar matriz A:** se ingresan m y n (n ≥ 7). El programa repite la generación hasta que la matriz básica tenga densidad ≥ 0.4, pide un nombre de archivo y la guarda. Después aplica YYC.
2. **Ingresar matriz manualmente:** se ingresan m y n, y luego cada fila con 0 y 1 (con o sin espacios). El programa calcula la matriz básica y aplica YYC.

La semilla del generador aleatorio no es fija, así que cada ejecución de la opción 1 produce otra matriz. Para reproducir los resultados del informe, use la opción 2 y copie las filas de `datos/matrizA.txt` (sin la primera línea).

## Formato de `matrizA.txt`

La primera línea contiene el número de filas y de columnas; después va una fila de la matriz por línea, con los valores separados por espacios:

```
4 7
0 1 1 0 0 0 0
0 1 0 0 0 1 1
1 0 0 1 1 0 1
0 0 0 0 1 1 1
```

El programa guarda el archivo en la carpeta desde donde se ejecuta (normalmente la del `.exe`) y con el nombre exacto que se escriba; no agrega la extensión `.txt`.
