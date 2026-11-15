#include "Trie.h"
#include "TrieConcurrente.h"
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <future>
#include <thread>
#include <iomanip>

using namespace std;
using namespace std::chrono;

string palabraAleatoria(mt19937& r) {
    uniform_int_distribution<int> largo(4, 11), letra(0, 25);
    string s;
    for (int i = 0, n = largo(r); i < n; i++) s += static_cast<char>('a' + letra(r));
    return s;
}

void ejecutarPrueba(int n, unsigned int numHilos) {
    mt19937 r(7);
    vector<string> palabras;
    palabras.reserve(n);
    for (int i = 0; i < n; i++) palabras.push_back(palabraAleatoria(r));

    // --- SECUENCIAL ---
    Trie trieSecuencial;
    auto ini = high_resolution_clock::now();
    for (const auto& p : palabras) trieSecuencial.insertar(p);
    auto fin = high_resolution_clock::now();
    auto msSecuencial = duration_cast<milliseconds>(fin - ini).count();

    // --- PARALELO ---
    TrieConcurrente trieParalelo;
    auto iniParalelo = high_resolution_clock::now();

    vector<future<void>> futuros;
    int tamanoBloque = n / numHilos;

    for (unsigned int i = 0; i < numHilos; i++) {
        int inicio = i * tamanoBloque;
        int finBloque = (i == numHilos - 1) ? n : (i + 1) * tamanoBloque;
        futuros.push_back(async(launch::async, [&trieParalelo, &palabras, inicio, finBloque]() {
            for (int j = inicio; j < finBloque; j++) trieParalelo.insertar(palabras[j]);
        }));
    }
    for (auto& f : futuros) f.get();
    auto finParalelo = high_resolution_clock::now();
    auto msParalelo = duration_cast<milliseconds>(finParalelo - iniParalelo).count();

    // --- IMPRIMIR FILA DE LA TABLA ---
    cout << "| " << setw(8) << n << " | "
         << setw(9) << msSecuencial << " ms | "
         << setw(8) << msParalelo << " ms | " << endl;
}

int main() {
    unsigned int numHilos = thread::hardware_concurrency();
    if (numHilos == 0) numHilos = 4;

    cout << "Generando Benchmark (Hilos usados: " << numHilos << ")" << endl;
    cout << "---------------------------------------------" << endl;
    cout << "| Tamano (N) | Secuencial | Paralelo |" << endl;
    cout << "|------------|------------|----------|" << endl;

    // Ejecutamos las 3 pruebas que pide el profesor
    ejecutarPrueba(10000, numHilos);
    ejecutarPrueba(100000, numHilos);
    ejecutarPrueba(1000000, numHilos);

    cout << "---------------------------------------------" << endl;
    cout << "Nota: En insercion pura, el paralelismo puede ser mas lento" << endl;
    cout << "por la contencion de locks globales en la raiz del arbol." << endl;

    return 0;
}