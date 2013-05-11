#include "ProxyControladorEvento.h"

ProxyControladorEvento::ProxyControladorEvento(void){
	this->pCliente = NULL;
	this->pServidor = NULL;
}

ProxyControladorEvento::~ProxyControladorEvento(void){

}

void ProxyControladorEvento::setSocketCliente(SocketCliente* pCliente){
	this->pCliente = pCliente;
}

void ProxyControladorEvento::setSocketServidor(SocketServidor* pServidor){
	this->pServidor = pServidor;
}


// Env�o el evento a trav�s del socket
// Devuelve true lo pudo enviar exitosamente. Devuelve false si hubo error al enviar 
bool ProxyControladorEvento::enviarEvento(ProxyControladorEvento::stEvento entidad){
	// >>> Implementar
	// Ac� uso this->pCliente;
	return true;
}

// Devuelve true si sac� datos de la cola de sockets. Devuelve false si la cola estaba vac�a
// Para ver si hubo errores al recibir del socket debo ver ProxyModeloEntidad::stEntidad.errorEnSocket, y ver si es true
bool ProxyControladorEvento::recibirEvento(ProxyControladorEvento::stEvento& evento){
	// >>> Implementar
	// Ac� uso this->pServidor;
	return true;
}

void ProxyControladorEvento::cargarStEvento(ProxyControladorEvento::stEvento& evento,int id,bool errorEnSocket,int mouseX,int mouseY,bool teclaA,bool teclaS,bool mouseClickIzquierdo,bool mouseDentroPantalla,bool finalizoElJuego){
	evento.errorEnSocket = errorEnSocket;
	evento.finalizoElJuego = finalizoElJuego;
	evento.id = id;
	evento.mouseClickIzquierdo = mouseClickIzquierdo;
	evento.mouseDentroPantalla = mouseDentroPantalla;
	evento.mouseX = mouseX;
	evento.mouseY = mouseY;
	evento.teclaA = teclaA;
	evento.teclaS = teclaS;
}

// TODO: Implementar los m�todos del proxy. Ver si hace falta agregar al struct mas variables necesarias 