#include "Trie.h"

#include <stdexcept>

Trie::Trie() : raiz(std::make_unique<NodoTrie>()) {}

// ------------------------------------------------------------------ inserción

bool Trie::insertar(const std::string& palabra) {
    validar(palabra);
    if (buscar(palabra)) {
        return false; // no se duplican palabras
    }
    NodoTrie* actual = raiz.get();
    actual->palabrasQuePasan++;
    for (unsigned char c : palabra) {
        auto& siguiente = actual->hijos[c]; // crea la entrada vacía si no existe
        if (!siguiente) {
            siguiente = std::make_unique<NodoTrie>();
            numNodos++;
        }
        siguiente->palabrasQuePasan++;
        actual = siguiente.get();
    }
    actual->finDePalabra = true;
    numPalabras++;
    return true;
}

// ------------------------------------------------------------------ búsqueda

bool Trie::buscar(const std::string& palabra) const {
    validar(palabra);
    const NodoTrie* nodo = bajarHasta(palabra);
    return nodo != nullptr && nodo->finDePalabra;
}

bool Trie::existePrefijo(const std::string& prefijo) const {
    const NodoTrie* nodo = bajarHasta(prefijo);
    return nodo != nullptr && nodo->palabrasQuePasan > 0;
}

int Trie::contarPorPrefijo(const std::string& prefijo) const {
    const NodoTrie* nodo = bajarHasta(prefijo);
    return nodo == nullptr ? 0 : nodo->palabrasQuePasan;
}

std::vector<std::string> Trie::buscarPorPrefijo(const std::string& prefijo) const {
    std::vector<std::string> resultado;
    const NodoTrie* nodo = bajarHasta(prefijo);
    if (nodo != nullptr) {
        std::string camino = prefijo;
        recolectar(nodo, camino, resultado);
    }
    return resultado;
}

std::vector<std::string> Trie::listarTodas() const {
    return buscarPorPrefijo("");
}

/** Recorre el árbol carácter por carácter; devuelve nullptr si el camino no existe. */
const NodoTrie* Trie::bajarHasta(const std::string& texto) const {
    const NodoTrie* actual = raiz.get();
    for (unsigned char c : texto) {
        actual = actual->hijo(c);
        if (actual == nullptr) return nullptr;
    }
    return actual;
}

/** Recorrido en profundidad (DFS) en preorden: como los hijos están en un std::map, sale ordenado. */
void Trie::recolectar(const NodoTrie* nodo, std::string& camino, std::vector<std::string>& resultado) {
    if (nodo->finDePalabra) {
        resultado.push_back(camino);
    }
    for (const auto& [c, hijo] : nodo->hijos) {
        camino.push_back(static_cast<char>(c));
        recolectar(hijo.get(), camino, resultado);
        camino.pop_back(); // backtracking
    }
}

// ------------------------------------------------------------------ eliminación

bool Trie::eliminar(const std::string& palabra) {
    validar(palabra);
    if (!buscar(palabra)) {
        return false;
    }
    // La palabra existe: bajamos descontando el contador y podamos la primera
    // rama que se queda sin palabras. Al borrar el unique_ptr se libera
    // automáticamente todo el subárbol que cuelga de ella.
    NodoTrie* actual = raiz.get();
    actual->palabrasQuePasan--;
    for (unsigned char c : palabra) {
        NodoTrie* siguiente = actual->hijo(c);
        siguiente->palabrasQuePasan--;
        if (siguiente->palabrasQuePasan == 0) {
            numNodos -= contarNodos(siguiente);
            actual->hijos.erase(c);
            numPalabras--;
            return true;
        }
        actual = siguiente;
    }
    actual->finDePalabra = false; // la palabra era prefijo de otra: solo se desmarca
    numPalabras--;
    return true;
}

int Trie::contarNodos(const NodoTrie* nodo) {
    int total = 1;
    for (const auto& [c, hijo] : nodo->hijos) {
        total += contarNodos(hijo.get());
    }
    return total;
}

// ------------------------------------------------------------------ utilidades

int Trie::tamanio() const { return numPalabras; }

bool Trie::estaVacio() const { return numPalabras == 0; }

int Trie::cantidadNodos() const { return numNodos; }

void Trie::limpiar() {
    raiz = std::make_unique<NodoTrie>(); // el árbol anterior se libera solo
    numPalabras = 0;
    numNodos = 1;
}

std::string Trie::dibujar() const {
    std::string salida = "(raiz)\n";
    dibujar(raiz.get(), "", salida);
    return salida;
}

void Trie::dibujar(const NodoTrie* nodo, const std::string& sangria, std::string& salida) {
    std::size_t i = 0;
    for (const auto& [c, hijo] : nodo->hijos) {
        bool ultimo = ++i == nodo->hijos.size();
        salida += sangria;
        salida += ultimo ? "`-- " : "|-- ";
        salida += static_cast<char>(c);
        if (hijo->finDePalabra) salida += " *";
        salida += '\n';
        dibujar(hijo.get(), sangria + (ultimo ? "    " : "|   "), salida);
    }
}

void Trie::validar(const std::string& palabra) {
    if (palabra.empty()) {
        throw std::invalid_argument("La palabra no puede ser vacia");
    }
}
