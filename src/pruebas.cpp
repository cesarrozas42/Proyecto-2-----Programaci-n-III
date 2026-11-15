// Pruebas del Trie sin dependencias externas (no necesita Google Test).
#include "Trie.h"

#include <functional>
#include <initializer_list>
#include <iostream>
#include <random>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

int pasadas = 0;
int fallidas = 0;

struct FallaPrueba : std::runtime_error {
    using std::runtime_error::runtime_error;
};

void verificar(bool condicion, const std::string& mensaje) {
    if (!condicion) throw FallaPrueba(mensaje);
}

void verificarLanza(const std::function<void()>& accion, const std::string& mensaje) {
    try {
        accion();
    } catch (const std::invalid_argument&) {
        return;
    }
    throw FallaPrueba("debia lanzar invalid_argument: " + mensaje);
}

void prueba(const std::string& nombre, const std::function<void()>& cuerpo) {
    try {
        cuerpo();
        pasadas++;
        std::cout << "[OK]    " << nombre << '\n';
    } catch (const std::exception& e) {
        fallidas++;
        std::cout << "[FALLA] " << nombre << " -> " << e.what() << '\n';
    }
}

using Lista = std::vector<std::string>;

void cargar(Trie& t, std::initializer_list<const char*> palabras) {
    for (const char* p : palabras) t.insertar(p);
}

std::string palabraAleatoria(std::mt19937& r) {
    std::uniform_int_distribution<int> largo(1, 4), letra(0, 2);
    std::string s;
    for (int i = 0, n = largo(r); i < n; i++) s += static_cast<char>('a' + letra(r));
    return s;
}

} // namespace

