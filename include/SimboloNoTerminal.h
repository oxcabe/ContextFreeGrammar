#ifndef SIMBOLONOTERMINAL_H
#define SIMBOLONOTERMINAL_H

#include <iostream>
#include <sstream>
#include <set>

namespace CyA {


class SimboloNoTerminal {
private:
  char simbolo_;
  unsigned int indice_;
  std::set<std::string> reglasDeProduccion_;
  void separarReglas(std::string& reglas);

public:
  SimboloNoTerminal() = default;
  SimboloNoTerminal(char simbolo);
  SimboloNoTerminal(char simbolo, unsigned int indice) : simbolo_(simbolo), indice_(indice), reglasDeProduccion_() {}
  SimboloNoTerminal(char simbolo, unsigned int indice, std::string& reglas) : simbolo_(simbolo), indice_(indice) { separarReglas(reglas); }
  ~SimboloNoTerminal() = default;

  char getSimbolo() const;
  std::set<std::string> getReglas() const;
  void insertRegla(std::string regla);
  void eraseRegla(std::string regla);

  bool contieneCadenaVacia() const;
  bool isEmpty() const;
  unsigned int getSize() const;

  operator std::set<std::string>() const { return reglasDeProduccion_; }

  bool operator<(const SimboloNoTerminal& s) const;
  bool operator==(const SimboloNoTerminal& s) const;

  void imprimir(std::ostream& salida) const;

};
}

std::ostream& operator<<(std::ostream& salida, const CyA::SimboloNoTerminal& s);

#endif // SIMBOLONOTERMINAL_H
