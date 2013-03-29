#include "VistaNivel.h"


VistaNivel::VistaNivel(){

}

void VistaNivel::levantarPantalla() {
	// Estructura para la superficie gr�fica, donde se va a dibujar
	SDL_Surface *temp;

	// Inicio SDL
	SDL_Init(SDL_INIT_VIDEO);

	// Seteo el nombre en la barra
	SDL_WM_SetCaption("..::TP Taller pap�!!!::..", "");

	// Creo la ventana
	this->pantalla = SDL_SetVideoMode(PANTALLA_ANCHO, PANTALLA_ALTO, 0, SDL_DOUBLEBUF|SDL_HWSURFACE|SDL_ANYFORMAT);

	// Creo el fondo
	temp  = SDL_LoadBMP(SDL_RUTA_FONDO);
	this->fondo = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	this->font = TTF_OpenFont(DEFAULT_FONT, 36);
}

SDL_Surface* VistaNivel::getPantalla(){
	return this->pantalla;
}

SDL_Surface* VistaNivel::getFondo(){
	return this->fondo;
}

TTF_Font* VistaNivel::getFont(){
	return this->font;
}

SDL_Surface* VistaNivel::getTextSurface(){
	std::string texto;
	texto.assign("hola");						
	SDL_Color textColor;
	textColor.b = 0xFB;
	textColor.g = 0xEF;
	textColor.r = 0xEF;	
	this->textSurface = TTF_RenderText_Solid(font, texto.c_str(), textColor);
	SDL_Rect offset;
	offset.x = 100;
	offset.y = 100;
	return this->textSurface;
}

VistaNivel::~VistaNivel(){
	/*SDL_FreeSurface(pantalla);
	SDL_FreeSurface(fondo);
	SDL_FreeSurface(textSurface);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();	*/
}


Observador* VistaNivel::obtenerObservadorScroll(void){
	return (Observador*)this->vistaScroll;
}

std::list<Observador*> VistaNivel::obtenerObservadoresEntidad(void){

	// TODO: Ver como paso la lista �se borra?
	std::list<Observador*> listaObservadoresEntidad;

	for (std::list<VistaEntidad*>::iterator it=this->listaVistaEntidad.begin() ; it != this->listaVistaEntidad.end(); it++ ){
		listaObservadoresEntidad.push_back( ((Observador*)*it) );
	}

	return listaObservadoresEntidad;

}

