#include "../include/GramaticaRegular.h"

namespace CyA {

GramaticaRegular::GramaticaRegular(std::string& rutaFichero) {

  leerFichero(rutaFichero);

}

void GramaticaRegular::leerFichero(std::string& rutaFichero) {

  std::ifstream fichero(rutaFichero);
  std::set<std::string> simbolosNoTerminales;

  if (fichero.is_open()) {
    unsigned int numSimbolos;
    std::string simbolo, reglas;

    fichero >> numSimbolos;

    for (unsigned int i = 0; i < numSimbolos; ++i) {
      fichero >> simbolo;
      simbolosTerminales_.insert(simbolo);
    }

    fichero >> numSimbolos;

    for (unsigned int i = 0; i < numSimbolos; ++i) {
      fichero >> simbolo;
      simbolosNoTerminales.insert(simbolo);
    }

    for (unsigned int i = 0; i < numSimbolos; ++i) {
      fichero >> simbolo;
      // std::cout << simbolo << ": " << simboloNoTerminal << '\n';
      // Comprobamos si el simbolo no terminal se encuentra en el conjunto ya procesado
      if (simbolosNoTerminales.count(simbolo)) {
        fichero >> reglas; // Nos saltamos el "->"
        fichero >> reglas;
        simbolosNoTerminales_.insert(SimboloNoTerminal(simbolo[0], i, reglas));
      }
    }
  }
  fichero.close();

}

void GramaticaRegular::simplificar() {

  eliminarProduccionesVacias();

}

void GramaticaRegular::imprimir(std::ostream& salida) const {
  salida << simbolosTerminales_.size() << '\n';

  for (auto& simbolo : simbolosTerminales_) {
    salida << simbolo << ' ';
  }

  salida << '\n';

  salida << simbolosNoTerminales_.size() << '\n';

  for (auto& simbolo : simbolosNoTerminales_) {
    salida << simbolo.getSimbolo() << ' ';
  }

  salida << '\n';

  for (auto& simbolo : simbolosNoTerminales_) {
    salida << simbolo.getSimbolo() << " -> ";
    salida << simbolo;
    salida << '\n';
  }


}

void GramaticaRegular::exportarGramatica(std::string& rutaFichero) {
  std::ofstream fichero(rutaFichero);
  fichero << (*this);
  fichero.close();
}

void GramaticaRegular::eliminarProduccionesVacias() {

  // Calcular variables anulables
  std::set<SimboloNoTerminal> simbolosAnulables, oldSimbolosAnulables;

  for (auto& simboloNoTerminal : simbolosNoTerminales_) {
    if (simboloNoTerminal.contieneCadenaVacia()) {
      simbolosAnulables = unir(simbolosAnulables, std::set<SimboloNoTerminal>({ simboloNoTerminal }));
    }
  }

  do {
    oldSimbolosAnulables = simbolosAnulables;

    for (auto& simboloNoTerminal: simbolosNoTerminales_) {
      for (auto& regla : (std::set<std::string>)simboloNoTerminal) {
        for (auto& simboloAnulable : oldSimbolosAnulables) {
          if (regla.find(simboloAnulable.getSimbolo()) != std::string::npos) {
            simbolosAnulables = unir(simbolosAnulables, std::set<SimboloNoTerminal>({ simboloNoTerminal }));
          }
        }
      }
    }
  } while(oldSimbolosAnulables != simbolosAnulables);

  // Eliminar producciones vacías
  std::set<SimboloNoTerminal> auxNoTerminales;

  for (auto simboloNoTerminal : simbolosNoTerminales_) {

    for (auto& simboloAnulable : simbolosAnulables) {

      std::set<std::string> conjuntoNoTerminales = simboloNoTerminal.getReglas();
      for (auto& regla : conjuntoNoTerminales) {

        if (regla == std::string(1, simboloAnulable.getSimbolo()) || regla == "~") {
          simboloNoTerminal.eraseRegla(regla);
        }

      }
    }
    if (!simboloNoTerminal.isEmpty()) {
      auxNoTerminales.insert(simboloNoTerminal);
    }
  }

  simbolosNoTerminales_ = auxNoTerminales;
  auxNoTerminales.clear();

  for (auto simboloNoTerminal : simbolosNoTerminales_) {

    for (auto& simboloAnulable : simbolosAnulables) {

      std::set<std::string> conjuntoNoTerminales = simboloNoTerminal.getReglas();
      for (auto& regla : conjuntoNoTerminales) {

        if (regla.find(simboloAnulable.getSimbolo()) != std::string::npos) {
          std::string reglaAux = regla;
          reglaAux.erase(std::remove(reglaAux.begin(), reglaAux.end(), simboloAnulable.getSimbolo()), reglaAux.end());
          if (!reglaAux.empty()) {
            simboloNoTerminal.insertRegla(reglaAux);
          } else {
            simboloNoTerminal.insertRegla("~");
          }
        }

      }
    }
    if (!simboloNoTerminal.isEmpty()) {
      auxNoTerminales.insert(simboloNoTerminal);
    }
  }

  // Sustituimos el conjunto generador por el actual
  simbolosNoTerminales_ = auxNoTerminales;

}

void GramaticaRegular::eliminarProduccionesUnitarias() {

  std::set<std::set<SimboloNoTerminal>> simbolosProduccionesUnitarias, oldSimbolosProduccionesUnitarias;

  for (auto& s1 : simbolosNoTerminales_) {
    for (auto& s2 : simbolosNoTerminales_) {
      if (s1.getSimbolo() != s2.getSimbolo()) {
        auto reglasDeProduccion = s1.getReglas();
        for (auto& regla : reglasDeProduccion) {
          if (regla.find(s2.getSimbolo()) != std::string::npos) {
            std::set<SimboloNoTerminal> aux;
            aux.insert(s1); aux.insert(s2);

            simbolosProduccionesUnitarias.insert(aux);
          }
        }
      }
    }
  }

  do {

    oldSimbolosProduccionesUnitarias = simbolosProduccionesUnitarias;

    for (auto& s1 : oldSimbolosProduccionesUnitarias) {
      for (auto& s2 : oldSimbolosProduccionesUnitarias) {
        if (s1 != s2) {
          std::set<SimboloNoTerminal> aux;
          aux.insert((*s1.begin())); aux.insert((*s2.rbegin()));
          if (oldSimbolosProduccionesUnitarias.find(aux) == oldSimbolosProduccionesUnitarias.end() &&
              (*s1.begin()).getSimbolo() != (*s2.rbegin()).getSimbolo()) {
            simbolosProduccionesUnitarias.insert(aux);
          }
        }
      }
    }

  } while (oldSimbolosProduccionesUnitarias != simbolosProduccionesUnitarias);

  // Eliminar producciones unitarias
  std::set<SimboloNoTerminal> auxNoTerminales;

  for (auto simboloNoTerminal : simbolosNoTerminales_) {
    std::set<std::string> conjuntoReglas = simboloNoTerminal.getReglas();

    for (auto& regla : conjuntoReglas) {
      for (auto& simbolo : simbolosNoTerminales_) {
        if (regla == std::string(1, simbolo.getSimbolo())) {
          simboloNoTerminal.eraseRegla(regla);
        }
      }
    }
    if (!simboloNoTerminal.isEmpty()) {
      auxNoTerminales.insert(simboloNoTerminal);
    }
  }

  simbolosNoTerminales_ = auxNoTerminales;
  auxNoTerminales.clear();

  // Añadimos producciones existentes
  for (auto simboloNoTerminal : simbolosNoTerminales_) {

    for (auto& pareja : simbolosProduccionesUnitarias) {
      SimboloNoTerminal primerSimbolo = (*pareja.begin()), segundoSimbolo = (*pareja.rbegin());

      if (simboloNoTerminal.getSimbolo() == primerSimbolo.getSimbolo()) {
        std::set<std::string> reglasSegundoSimbolo = segundoSimbolo.getReglas();
        for (auto& regla : reglasSegundoSimbolo) {
          for (auto& simboloTerminal : simbolosTerminales_) {
            if (regla.find(simboloTerminal) != std::string::npos) {
              simboloNoTerminal.insertRegla(regla);
            }
          }
        }
      }

    }

    if (!simboloNoTerminal.isEmpty()) {
      auxNoTerminales.insert(simboloNoTerminal);
    }
  }

  simbolosNoTerminales_ = auxNoTerminales;

}

std::set<SimboloNoTerminal> GramaticaRegular::unir(std::set<SimboloNoTerminal> s1, std::set<SimboloNoTerminal> s2) {
  for (auto& simbolo : s2) {
    s1.insert(simbolo);
  }
  return s1;
}

bool GramaticaRegular::simboloExiste(char simbolo) {
  for (auto& simboloNoTerminal : simbolosNoTerminales_) {
    if (simboloNoTerminal.getSimbolo() == simbolo) {
      return true;
    }
  }
  return false;
}

}

std::ostream& operator<<(std::ostream& salida, const CyA::GramaticaRegular& g) {
  g.imprimir(salida);
  return salida;
}
