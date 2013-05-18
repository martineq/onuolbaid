#include "ModeloEntidad.h"

using namespace std;

ModeloEntidad* ModeloEntidad::ModeloMovimiento::detectarColision(Posicion posicion) {
	this->_mutexJugadores->lockLectura(__FILE__, __LINE__);
	list<ModeloEntidad*>* listaJugadores = this->_jugadores;
	this->_mutexJugadores->unlock(__FILE__, __LINE__);

	// Detecto colision con jugadores
	if (listaJugadores != NULL) {
		list<ModeloEntidad*>::iterator iterador = listaJugadores->begin();

		while (iterador != listaJugadores->end()) {
			if (((*iterador) != this->_modeloEntidad) && (*iterador)->ocupaPosicion(posicion))
				return *iterador;
			iterador++;
		}
	}

	this->_mutexEntidades->lockLectura(__FILE__, __LINE__);
	multimap<std::pair<int, int>, ModeloEntidad*>::iterator entidad = this->_entidades->find(make_pair(posicion.x, posicion.y));
	multimap<std::pair<int, int>, ModeloEntidad*>::iterator fin = this->_entidades->end();
	this->_mutexEntidades->unlock(__FILE__, __LINE__);
	
	return (entidad == fin) ? NULL : (*entidad).second;
}

Direccion ModeloEntidad::ModeloMovimiento::obtenerDireccion(Posicion posicionOrigen, Posicion posicionDestino) {
	if (posicionOrigen.x > posicionDestino.x) {
		if (posicionOrigen.y > posicionDestino.y)
			return NOROESTE;
		else if (posicionOrigen.y < posicionDestino.y)
			return SUDOESTE;
		else
			return OESTE;
	}
	else if (posicionOrigen.x < posicionDestino.x)
		if (posicionOrigen.y > posicionDestino.y)
			return NORESTE;
		else if (posicionOrigen.y < posicionDestino.y)
			return SUDESTE;
		else
			return ESTE;
	else {
		if (posicionOrigen.y > posicionDestino.y)
			return NORTE;
		else if (posicionOrigen.y < posicionDestino.y)
			return SUR;
		else
			return SUR;
	}
}

Posicion ModeloEntidad::ModeloMovimiento::obtenerPosicionSiguiente() {
	Posicion posicionSiguiente = this->_posiciones.front();
	this->_posiciones.pop_front();
	return posicionSiguiente;
}

ModeloEntidad::ModeloMovimiento::ModeloMovimiento(const ModeloMovimiento &modeloMovimiento) {
}

ModeloEntidad::ModeloMovimiento& ModeloEntidad::ModeloMovimiento::operator=(const ModeloMovimiento &modeloMovimiento) {
	return *this;
}

ModeloEntidad::ModeloMovimiento::ModeloMovimiento(int altoNivel, int anchoNivel, ModeloEntidad* modeloEntidad) {
	this->_altoNivel = altoNivel;
	this->_anchoNivel = anchoNivel;
	this->_modeloEntidad = modeloEntidad;
	this->_jugadores = NULL;
	this->_entidades = NULL;
	this->_instanteUltimoCambioEstado = 0;
}

ModeloEntidad::ModeloMovimiento::~ModeloMovimiento() {
}

bool ModeloEntidad::ModeloMovimiento::agregarTile(list<Tile>* tilesAbiertos, list<Tile>* tilesCerrados, Posicion posicion, Posicion posicionDestino, Tile* padre, int distancia) {
	// Si la posicion esta fuera del nivel no la proceso
	if ((posicion.x < 0) || (posicion.x >= this->_anchoNivel) || (posicion.y < 0) || (posicion.y >= this->_anchoNivel))
		return false;
	
	// Si encuentro el tile en los tiles cerrados lo lo agrego
	for (list<Tile>::iterator tile = tilesCerrados->begin(); tile != tilesCerrados->end(); tile++) {
		if ((*tile).posicion() == posicion)
			return false;
	}

	// Si encuentro el tile en los abiertos le cambio el padre
	for (list<Tile>::iterator tile = tilesAbiertos->begin(); tile != tilesAbiertos->end(); tile++) {
		if ((*tile).posicion() == posicion) {
			(*tile).padre(padre, distancia);
			return true;
		}
	}

	// Si la posicion esta ocupada no la agrego
	if (this->detectarColision(posicion) != NULL)
		return false;

	// Si el tile no esta en ninguna de las dos listas lo agrego
	Tile tile;
	tile.padre(padre, distancia);
	tile.posicion(posicion);
	tile.posicionDestino(posicionDestino);
	tilesAbiertos->push_back(tile);

	return true;
}

