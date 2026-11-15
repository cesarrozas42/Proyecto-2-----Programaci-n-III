#ifndef NODO_TRIE_H
#define NODO_TRIE_H

#include <map>
#include <memory>

/**
 * Nodo del Trie (árbol de prefijos).
 *
 * Cada nodo representa un carácter (un byte). El camino desde la raíz hasta un
 * nodo forma un prefijo; si finDePalabra es true, ese prefijo es una palabra.
 *
 * La clave es unsigned char para que el orden de los bytes coincida con el
 * orden alfabético también con tildes y ñ en UTF-8.
 */
struct NodoTrie {
    /** Hijos ordenados: así los recorridos salen en orden alfabético.
        unique_ptr libera la memoria sola al borrar una rama (sin delete manual). */
    std::map<unsigned char, std::unique_ptr<NodoTrie>> hijos;

    /** Marca que el camino raíz -> este nodo forma una palabra completa. */
    bool finDePalabra = false;

    /** Cuántas palabras pasan por este nodo. Permite contar por prefijo sin recorrer el subárbol. */
    int palabrasQuePasan = 0;

    NodoTrie* hijo(unsigned char c) const {
        auto it = hijos.find(c);
        return it == hijos.end() ? nullptr : it->second.get();
    }
};

#endif
