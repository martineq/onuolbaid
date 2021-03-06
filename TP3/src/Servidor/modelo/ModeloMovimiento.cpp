#include "ModeloMovimiento.h"

using namespace std;

bool ModeloMovimiento::agregarTile(char* mapaTilesCerrados, list<Tile>* tilesAbiertos, Posicion posicion, Posicion posicionDestino, Tile* padre, int distancia) {
	Tile tile;
	tile.padre(padre, distancia);
	tile.posicion(posicion);
	tile.posicionDestino(posicionDestino);
	
	// Si la posicion esta fuera del nivel no la proceso
	if ((posicion.x < 0) || (posicion.x >= this->_anchoNivel) || (posicion.y < 0) || (posicion.y >= this->_anchoNivel))
		return false;
	
	// Si ya cerre el tile salgo
	if (mapaTilesCerrados[(this->_anchoNivel * posicion.y) + posicion.x] == 1)
		return false;

	// Si la posicion esta ocupada no la agrego
	if (this->_listaEntidades->detectarColision(this->_modeloEntidad, posicion) != NULL)
		return false;

	// Si encuentro el tile en los abiertos le cambio el padre
	for (list<Tile>::iterator tile = tilesAbiertos->begin(); tile != tilesAbiertos->end(); tile++) {
		if ((*tile).posicion() == posicion) {
			(*tile).padre(padre, distancia);
			return true;
		}
	}

	// Si el tile no esta en ninguna de las dos listas lo agrego y lo marco como visitado
	tilesAbiertos->push_back(tile);

	return true;
}

Posicion ModeloMovimiento::calcularPosicionDestino(Posicion posicionDestino) {
	// Obtengo la entidad en la posicion destino
	ModeloEntidad* modeloEntidad = this->_listaEntidades->detectarColision(this->_modeloEntidad, posicionDestino);

	// Si no choca con nada devuelvo la misma posicion
	if (modeloEntidad == NULL)
		return posicionDestino;

	// Obtengo la posicion y dimension de todas las entidades adyacentes
	int x = this->obtenerX(modeloEntidad);
	int y = this->obtenerY(modeloEntidad);
	int alto = this->obtenerAlto(y, modeloEntidad);
	int ancho = this->obtenerAncho(x, modeloEntidad);
	
	// Choco con cara norte
	if (this->_modeloEntidad->posicion().y < y)
		posicionDestino.y = y - 1;
	// Choco con cara sur
	else if (this->_modeloEntidad->posicion().y >= y + alto)
		posicionDestino.y = y + alto;
	
	// Choco con cara este
	if (this->_modeloEntidad->posicion().x >= x + ancho)
		posicionDestino.x = x + ancho;
	// Choco con cara oeste
	else if (this->_modeloEntidad->posicion().x < x)
		posicionDestino.x = x - 1;

	return posicionDestino;
}

int ModeloMovimiento::obtenerAlto(int y, ModeloEntidad* modeloEntidad) {
	int alto = modeloEntidad->posicion().y - y;
	while (modeloEntidad != NULL) {
		alto += modeloEntidad->alto();
		Posicion posicion = modeloEntidad->posicion();
		posicion.y += modeloEntidad->alto();
		modeloEntidad = this->_listaEntidades->detectarColision(this->_modeloEntidad, posicion);
	}
	return alto;
}

int ModeloMovimiento::obtenerAncho(int x, ModeloEntidad* modeloEntidad) {
	int ancho = modeloEntidad->posicion().x - x;
	while (modeloEntidad != NULL) {
		ancho += modeloEntidad->alto();
		Posicion posicion = modeloEntidad->posicion();
		posicion.x += modeloEntidad->ancho();
		modeloEntidad = this->_listaEntidades->detectarColision(this->_modeloEntidad, posicion);
	}
	return ancho;
}

Posicion ModeloMovimiento::obtenerPosicionSiguiente() {
	Posicion posicionSiguiente = this->_posiciones.front();
	this->_posiciones.pop_front();
	return posicionSiguiente;
}

