#ifndef TRIE_CONCURRENTE_H
#define TRIE_CONCURRENTE_H

#include "Arbol.h"
#include "Trie.h"
#include <shared_mutex>
#include <string>
#include <vector>

class TrieConcurrente : public Arbol {
private:
    Trie arbolInterno;
    // Este mutex permite múltiples lectores a la vez, pero bloquea todo si hay un escritor
    mutable std::shared_mutex mutexArbol;

public:
    TrieConcurrente() = default;

    // --- Escrituras (Bloqueo exclusivo) ---
    bool insertar(const std::string& palabra) override;
    bool eliminar(const std::string& palabra) override;
    void limpiar() override;

    // --- Lecturas (Bloqueo compartido) ---
    bool buscar(const std::string& palabra) const override;
    bool existePrefijo(const std::string& prefijo) const override;
    std::vector<std::string> buscarPorPrefijo(const std::string& prefijo) const override;
    int contarPorPrefijo(const std::string& prefijo) const override;
    std::vector<std::string> listarTodas() const override;
    int tamanio() const override;
    bool estaVacio() const override;
    int cantidadNodos() const override;
    std::string dibujar() const override;
};

#endif // TRIE_CONCURRENTE_HS_TRIECONCURRENTE_H