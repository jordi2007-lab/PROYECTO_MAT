# Algoritmos para el cálculo de todos los testores típicos de una matriz básica

Matemáticas Discretas (MAT 2004), semestre 202610, Universidad San Francisco de Quito USFQ.
Profesor: Eduardo Alba Cabrera.

Integrantes:
- Jordi Yungan (00346350)
- Miguel Carrera (00343860)

## Estructura del repositorio

```
PROYECTO_MAT/
├── EJERCICIO_1/
│   └── Ejercicio1.cpp
├── EJERCICIO_2/
│   ├── Ejercicio2.cpp
│   └── matrizA
├── EJERCICIO_3/
│   └── Ejercicio3.cpp
├── EJERCICIO_4/
│   ├── Ejercicio4.cpp
│   └── matrizB
├── EJERCICIO_5/
│   └── Ejercicio5.cpp
└── README.md
```

| Archivo | Descripción |
|---|---|
| `Ejercicio1.cpp` | Genera una matriz booleana aleatoria, calcula su matriz básica y la densidad de esta. |
| `Ejercicio2.cpp` | Genera la matriz básica A (n ≥ 7, densidad ≥ 0.4) o permite ingresar una matriz, y aplica YYC. |
| `matrizA` | Matriz básica A usada en el reporte (4 × 7). |
| `Ejercicio3.cpp` | Aplica BT o YYC a una matriz generada o ingresada. |
| `Ejercicio4.cpp` | Construye la matriz B con los testores típicos de A, le aplica BT y compara A y B. |
| `matrizB` | Matriz B usada en el reporte (8 × 7). |
| `Ejercicio5.cpp` | Implementa los operadores φ, θ y γ, genera las matrices de prueba y mide los tiempos de YYC y BT. |

Los archivos `matrizA` y `matrizB` tienen el número de filas y columnas en la primera línea y después una fila por línea, con los valores separados por espacios. Los programas no los leen directamente: las filas se escriben en el menú de ingreso.

## Compilación y ejecución

Se requiere un compilador de C++11 o superior (se usó GCC 16.2.0 de MSYS2 UCRT64).

Ejercicios 1 a 4 (ejemplo con el 2):

```bash
cd EJERCICIO_2
g++ -g Ejercicio2.cpp -o Ejercicio2.exe
./Ejercicio2.exe
```

Ejercicio 5 (con optimización y fuera del depurador, porque tarda varios minutos):

```bash
cd EJERCICIO_5
g++ -O2 Ejercicio5.cpp -o Ejercicio5.exe
./Ejercicio5.exe
```

Si `g++` no se reconoce como comando, use la ruta completa del compilador (por ejemplo `C:\msys64\ucrt64\bin\g++.exe`).

## Uso

**Ejercicio 1.** Se ingresan m y n. El programa no valida la entrada.

**Ejercicio 2.** Menú:
1. Generar A: se ingresan m y n (n ≥ 7). El programa repite la generación, sin límite de intentos, hasta que la matriz básica tenga densidad ≥ 0.4. Después pide un nombre de archivo y guarda la matriz.
2. Ingresar una matriz fila por fila, con o sin espacios.

**Ejercicio 3.** Menú:
1. Generar una matriz aleatoria, con un máximo de 1000 intentos.
2. Ingresar una matriz fila por fila, **sin espacios**.

Después se elige el algoritmo: `1` para YYC o `2` para BT.

**Ejercicio 4.** Tiene las opciones del Ejercicio 3 y una tercera para ingresar toda la matriz en una sola línea, con espacios permitidos. Después de aplicar el algoritmo, pregunta si se desea construir B; si se acepta, pide un nombre de archivo para guardarla.

**Ejercicio 5.** Tiene el mismo menú que el Ejercicio 4. Al final pregunta si se desean generar las matrices de prueba. En las matrices de prueba, BT se detiene si supera los 60 s y en la tabla aparece `-`. Una ejecución completa tarda unos 10 minutos.

> En los Ejercicios 3 a 5, las filas ingresadas no se validan: todo carácter distinto de `0` se lee como `1`. Por eso las filas deben escribirse sin espacios (o usar la opción 3 en los Ejercicios 4 y 5).