#include "ModeloLoop.h"

ModeloLoop::ModeloLoop(){

}

ModeloLoop::~ModeloLoop(){

}

bool ModeloLoop::loop(ModeloNivel& modeloNivel){

	this->procesarJugadoresEliminados(modeloNivel);

	// Recorro varios eventos en un solo loop. Est� asegurado que no habr� 2 eventos del mismo ID.
	while( this->_modeloEvento.getActualizado() ){
		int idJugador = this->_modeloEvento.getIdJugador();
		int mousePosX = this->_modeloEvento.getMousePosX();
		int mousePosY = this->_modeloEvento.getMousePosY();

		if (this->_modeloEvento.getMouseClickIzquierdo()){
			modeloNivel.moverJugador(mousePosX,mousePosY,idJugador); // Busca por ID el jugador a mover
		}
		
		this->_modeloEvento.cargarProximoEvento();  // Con esto descarto el evento que acabo de leer y carga el pr�ximo

		modeloNivel.actualizar(idJugador); // TODO: Ver si esta l�nea va ac�, o si va afuera del while, y de esa forma se debe actualizar a todos los jugadores existentes Preguntar Mati
	}

	return true;
}

void ModeloLoop::setProxyControladorEvento(ProxyControladorEvento* pProxyEvento){
	this->_modeloEvento.setProxyControladorEvento(pProxyEvento);
	return void();
}

void ModeloLoop::procesarJugadoresEliminados(ModeloNivel& modeloNivel){
	// TODO: Implementar. La idea es tomar todos los jugadores que tienen error de socket y marcarlos como congelados en el modelo nivel, restando la cantidad de jugadores conectados
	return void();
}