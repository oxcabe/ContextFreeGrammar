#include "../include/SimboloNoTerminal.h"

namespace CyA {

SimboloNoTerminal::SimboloNoTerminal(char simbolo) : simbolo_(simbolo), reglasDeProduccion_() {
  indice_ = 1;
}

char SimboloNoTerminal::getSimbolo() const {
  return simbolo_;
}

std::set<std::string> SimboloNoTerminal::getReglas() const {
  return reglasDeProduccion_;
}

void SimboloNoTerminal::insertRegla(std::string regla) {
  reglasDeProduccion_.insert(regla);
}

void SimboloNoTerminal::eraseRegla(std::string regla) {
  reglasDeProduccion_.erase(regla);
}

bool SimboloNoTerminal::contieneCadenaVacia() const {
  for (auto& regla : reglasDeProduccion_) {
    if (regla == std::string(1, '~')) { return true; }
  }
  return false;
}

bool SimboloNoTerminal::isEmpty() const {
  return reglasDeProduccion_.size() == 0;
}

unsigned int SimboloNoTerminal::getSize() const {
  return reglasDeProduccion_.size();
}

bool SimboloNoTerminal::operator<(const SimboloNoTerminal& s) const {
  return indice_ < s.indice_;
}
bool SimboloNoTerminal::operator==(const SimboloNoTerminal& s) const {
  return indice_ == s.indice_;
}

void SimboloNoTerminal::imprimir(std::ostream& salida) const {

  unsigned int comprobarFinal = 1;

  for (auto& regla : reglasDeProduccion_) {
    salida << regla;
    if (comprobarFinal != reglasDeProduccion_.size()) {
      salida << "|";
    }
    ++comprobarFinal;
  }

}

void SimboloNoTerminal::separarReglas(std::string& reglas) {
  std::istringstream reglasStream(reglas);
  std::string regla;
  while(std::getline(reglasStream, regla, '|')) {
    reglasDeProduccion_.insert(regla);
  }
}

}

std::ostream& operator<<(std::ostream& salida, const CyA::SimboloNoTerminal& s) {
  s.imprimir(salida);
  return salida;
}
