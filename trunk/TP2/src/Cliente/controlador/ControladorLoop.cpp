#include "ControladorLoop.h"

ControladorLoop::ControladorLoop() {
}

void ControladorLoop::loop(VistaChat* vistaChat, VistaNivel* nivel) {
	this->detector.detectar();
	this->evento.limpiar();

	// Capturo eventos para el scroll
	if (!this->detector.getDentroDePantalla())
		this->evento.getControladorScroll()->detener();
	else
		this->evento.getControladorScroll()->actualizar(detector.getPosicionMouseX(), detector.getPosicionMouseY());
	this->evento.getControladorScroll()->cambiarEstado();

	// Capturo eventos para el chat
	if (this->detector.getClicMouseBotonIzquierdo() == 1) {
		int x = 0, y = 0;
		VistaEntidad* jugador = NULL;
		std::list<VistaEntidad*> jugadores = nivel->getListaOtrosJugadores();
		std::list<VistaEntidad*>::iterator unJugador = jugadores.begin();
		
		Posicion::convertirPixelATile(nivel->getAltoDeNivelEnTiles(), this->evento.getPosicionMouseX(), this->evento.getPosicionMouseY(), x, y);

		while ((unJugador != jugadores.end()) && (jugador == NULL)) {
			if (((*unJugador) != nivel->getJugador()) && (x == (*unJugador)->getTileX()) && (y == (*unJugador)->getTileY()))
				jugador = *unJugador;
			unJugador++;
		}
		
		if (jugador != NULL) {
			vistaChat->visible(true);
			vistaChat->asignarDestinatario(jugador->getNombreEntidad());
		}
	}
	else if (this->detector.getEscape())
		vistaChat->visible(false);
	else if (this->detector.getRetroceso())
			vistaChat->borrarCaracter();
	else if (this->detector.getCaracter() != 0)
		vistaChat->agregarCaracter(this->detector.getCaracter());

	// Caputro eventos para el servidor
	this->evento.setPosicionMouseXY(this->detector.getPosicionMouseX(), this->detector.getPosicionMouseY());
	if (this->detector.getClicMouseBotonIzquierdo() == 1)
		this->evento.setClicMouseBotonIzquierdo(this->detector.getClicMouseBotonIzquierdo());
	else if (vistaChat->visible())
		return;
	else if (this->detector.getTeclaAApretada())
		this->evento.setTeclaAApretada(this->detector.getTeclaAApretada());
	else if (this->detector.getTeclaSApretada())
		this->evento.setTeclaSApretada(this->detector.getTeclaSApretada());
}

bool ControladorLoop::getQuit() {
	return this->detector.getQuit();
}

ControladorEvento* ControladorLoop::getControladorEvento() {
	return &this->evento;
}

ControladorLoop::~ControladorLoop() {
}