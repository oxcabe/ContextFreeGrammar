#include <iostream>
#include <string>
#include <getopt.h>
#include "include/GramaticaRegular.h"

void ayuda(char* nombrePrograma) {
	std::cout << "Uso: " << nombrePrograma << " [-h|--help] [-i|--input] gramatica.grammar [-o|--output] gramatica.grammar\n" <<
		"\t[-h]               Muestra este menu.\n" <<
		"\t[-i|--input]       Fichero con la gramática a introducir.\n" <<
		"\t[-o|--output]      Fichero en el que escribir la gramática resultante.\n";
}

void procesarArgumentos(int argc, char* argv[]) {

	bool mostrarAyuda = false;
	std::string ficheroInput, ficheroOutput;

	const char* const opcionesCortas = "hi:o:";
	const option opcionesLargas[] = {
		{"help", no_argument, nullptr, 'h'},
		{"input", required_argument, nullptr, 'i'},
		{"output", required_argument, nullptr, 'o'},
		{nullptr, no_argument, nullptr, 0}
	};

	while (true) {
		const auto opciones = getopt_long(argc, argv, opcionesCortas, opcionesLargas, nullptr);

		if (opciones == -1) {
			break;
		}

		switch (opciones) {
			case 'i': {
				ficheroInput = std::string(optarg);
				break;
			}
			case 'o': {
				ficheroOutput = std::string(optarg);
				break;
			}
			case '?': {
				std::cout << "Argumento inválido\n";
			}
			case 'h':
			default: {
				mostrarAyuda = true;
				break;
			}
		}
	}

	// Implementar lógica de argumentos aquí
	if (mostrarAyuda) {
		ayuda(argv[0]);
	} else {
		if (!ficheroInput.empty() && !ficheroOutput.empty()) {
			CyA::GramaticaRegular gramatica(ficheroInput);
			std::cout << gramatica;
			gramatica.simplificar();
			std::cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n" <<
									 "\\\\\\\\\\\\ SIMPLIFICADO \\\\\\\\\\\\\n" <<
								   "\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\n";
			std::cout << gramatica;
			gramatica.exportarGramatica(ficheroOutput);
		}
	}

}

int main(int argc, char* argv[]) {
	procesarArgumentos(argc, argv);
}