int ModeloMovimiento::obtenerX(ModeloEntidad* modeloEntidad) {
	int x = 0;
	while (modeloEntidad != NULL) {
		Posicion posicion = modeloEntidad->posicion();
		x = posicion.x--;
		modeloEntidad = this->_listaEntidades->detectarColision(this->_modeloEntidad, posicion);
	}
	return x;
}

int ModeloMovimiento::obtenerY(ModeloEntidad* modeloEntidad) {
	int y = 0;
	while (modeloEntidad != NULL) {
		Posicion posicion = modeloEntidad->posicion();
		y = posicion.y--;
		modeloEntidad = this->_listaEntidades->detectarColision(this->_modeloEntidad, posicion);
	}
	return y;
}

ModeloMovimiento::ModeloMovimiento(const ModeloMovimiento &modeloMovimiento) {
}

ModeloMovimiento& ModeloMovimiento::operator=(const ModeloMovimiento &modeloMovimiento) {
	return *this;
}

ModeloMovimiento::ModeloMovimiento(int altoNivel, int anchoNivel, ModeloEntidad* modeloEntidad) {
	this->_altoNivel = altoNivel;
	this->_anchoNivel = anchoNivel;
	this->_modeloEntidad = modeloEntidad;
	this->_listaEntidades = NULL;
	this->_instanteUltimoCambioEstado = 0;
}

ModeloMovimiento::~ModeloMovimiento() {
}

Posicion ModeloMovimiento::posicionSiguiente() {
	return this->_posicionSiguiente;
}

