#pragma once

#include <fstream>
#include <string>

class ESArchivoCpp{

	std::fstream archivo;

	public:
		ESArchivoCpp(const char* ruta, bool esEntrada, bool esTexto);
		ESArchivoCpp(void);
		~ESArchivoCpp(void);

		void abrirArchivo(const char* ruta, bool esEntrada, bool esTexto);

		void leerArchivo(char* cadena, unsigned long offset, unsigned long tamanio);
		unsigned long leerArchivo(char** cadena);
		bool leerLinea(std::string& linea);

		void escribirEnArchivo(const char* cadena,  unsigned long offset, unsigned long tamanio);
		void escribirEnArchivo(const char* cadena, unsigned long tamanio);
		void escribirLinea(std::string linea);

};
