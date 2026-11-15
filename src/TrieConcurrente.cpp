#include "TrieConcurrente.h"
#include<mutex>
using namespace std;

// ESCRITURA (Usa unique_lock: bloquea todo)

bool TrieConcurrente::insertar(const std::string& palabra) {
    std::unique_lock<shared_mutex> lock(mutexArbol);
    return arbolInterno.insertar(palabra);
}

bool TrieConcurrente::eliminar(const std::string& palabra) {
    std::unique_lock<shared_mutex> lock(mutexArbol);
    return arbolInterno.eliminar(palabra);
}

void TrieConcurrente::limpiar() {
    std::unique_lock<shared_mutex> lock(mutexArbol);
    arbolInterno.limpiar();
}

// LECTURA (Usa shared_lock: permite varios a la vez)

bool TrieConcurrente::buscar(const std::string& palabra) const {
    std::shared_lock<std::shared_mutex> lock(mutexArbol);
    return arbolInterno.buscar(palabra);
}

bool TrieConcurrente::existePrefijo(const std::string& prefijo) const {
    std::shared_lock<std::shared_mutex> lock(mutexArbol);
    return arbolInterno.existePrefijo(prefijo);
}

std::vector<std::string> TrieConcurrente::buscarPorPrefijo(const std::string& prefijo) const {
    std::shared_lock<std::shared_mutex> lock(mutexArbol);
    return arbolInterno.buscarPorPrefijo(prefijo);
}

int TrieConcurrente::contarPorPrefijo(const std::string& prefijo) const {
    std::shared_lock<std::shared_mutex> lock(mutexArbol);
    return arbolInterno.contarPorPrefijo(prefijo);
}

std::vector<std::string> TrieConcurrente::listarTodas() const {
    std::shared_lock<std::shared_mutex> lock(mutexArbol);
    return arbolInterno.listarTodas();
}

int TrieConcurrente::tamanio() const {
    std::shared_lock<std::shared_mutex> lock(mutexArbol);
    return arbolInterno.tamanio();
}

bool TrieConcurrente::estaVacio() const {
    std::shared_lock<std::shared_mutex> lock(mutexArbol);
    return arbolInterno.estaVacio();
}

int TrieConcurrente::cantidadNodos() const {
    std::shared_lock<std::shared_mutex> lock(mutexArbol);
    return arbolInterno.cantidadNodos();
}

std::string TrieConcurrente::dibujar() const {
    std::shared_lock<std::shared_mutex> lock(mutexArbol);
    return arbolInterno.dibujar();
}