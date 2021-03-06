#pragma once

#include <iostream>
#include <string.h>

#include "../../utils/Constantes/Constantes.h"
#include "Posicion.h"

class EstadoNivel {
	private:
		int _alto;
		int _ancho;
		Posicion _posicion;
		int _rangoVision;
		char* _nivel;
		bool tieneMapa;

		EstadoNivel(const EstadoNivel &estadoNivel);

		EstadoNivel& operator=(const EstadoNivel &estadoNivel);

	public:
		EstadoNivel(int alto, int ancho, int x, int y, int rangoVision);

		virtual ~EstadoNivel();

		int rangoVision();

		void rangoVision(int rangoVision);

		void visitar(int x, int y);

		char visibilidad(int x, int y);

		void setMatriz(std::string);

		std::string getMatriz();

		int getAlto();

		int getAncho();

		void setTieneMapa(bool);

		bool getTieneMapa();
};
