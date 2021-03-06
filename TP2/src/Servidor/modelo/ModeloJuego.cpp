#include "ModeloJuego.h"

ModeloJuego::ModeloJuego() {
	this->pHiloReceptor = NULL;
}

ModeloJuego::~ModeloJuego() {

}

bool ModeloJuego::iniciarEscenario(SocketServidor* pSocket){
	return this->_modeloFactory.crearNivel(this->_modeloNivel,this->_modeloLoop,pSocket);
}

// Inicio el hilo de recepci�n ac�
bool ModeloJuego::iniciarRecepcion(SocketServidor* pSocket){

	//Cargo los pr�metros necesarios para la configuraci�n del jugador
	HiloConfiguracion::stParametrosConfiguracion parametros;
	parametros.pModeloNivel = &(this->_modeloNivel);
	parametros.pModeloFactory = &(this->_modeloFactory);
	parametros.pServidor = pSocket;

	this->pHiloReceptor = new HiloReceptor();
	this->pHiloReceptor->correrRecepcion(parametros);

	return true;
}

void ModeloJuego::finalizarRecepcion(){
	if( this->pHiloReceptor != NULL ){
		this->pHiloReceptor->detenerActividad();
		this->pHiloReceptor->join();
		delete this->pHiloReceptor;
		this->pHiloReceptor = NULL;
	}
	return void();
}

bool ModeloJuego::loop() {
	return this->_modeloLoop.loop(this->_modeloNivel);
}

int ModeloJuego::cantidadJugadores(void){
	return this->_modeloNivel.cantidadJugadores();
}


void ModeloJuego::destruirEntidades(){
	this->_modeloNivel.destruirListas();
}

