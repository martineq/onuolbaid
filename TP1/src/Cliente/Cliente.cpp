#include "./Cliente.h"

Cliente::Cliente(void){

}

Cliente::~Cliente(void){

}

bool Cliente::iniciar(void){
	//instancia el nivel en vistaJuego
	return this->vistaJuego.iniciar();
}

Observador* Cliente::obtenerObservadorScroll(void){
	return this->vistaJuego.obtenerObservadorScroll();
}

Observador* Cliente::obtenerObservadorJugador(void){
	return this->vistaJuego.obtenerObservadorJugador();
}


std::list<Observador*> Cliente::obtenerObservadoresEntidad(void){
	return this->vistaJuego.obtenerObservadoresEntidad();
}

void Cliente::agregarObservadorEventos(Observador* observador){
	this->controladorJuego.agregarObservadorEventos(observador);
	return void();
}


bool Cliente::loopControl(void){
	return this->controladorJuego.loop();
}

bool Cliente::loopVista(void){
	return this->vistaJuego.loop();
}


void Cliente::destruirEntidades(void){
	return this->vistaJuego.destruirEntidades();
}


