#ifndef VISTAANIMACIONES_H_
#define VISTAANIMACIONES_H_
#include "VistaAnimacion.h"

class VistaAnimaciones {

	map<std::string, VistaAnimacion* > animaciones;
	std::vector<VistaAnimacion*> vAnimaciones;
	bool animacionesAutomaticas;

public:
	VistaAnimaciones();
	void setAnimacionesAutomaticas();
	void setPantalla(SDL_Surface* screen);
	void agregar(std::string nombreAnimacion, list<std::string> sprites, int periodo, double ancho, double alto, int fps);

	VistaAnimacion* get(std::string nombreAnimacion);
	VistaAnimacion* get(unsigned int indice);

	int getTamanio();

	virtual ~VistaAnimaciones();
};

#endif /* ANIMACIONES_H_ */
