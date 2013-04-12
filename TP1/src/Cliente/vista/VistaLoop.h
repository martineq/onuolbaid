#pragma once

#include <SDL.h>
#include "./VistaNivel.h"
#include "ImageLoader.h"


class VistaLoop{
	private:

		SDL_Surface *pantalla,*fondo;			
		void dibujarPantalla(VistaNivel& vistaNivel);			

	public:
		VistaLoop(void);
		~VistaLoop(void);						
		bool loop(VistaNivel& vistaNivel);

		//levanta el fondo y la pantalla
		bool levantarPantalla(int,int);
		void setPantalla(SDL_Surface *pantalla);
		
};

// TODO: Ver implementaci�n de los m�todos para iniciar el sdl y para dibujar. Seguramente vn todo en el loop