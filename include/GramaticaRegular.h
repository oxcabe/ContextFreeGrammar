#ifndef GRAMATICAREGULAR_H
#define GRAMATICAREGULAR_H

#include <iostream>
#include <set>
#include <map>
#include <fstream>
#include <algorithm>
#include "SimboloNoTerminal.h"

namespace CyA {

/**
 * \class GramaticaRegular
 *
 * \brief Simulación de Gramática Regular
 *
 * Una gramática regular es una gramática formal (N, Σ, P, S) que puede ser
 * clasificada como regular izquierda o regular derecha. Las gramáticas regulares
 * sólo pueden generar a los lenguajes regulares de manera similar a los autómatas
 * finitos y las expresiones regulares.
 *
 * \author Óscar
 *
 * \version $Revision: 1.1 $
 *
 *
 *
 */
class GramaticaRegular {
private:
  std::set<std::string> simbolosTerminales_;
  std::set<SimboloNoTerminal> simbolosNoTerminales_;

  void eliminarProduccionesVacias();
  void eliminarProduccionesUnitarias();
  std::set<SimboloNoTerminal> unir(std::set<SimboloNoTerminal> s1, std::set<SimboloNoTerminal> s2);
  bool simboloExiste(char simbolo);

public:
  GramaticaRegular() = default;
  GramaticaRegular(std::string& rutaFichero);

  ~GramaticaRegular() = default;

  void leerFichero(std::string& rutaFichero);

  void simplificar();
  /** \brief Introduce la gramática en un objeto ostream.
  * \param salida
  * Objeto ostream en el que almacenar la gramática.
  */
  void imprimir(std::ostream& salida) const;
  void exportarGramatica(std::string& rutaFichero);


};

}

std::ostream& operator<<(std::ostream& salida, const CyA::GramaticaRegular& g);

#endif // GRAMATICAREGULAR_H
