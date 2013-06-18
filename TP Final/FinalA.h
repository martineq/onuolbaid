#pragma once

#include <iostream>
#include <sstream>
#include "ESArchivoCpp.h"

class FinalA{

	private:

		struct stDatos{

			// Datos a obtener a partir del archivo. Los tipos de los datos est�n dados en el enunciado.
			int anchoPix;
			int altoPix;
			unsigned int tamanioArchivo;
			unsigned int offsetDatos;
			unsigned int anchoFila;			// Valor en bytes. Ya incluye el padding.
			unsigned int cantidadFilas;
			unsigned int paddingAplicado;
			unsigned int bytesPorPixel;
			unsigned char* arrayPixeles;

			// Resultados de duplicar
			unsigned char* arrayPixelesDuplicados;
			unsigned int anchoFilaDoble;			// Valor en bytes. Ya incluye el padding.

			// Resultados de encontrar el color de transparencia
			unsigned char transparenteR;
			unsigned char transparenteG;
			unsigned char transparenteB;

			// Resultados de transparentar
			unsigned char* arrayPixelesTransparentados;

		};

		void leerArchivo(stDatos &datos);
		void duplicarTamanioImagen(stDatos &datos);
		void calcularColorTransparente(stDatos &datos);
		void reemplazarColorTransparente(stDatos &datos);
		void guardarArchivo(stDatos &datos);

	public:
		FinalA(void);
		~FinalA(void);

		void resolver(void);
		void pruebaPintaVerde(void);

};