void ModeloEntidad::ModeloMovimiento::actualizar(Posicion posicionDestino) {
	// Dentego el movimiento anterior
	this->detener();

	// Si la posicion actual es igual a la destino, o si en la destino hay un obstaculo no ejectu el movimiento
	if (this->_modeloEntidad->posicionActual() == posicionDestino)
		return;

	list<Tile> tilesAbiertos;
	list<Tile> tilesCerrados;
	Tile* tileActual = NULL;

	this->agregarTile(&tilesAbiertos, &tilesCerrados, this->_modeloEntidad->posicionActual(), posicionDestino, NULL, 0);
	
	while (!tilesAbiertos.empty()) {
		// Paso el primer tile abierto a la lista de cerrados
		tilesCerrados.push_back(tilesAbiertos.front());
		tilesAbiertos.pop_front();
		tileActual = &tilesCerrados.back();

		// Si llegue a la posicion destino reconstruyo el camino
		if (tileActual->posicion() == posicionDestino) {
			while (tileActual != NULL) {
				this->_posiciones.push_front(tileActual->posicion());
				tileActual = tileActual->padre();
			}

			// Elimino la primera posicion porque ya esta dibujada
			this->_posiciones.pop_front();
			return;
		}

		Posicion posicionAdyacente;
		bool posicionProcesada = false;

		// Posicion superior izquierda
		posicionAdyacente = tileActual->posicion();
		posicionAdyacente.x -= 1;
		posicionAdyacente.y -= 1;
		posicionProcesada |= this->agregarTile(&tilesAbiertos, &tilesCerrados, posicionAdyacente, posicionDestino, tileActual, 14);

		// Posicion superior
		posicionAdyacente = tileActual->posicion();
		posicionAdyacente.y -= 1;
		posicionProcesada |= posicionProcesada |= this->agregarTile(&tilesAbiertos, &tilesCerrados, posicionAdyacente, posicionDestino, tileActual, 10);

		// Posicion superior derecha
		posicionAdyacente = tileActual->posicion();
		posicionAdyacente.x += 1;
		posicionAdyacente.y -= 1;
		posicionProcesada |= this->agregarTile(&tilesAbiertos, &tilesCerrados, posicionAdyacente, posicionDestino, tileActual, 14);

		// Posicion izquierda
		posicionAdyacente = tileActual->posicion();
		posicionAdyacente.x -= 1;
		posicionProcesada |= posicionProcesada |= this->agregarTile(&tilesAbiertos, &tilesCerrados, posicionAdyacente, posicionDestino, tileActual, 10);

		// Posicion derecha
		posicionAdyacente = tileActual->posicion();
		posicionAdyacente.x += 1;
		posicionProcesada |= posicionProcesada |= this->agregarTile(&tilesAbiertos, &tilesCerrados, posicionAdyacente, posicionDestino, tileActual, 10);

		// Posicion inferior izquierda
		posicionAdyacente = tileActual->posicion();
		posicionAdyacente.x -= 1;
		posicionAdyacente.y += 1;
		posicionProcesada |= this->agregarTile(&tilesAbiertos, &tilesCerrados, posicionAdyacente, posicionDestino, tileActual, 14);

		// Posicion inferior
		posicionAdyacente = tileActual->posicion();
		posicionAdyacente.y += 1;
		posicionProcesada |= posicionProcesada |= this->agregarTile(&tilesAbiertos, &tilesCerrados, posicionAdyacente, posicionDestino, tileActual, 10);

		// Posicion inferior derecha
		posicionAdyacente = tileActual->posicion();
		posicionAdyacente.x += 1;
		posicionAdyacente.y += 1;
		posicionProcesada |= this->agregarTile(&tilesAbiertos, &tilesCerrados, posicionAdyacente, posicionDestino, tileActual, 14);

		// Ordeno la lista si se prceso alguna posicion
		if (posicionProcesada)
			tilesAbiertos.sort();
	}
}

void ModeloEntidad::ModeloMovimiento::asignarJugadores(Mutex* mutexJugadores, std::list<ModeloEntidad*>* jugadores) {
	this->_mutexJugadores = mutexJugadores;
	this->_jugadores = jugadores;
}

void ModeloEntidad::ModeloMovimiento::asignarEntidades(Mutex* mutexEntidades, multimap<std::pair<int, int>, ModeloEntidad*>* entidades) {
	this->_mutexEntidades = mutexEntidades;
	this->_entidades = entidades;
}

void ModeloEntidad::ModeloMovimiento::cambiarEstado() {
	if (this->_posiciones.empty())
		return;
	
	if (this->_instanteUltimoCambioEstado == 0) {
		this->_instanteUltimoCambioEstado = GetTickCount();
		return;
	}

	if (this->_modeloEntidad->velocidad() > (GetTickCount() - this->_instanteUltimoCambioEstado))
		return;

	// Obtengo la siguente posicion del movimiento actual
	Posicion posicionSiguiente = this->obtenerPosicionSiguiente();
	this->_modeloEntidad->direccion(this->obtenerDireccion(this->_modeloEntidad->posicionActual(), posicionSiguiente));
	
	// Si me choque con algo en el camino me dentego
	if (this->detectarColision(posicionSiguiente) != NULL)
		this->detener();

	// Notifico a VistaMovimiento
	this->_modeloEntidad->posicionSiguiente(posicionSiguiente);
	this->notificarObservadores();
	this->_modeloEntidad->posicionActual(this->_modeloEntidad->posicionSiguiente());

	if (this->_posiciones.empty())
		this->_modeloEntidad->accion(QUIETO);

	this->_instanteUltimoCambioEstado = GetTickCount();
}

void ModeloEntidad::ModeloMovimiento::detener() {
	this->_posiciones.clear();
	this->_modeloEntidad->accion(QUIETO);
}