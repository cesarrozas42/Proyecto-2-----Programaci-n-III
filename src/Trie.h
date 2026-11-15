#ifndef TRIE_H
#define TRIE_H

#include "Arbol.h"
#include "NodoTrie.h"

#include <memory>
#include <string>
#include <vector>

/**
 * Árbol de prefijos (Trie) para almacenar strings.
 *
 * Complejidades (L = longitud de la palabra o prefijo):
 *  - insertar / buscar / eliminar : O(L)  — no depende de cuántas palabras haya
 *  - contarPorPrefijo            : O(L)  — gracias al contador en cada nodo
 *  - buscarPorPrefijo            : O(L + tamaño del subárbol)
 *  - listarTodas                 : O(total de nodos)
 *
 * Distingue mayúsculas y minúsculas; la normalización la decide quien llama.
 *
 * NO es thread-safe: el integrante 2 la envuelve o sincroniza para la versión paralela.
 */
class Trie : public Arbol {
public:
    Trie();

    bool insertar(const std::string& palabra) override;
    bool buscar(const std::string& palabra) const override;
    bool eliminar(const std::string& palabra) override;
    bool existePrefijo(const std::string& prefijo) const override;
    std::vector<std::string> buscarPorPrefijo(const std::string& prefijo) const override;
    int contarPorPrefijo(const std::string& prefijo) const override;
    std::vector<std::string> listarTodas() const override;
    int tamanio() const override;
    bool estaVacio() const override;
    int cantidadNodos() const override;
    void limpiar() override;
    std::string dibujar() const override;

private:
    std::unique_ptr<NodoTrie> raiz;
    int numPalabras = 0;
    int numNodos = 1; // la raíz

    const NodoTrie* bajarHasta(const std::string& texto) const;
    static void recolectar(const NodoTrie* nodo, std::string& camino, std::vector<std::string>& resultado);
    static int contarNodos(const NodoTrie* nodo);
    static void dibujar(const NodoTrie* nodo, const std::string& sangria, std::string& salida);
    static void validar(const std::string& palabra);
};

#endif
