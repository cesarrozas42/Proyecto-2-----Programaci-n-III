// Demo para la exposición del integrante 1.
#include "Trie.h"

#include <chrono>
#include <cstdio>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>
using namespace std;
namespace {

void titulo(const string& t) {
    cout << "\n=== " << t << " ===\n";
}

string lista(const vector<string>& v) {
    string s = "[";
    for (size_t i = 0; i < v.size(); i++) s += (i ? ", " : "") + v[i];
    return s + "]";
}

const char* sn(bool b) { return b ? "true" : "false"; }

string palabraAleatoria(mt19937& r) {
    uniform_int_distribution<int> largo(4, 11), letra(0, 25);
    string s;
    for (int i = 0, n = largo(r); i < n; i++) s += static_cast<char>('a' + letra(r));
    return s;
}

void rendimiento() {
    using reloj = chrono::steady_clock;
    mt19937 r(7);
    for (int n : {10000, 100000, 1000000}) {
        vector<string> palabras;
        palabras.reserve(n);
        for (int i = 0; i < n; i++) palabras.push_back(palabraAleatoria(r));

        Trie t;
        auto ini = reloj::now();
        for (const auto& p : palabras) t.insertar(p);
        auto msInsertar = chrono::duration_cast<chrono::milliseconds>(reloj::now() - ini).count();

        ini = reloj::now();
        int encontradas = 0;
        for (int i = 0; i < 100000; i++) if (t.buscar(palabras[i % n])) encontradas++;
        double nsPorBusqueda =
            chrono::duration_cast<chrono::nanoseconds>(reloj::now() - ini).count() / 100000.0;

        printf("N=%8d  insertar: %5lld ms   buscar: %6.0f ns/op   nodos: %8d  (%d encontradas)\n",
                    n, static_cast<long long>(msInsertar), nsPorBusqueda, t.cantidadNodos(), encontradas);
    }
}

}

int main() {
    unique_ptr<Arbol> arbol = make_unique<Trie>();

    titulo("1. Insercion");
    for (const char* p : {"casa", "casco", "cama", "ca", "perro", "pera"}) {
        printf("insertar(%-6s) -> %s\n", p, sn(arbol->insertar(p)));
    }
    cout << "insertar(casa)   -> " << sn(arbol->insertar("casa")) << "  (repetida)\n";

    titulo("2. Estructura del arbol (* = fin de palabra)");
    cout << arbol->dibujar();
    cout << "palabras = " << arbol->tamanio() << ", nodos = " << arbol->cantidadNodos() << '\n';

    titulo("3. Busqueda");
    cout << "buscar(casa)  -> " << sn(arbol->buscar("casa")) << '\n';
    cout << "buscar(cas)   -> " << sn(arbol->buscar("cas")) << "  (es prefijo, no palabra)\n";
    cout << "buscar(gato)  -> " << sn(arbol->buscar("gato")) << '\n';

    titulo("4. Prefijos (autocompletado)");
    cout << "buscarPorPrefijo(ca) -> " << lista(arbol->buscarPorPrefijo("ca")) << '\n';
    cout << "contarPorPrefijo(ca) -> " << arbol->contarPorPrefijo("ca") << '\n';
    cout << "buscarPorPrefijo(pe) -> " << lista(arbol->buscarPorPrefijo("pe")) << '\n';

    titulo("5. Eliminacion");
    cout << "eliminar(casco) -> " << sn(arbol->eliminar("casco")) << "  (poda los nodos 'c','o')\n";
    cout << "eliminar(ca)    -> " << sn(arbol->eliminar("ca")) << "  (solo desmarca: es prefijo de otras)\n";
    cout << arbol->dibujar();
    cout << "palabras = " << arbol->tamanio() << ", nodos = " << arbol->cantidadNodos() << '\n';

    titulo("6. Rendimiento: el tiempo de busqueda depende de L, no de N");
    rendimiento();
}
