#ifndef PATRONES_ARBOL_H
#define PATRONES_ARBOL_H

#include "Arbol.h"
#include <memory>
#include <string>
#include <vector>

class EstrategiaEjecucion {
public:
    virtual ~EstrategiaEjecucion() = default;
    virtual std::unique_ptr<Arbol> crearArbol() const = 0;
    virtual std::string nombre() const = 0;
};

class EstrategiaSecuencial : public EstrategiaEjecucion {
public:
    std::unique_ptr<Arbol> crearArbol() const override;
    std::string nombre() const override;
};

class EstrategiaParalela : public EstrategiaEjecucion {
public:
    std::unique_ptr<Arbol> crearArbol() const override;
    std::string nombre() const override;
};

// OBSERVER: eventos que el gestor comunica a sus observadores.
enum class TipoEventoArbol {
    Insertado,
    Eliminado,
    Limpiado,
    EstrategiaCambiada
};

struct EventoArbol {
    TipoEventoArbol tipo;
    std::string palabra;
    std::string estrategia;
};

class ObservadorArbol {
public:
    virtual ~ObservadorArbol() = default;
    virtual void alCambiar(const EventoArbol& evento) = 0;
};

class GestorArbol {
private:
    std::unique_ptr<EstrategiaEjecucion> estrategia;
    std::unique_ptr<Arbol> arbol;
    std::vector<ObservadorArbol*> observadores;

    void notificar(const EventoArbol& evento);

public:
    GestorArbol();
    explicit GestorArbol(std::unique_ptr<EstrategiaEjecucion> estrategiaInicial);

    bool insertar(const std::string& palabra);
    bool buscar(const std::string& palabra) const;
    bool eliminar(const std::string& palabra);
    bool existePrefijo(const std::string& prefijo) const;
    std::vector<std::string> buscarPorPrefijo(const std::string& prefijo) const;
    int contarPorPrefijo(const std::string& prefijo) const;
    std::vector<std::string> listarTodas() const;
    int tamanio() const;
    bool estaVacio() const;
    int cantidadNodos() const;
    void limpiar();
    std::string dibujar() const;

    void cambiarEstrategia(std::unique_ptr<EstrategiaEjecucion> nuevaEstrategia);
    std::string estrategiaActual() const;

    void agregarObservador(ObservadorArbol* observador);
    void quitarObservador(ObservadorArbol* observador);
};

#endif
