#ifndef ARBOL_H
#define ARBOL_H

#include <string>
#include <vector>

/**
 * Contrato común del árbol de strings (clase abstracta).
 *
 * Todo el equipo programa contra esta interfaz:
 *  - Integrante 2 (paralelismo) la usa para búsqueda/inserción concurrente.
 *  - Integrantes 3 y 4 (patrones) la usan en Factory, Singleton, Strategy y Observer.
 *  - Integrante 5 (GUI) la usa para mostrar y operar el árbol.
 */
class Arbol {
public:
    virtual ~Arbol() = default;

    /** Inserta la palabra. Devuelve true si era nueva, false si ya existía. */
    virtual bool insertar(const std::string& palabra) = 0;

    /** Devuelve true si la palabra exacta está en el árbol. */
    virtual bool buscar(const std::string& palabra) const = 0;

    /** Elimina la palabra. Devuelve true si existía y se eliminó. */
    virtual bool eliminar(const std::string& palabra) = 0;

    /** Devuelve true si alguna palabra del árbol empieza con el prefijo. */
    virtual bool existePrefijo(const std::string& prefijo) const = 0;

    /** Todas las palabras que empiezan con el prefijo, en orden alfabético. */
    virtual std::vector<std::string> buscarPorPrefijo(const std::string& prefijo) const = 0;

    /** Cuántas palabras empiezan con el prefijo (O(longitud del prefijo)). */
    virtual int contarPorPrefijo(const std::string& prefijo) const = 0;

    /** Todas las palabras del árbol, en orden alfabético (recorrido en profundidad). */
    virtual std::vector<std::string> listarTodas() const = 0;

    /** Número de palabras almacenadas. */
    virtual int tamanio() const = 0;

    virtual bool estaVacio() const = 0;

    /** Número de nodos del árbol (sirve para comparar memoria en la exposición). */
    virtual int cantidadNodos() const = 0;

    /** Elimina todas las palabras. */
    virtual void limpiar() = 0;

    /** Representación del árbol en texto, para depurar o mostrar en la GUI. */
    virtual std::string dibujar() const = 0;
};

#endif