int main() {
    prueba("insertar y buscar", [] {
        Trie t;
        verificar(t.insertar("casa"), "insertar nueva devuelve true");
        verificar(t.buscar("casa"), "encuentra palabra insertada");
        verificar(!t.buscar("cas"), "un prefijo no es palabra");
        verificar(!t.buscar("casas"), "una extension no es palabra");
        verificar(t.tamanio() == 1, "tamanio 1");
    });

    prueba("no duplica", [] {
        Trie t;
        t.insertar("sol");
        verificar(!t.insertar("sol"), "insertar repetida devuelve false");
        verificar(t.tamanio() == 1, "tamanio sigue en 1");
        verificar(t.contarPorPrefijo("s") == 1, "contador no se duplica");
    });

    prueba("prefijos", [] {
        Trie t;
        cargar(t, {"casa", "casco", "cama", "perro", "pera", "ca"});
        verificar(t.buscarPorPrefijo("ca") == Lista{"ca", "cama", "casa", "casco"}, "buscarPorPrefijo ordenado");
        verificar(t.contarPorPrefijo("cas") == 2, "contar 'cas'");
        verificar(t.contarPorPrefijo("pe") == 2, "contar 'pe'");
        verificar(t.contarPorPrefijo("x") == 0, "contar inexistente");
        verificar(t.existePrefijo("per"), "existe 'per'");
        verificar(!t.existePrefijo("pz"), "no existe 'pz'");
        verificar(t.buscarPorPrefijo("zzz").empty(), "prefijo inexistente -> lista vacia");
    });

    prueba("listarTodas ordenado", [] {
        Trie t;
        cargar(t, {"zeta", "alfa", "beta", "al"});
        verificar(t.listarTodas() == Lista{"al", "alfa", "beta", "zeta"}, "orden alfabetico");
    });

    prueba("eliminar hoja y poda de nodos", [] {
        Trie t;
        cargar(t, {"casa", "casco"});
        int nodosAntes = t.cantidadNodos();
        verificar(t.eliminar("casco"), "eliminar existente");
        verificar(!t.buscar("casco"), "ya no esta");
        verificar(t.buscar("casa"), "la otra sigue");
        verificar(t.cantidadNodos() == nodosAntes - 2, "se podaron 'c','o'");
        verificar(t.contarPorPrefijo("cas") == 1, "contador actualizado");
    });

    prueba("eliminar palabra que es prefijo de otra", [] {
        Trie t;
        cargar(t, {"ca", "casa"});
        int nodos = t.cantidadNodos();
        verificar(t.eliminar("ca"), "eliminar 'ca'");
        verificar(!t.buscar("ca"), "'ca' ya no es palabra");
        verificar(t.buscar("casa"), "'casa' sigue");
        verificar(t.cantidadNodos() == nodos, "no se borran nodos compartidos");
        verificar(t.existePrefijo("ca"), "'ca' sigue siendo prefijo");
    });

    prueba("eliminar inexistente y prefijo", [] {
        Trie t;
        cargar(t, {"casa"});
        verificar(!t.eliminar("cas"), "prefijo no se elimina");
        verificar(!t.eliminar("perro"), "inexistente no se elimina");
        verificar(t.tamanio() == 1, "tamanio intacto");
    });

    prueba("eliminar todo deja el arbol vacio", [] {
        Trie t;
        cargar(t, {"a", "ab", "abc"});
        t.eliminar("ab");
        t.eliminar("abc");
        t.eliminar("a");
        verificar(t.estaVacio(), "vacio");
        verificar(t.cantidadNodos() == 1, "solo queda la raiz");
        verificar(!t.existePrefijo(""), "sin palabras");
    });

    prueba("validaciones", [] {
        Trie t;
        verificarLanza([&] { t.insertar(""); }, "insertar vacio");
        verificarLanza([&] { t.buscar(""); }, "buscar vacio");
        verificarLanza([&] { t.eliminar(""); }, "eliminar vacio");
    });

    prueba("UTF-8 (tildes, enie) y mayusculas", [] {
        Trie t;
        cargar(t, {"\xC3\x91" "andu", "\xC3\xB1" "andu", "\xC3\xA1" "rbol", "zorro"}); // Ñandu, ñandu, árbol
        verificar(t.buscar("\xC3\x91" "andu") && t.buscar("\xC3\xB1" "andu"), "distingue mayusculas");
        verificar(t.buscar("\xC3\xA1" "rbol"), "tildes");
        verificar(t.tamanio() == 4, "cuatro palabras");
        verificar(t.listarTodas().front() == "zorro", "ASCII antes que acentos (orden por bytes)");
    });

    prueba("limpiar", [] {
        Trie t;
        cargar(t, {"uno", "dos"});
        t.limpiar();
        verificar(t.estaVacio() && t.cantidadNodos() == 1 && !t.buscar("uno"), "arbol reiniciado");
    });

    prueba("uso polimorfico via Arbol*", [] {
        std::unique_ptr<Arbol> a = std::make_unique<Trie>();
        a->insertar("hola");
        verificar(a->buscar("hola") && a->tamanio() == 1, "funciona a traves de la interfaz");
    });

    prueba("aleatoria contra std::set (10 000 operaciones)", [] {
        std::mt19937 r(42);
        std::uniform_int_distribution<int> op(0, 2);
        Trie t;
        std::set<std::string> esperado;
        for (int i = 0; i < 10000; i++) {
            std::string p = palabraAleatoria(r);
            switch (op(r)) {
                case 0: verificar(t.insertar(p) == esperado.insert(p).second, "insertar " + p); break;
                case 1: verificar(t.eliminar(p) == (esperado.erase(p) == 1), "eliminar " + p); break;
                default: verificar(t.buscar(p) == (esperado.count(p) == 1), "buscar " + p);
            }
        }
        verificar(t.listarTodas() == Lista(esperado.begin(), esperado.end()), "mismo contenido que std::set");
        verificar(t.tamanio() == static_cast<int>(esperado.size()), "mismo tamanio");
    });

    std::cout << "\nResultado: " << pasadas << " pruebas pasadas, " << fallidas << " fallidas\n";
    return fallidas == 0 ? 0 : 1;
}
