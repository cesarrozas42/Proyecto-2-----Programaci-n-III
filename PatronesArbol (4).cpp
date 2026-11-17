#include "PatronesArbol.h"
#include "Trie.h"
#include "TrieConcurrente.h"

#include <algorithm>
#include <stdexcept>
#include <utility>

std::unique_ptr<Arbol> EstrategiaSecuencial::crearArbol() const {
    return std::make_unique<Trie>();
}

std::string EstrategiaSecuencial::nombre() const {
    return "Secuencial";
}

std::unique_ptr<Arbol> EstrategiaParalela::crearArbol() const {
    return std::make_unique<TrieConcurrente>();
}

std::string EstrategiaParalela::nombre() const {
    return "Paralela";
}

GestorArbol::GestorArbol()
    : GestorArbol(std::make_unique<EstrategiaSecuencial>()) {}

GestorArbol::GestorArbol(std::unique_ptr<EstrategiaEjecucion> estrategiaInicial)
    : estrategia(std::move(estrategiaInicial)) {
    if (!estrategia) {
        throw std::invalid_argument("La estrategia inicial no puede ser nula");
    }
    arbol = estrategia->crearArbol();
}

bool GestorArbol::insertar(const std::string& palabra) {
    const bool resultado = arbol->insertar(palabra);
    if (resultado) {
        notificar({TipoEventoArbol::Insertado, palabra, estrategia->nombre()});
    }
    return resultado;
}

bool GestorArbol::buscar(const std::string& palabra) const {
    return arbol->buscar(palabra);
}

bool GestorArbol::eliminar(const std::string& palabra) {
    const bool resultado = arbol->eliminar(palabra);
    if (resultado) {
        notificar({TipoEventoArbol::Eliminado, palabra, estrategia->nombre()});
    }
    return resultado;
}

bool GestorArbol::existePrefijo(const std::string& prefijo) const {
    return arbol->existePrefijo(prefijo);
}

std::vector<std::string> GestorArbol::buscarPorPrefijo(const std::string& prefijo) const {
    return arbol->buscarPorPrefijo(prefijo);
}

int GestorArbol::contarPorPrefijo(const std::string& prefijo) const {
    return arbol->contarPorPrefijo(prefijo);
}

std::vector<std::string> GestorArbol::listarTodas() const {
    return arbol->listarTodas();
}

int GestorArbol::tamanio() const {
    return arbol->tamanio();
}

bool GestorArbol::estaVacio() const {
    return arbol->estaVacio();
}

int GestorArbol::cantidadNodos() const {
    return arbol->cantidadNodos();
}

void GestorArbol::limpiar() {
    if (arbol->estaVacio()) return;
    arbol->limpiar();
    notificar({TipoEventoArbol::Limpiado, "", estrategia->nombre()});
}

std::string GestorArbol::dibujar() const {
    return arbol->dibujar();
}

void GestorArbol::cambiarEstrategia(std::unique_ptr<EstrategiaEjecucion> nuevaEstrategia) {
    if (!nuevaEstrategia) {
        throw std::invalid_argument("La nueva estrategia no puede ser nula");
    }

    auto nuevoArbol = nuevaEstrategia->crearArbol();
    for (const std::string& palabra : arbol->listarTodas()) {
        nuevoArbol->insertar(palabra);
    }

    arbol = std::move(nuevoArbol);
    estrategia = std::move(nuevaEstrategia);
    notificar({TipoEventoArbol::EstrategiaCambiada, "", estrategia->nombre()});
}

std::string GestorArbol::estrategiaActual() const {
    return estrategia->nombre();
}

void GestorArbol::agregarObservador(ObservadorArbol* observador) {
    if (observador &&
        std::find(observadores.begin(), observadores.end(), observador) == observadores.end()) {
        observadores.push_back(observador);
    }
}

void GestorArbol::quitarObservador(ObservadorArbol* observador) {
    observadores.erase(
        std::remove(observadores.begin(), observadores.end(), observador),
        observadores.end()
    );
}

void GestorArbol::notificar(const EventoArbol& evento) {
    const auto copia = observadores;
    for (ObservadorArbol* observador : copia) {
        if (observador &&
            std::find(observadores.begin(), observadores.end(), observador) != observadores.end()) {
            observador->alCambiar(evento);
        }
    }
}