void ModeloMovimiento::actualizar(Posicion posicionDestino) {
	// Dentego el movimiento anterior
	this->detener();

	// Ajusta los movimientos para esten dentro del mapa
	if (posicionDestino.x < 0)
		posicionDestino.x = 0;
	else if (posicionDestino.x >= this->_anchoNivel)
		posicionDestino.x = this->_anchoNivel - 1;
	if (posicionDestino.y < 0)
		posicionDestino.y = 0;
	else if (posicionDestino.y >= this->_altoNivel)
		posicionDestino.y = this->_altoNivel - 1;

	// Si la posicion actual es igual a la destino no ejectuo el movimiento
	if (this->_modeloEntidad->posicion() == posicionDestino)
		return;

	// Calculo la posicion destino por si seleccione una posicion ocupada
	posicionDestino = this->calcularPosicionDestino(posicionDestino);

	char* mapaTilesCerrados = new char[this->_altoNivel * this->_anchoNivel * sizeof(char)];
	list<Tile> tilesAbiertos;
	list<Tile> tilesCerrados;
	Tile* tileActual = NULL;

	memset(mapaTilesCerrados, 0, this->_altoNivel * this->_anchoNivel * sizeof(char));

	this->agregarTile(mapaTilesCerrados, &tilesAbiertos, this->_modeloEntidad->posicion(), posicionDestino, NULL, 0);
	
	DWORD inicio = GetTickCount();

	while (!tilesAbiertos.empty()) {
		// Paso el primer tile abierto a la lista de cerrados
		tilesCerrados.push_back(tilesAbiertos.front());
		tilesAbiertos.pop_front();
		tileActual = &tilesCerrados.back();
		mapaTilesCerrados[(this->_anchoNivel * tileActual->posicion().y) + tileActual->posicion().x] = 1;

		// Si llegue a la posicion destino reconstruyo el camino
		if (tileActual->posicion() == posicionDestino) {
			while (tileActual != NULL) {
				this->_posiciones.push_front(tileActual->posicion());
				tileActual = tileActual->padre();
			}

			// Elimino la primera posicion porque ya estoy en ella
			this->_posiciones.pop_front();

			delete[] mapaTilesCerrados;
			return;
		}

		// Si ya paso el tiempo para calcular el camino minimo salgo
		if ((GetTickCount() - inicio) >= DURACION_CALCULO_CAMINO_MINIMO) {
			delete[] mapaTilesCerrados;
			return;
		}

		Posicion posicionAdyacente;
		bool posicionProcesada = false;

		// Posicion superior izquierda
		posicionAdyacente = tileActual->posicion();
		posicionAdyacente.x -= 1;
		posicionAdyacente.y -= 1;
		posicionProcesada |= this->agregarTile(mapaTilesCerrados, &tilesAbiertos, posicionAdyacente, posicionDestino, tileActual, 14);

		// Posicion superior
		posicionAdyacente = tileActual->posicion();
		posicionAdyacente.y -= 1;
		posicionProcesada |= posicionProcesada |= this->agregarTile(mapaTilesCerrados, &tilesAbiertos, posicionAdyacente, posicionDestino, tileActual, 10);

		// Posicion superior derecha
		posicionAdyacente = tileActual->posicion();
		posicionAdyacente.x += 1;
		posicionAdyacente.y -= 1;
		posicionProcesada |= this->agregarTile(mapaTilesCerrados, &tilesAbiertos, posicionAdyacente, posicionDestino, tileActual, 14);

		// Posicion izquierda
		posicionAdyacente = tileActual->posicion();
		posicionAdyacente.x -= 1;
		posicionProcesada |= posicionProcesada |= this->agregarTile(mapaTilesCerrados, &tilesAbiertos, posicionAdyacente, posicionDestino, tileActual, 10);

		// Posicion derecha
		posicionAdyacente = tileActual->posicion();
		posicionAdyacente.x += 1;
		posicionProcesada |= posicionProcesada |= this->agregarTile(mapaTilesCerrados, &tilesAbiertos, posicionAdyacente, posicionDestino, tileActual, 10);

		// Posicion inferior izquierda
		posicionAdyacente = tileActual->posicion();
		posicionAdyacente.x -= 1;
		posicionAdyacente.y += 1;
		posicionProcesada |= this->agregarTile(mapaTilesCerrados, &tilesAbiertos, posicionAdyacente, posicionDestino, tileActual, 14);

		// Posicion inferior
		posicionAdyacente = tileActual->posicion();
		posicionAdyacente.y += 1;
		posicionProcesada |= posicionProcesada |= this->agregarTile(mapaTilesCerrados, &tilesAbiertos, posicionAdyacente, posicionDestino, tileActual, 10);

		// Posicion inferior derecha
		posicionAdyacente = tileActual->posicion();
		posicionAdyacente.x += 1;
		posicionAdyacente.y += 1;
		posicionProcesada |= this->agregarTile(mapaTilesCerrados, &tilesAbiertos, posicionAdyacente, posicionDestino, tileActual, 14);

		// Ordeno la lista si se prceso alguna posicion
		if (posicionProcesada)
			tilesAbiertos.sort();
	}

	delete[] mapaTilesCerrados;
}

void ModeloMovimiento::asignarListaEntidades(ListaEntidades* listaEntidades) {
	this->_listaEntidades = listaEntidades;
}

void ModeloMovimiento::cambiarEstado() {
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
	this->_modeloEntidad->direccion(Posicion::obtenerDireccion(this->_modeloEntidad->posicion(), posicionSiguiente));
	
	// Si choque con algo en el camino recalculo
	if (this->_listaEntidades->detectarColision(this->_modeloEntidad, posicionSiguiente) != NULL) {
		// Si la ultima posicion es la que esta ocupada me dentego
		if (this->_posiciones.empty()) {
			this->detener();
			return;
		}

		// Recalculo el camino y si no pudo resolverlo se detiene
		this->actualizar(this->_posiciones.back());
		if (this->_posiciones.empty()) {
			this->detener();
			return;
		}
		Posicion posicionSiguiente = this->obtenerPosicionSiguiente();
		this->_modeloEntidad->direccion(Posicion::obtenerDireccion(this->_modeloEntidad->posicion(), posicionSiguiente));
	}

	// Notifico a VistaMovimiento
	this->_posicionSiguiente = posicionSiguiente;
	this->notificarObservadores();
	this->_modeloEntidad->posicion(this->_posicionSiguiente);
	
	this->_instanteUltimoCambioEstado = GetTickCount();
}

void ModeloMovimiento::detener() {
	this->_posiciones.clear();
}
