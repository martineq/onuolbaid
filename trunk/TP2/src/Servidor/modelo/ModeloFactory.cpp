#include "ModeloFactory.h"

ModeloFactory::ModeloFactory(void){

}

ModeloFactory::~ModeloFactory(void){

}

// En este m�todo no hace falta usar mutex porque se hace antes de lanzar otros hilos
bool ModeloFactory::crearNivel(ModeloNivel& modeloNivel,ModeloLoop& modeloLoop,SocketServidor* pSocket){
	
	// Inicio el servidor
	if( pSocket->inciarServidor(PUERTO_SERVIDOR) == false ) return false;

	// Cargo el archivo de configuraci�n
	ParserYaml::stJuego juegoYaml;
	juegoYaml = ParserYaml::getInstance().cargarConfiguracionDeJuego();
	if( juegoYaml.juegoValido == false ){
		Log::getInstance().log(1,__FILE__,__LINE__,"Error al obtener la configuraci�n de juego.");
		return false;
	}

	// Preparo el juego elegido
	this->juegoElegido.listaEntidades = juegoYaml.entidades;
	this->juegoElegido.pantalla = juegoYaml.pantalla;
	this->juegoElegido.configuracion = juegoYaml.configuracion;
	if( this->elegirEscenario(juegoYaml.escenarios) == false) return false;

	// Seteo alto y ancho
	modeloNivel.setAnchoTiles(this->juegoElegido.escenario.tamanioX);
	modeloNivel.setAltoTiles(this->juegoElegido.escenario.tamanioY);

	// Creo las entidades del nivel, las que no son los jugadores
	this->crearEntidades(modeloNivel,pSocket);

	// Agrego el ProxyControladorEvento
	ProxyControladorEvento* pProxyEvento = new ProxyControladorEvento();
	pProxyEvento->setSocketServidor(pSocket);
	modeloLoop.setProxyControladorEvento(pProxyEvento);

	return true;
}

bool ModeloFactory::elegirEscenario(std::list<ParserYaml::stEscenario>& listaEscenarios){
	
	// Ver si el escenario se elije desde consola, o si hay que levantarlo de archivo. Por ahora lo tomo de consola
	std::cout << "Elija el escenario: " << std::endl;
	for (std::list<ParserYaml::stEscenario>::iterator it=listaEscenarios.begin() ; it != listaEscenarios.end(); it++ ){
		std::string opc;
		std::cout << "Elije el escenario: "<< (*it).nombre << " ? (s) para confirmar, otra tecla para rechazar" << std::endl;
		getline (std::cin,opc);
		if( opc.compare("s") == 0 || opc.compare("S") == 0 ){
			std::cout << "Se usara el escenario: "<< (*it).nombre << std::endl;
			this->juegoElegido.escenarioElegido = (*it).nombre;
			this->juegoElegido.escenario = (*it);
			return true;
		}
	}

	// Si no eligi� nada
	std::cout << "Se usara el escenario: "<< listaEscenarios.front().nombre << std::endl;
	this->juegoElegido.escenarioElegido = listaEscenarios.front().nombre;
	this->juegoElegido.escenario = listaEscenarios.front();
	return true;
}

bool rutinaAgregarNuevoCliente(ModeloNivel* modeloNivel,SocketServidor* pSocket, int id){
	// TODO: Implementar. Ac� lanzo las subrutinas:
		//bool enviarEscenario(SocketServidor* pSocket);
		//bool elegirProtagonista(SocketServidor* pSocket);
		//bool enviarOtrosJugadores(ModeloNivel* modeloNivel,SocketServidor* pSocket);

	return true; // return false si hay error de sockets en las subrutinas
}

bool ModeloFactory::enviarEscenario(SocketServidor* pSocket){
	// TODO: Implementar. Env�o el escenario elegido al Cliente
	// Ac� envio this->juegoElegido.escenarioElegido y this->juegoElegido.listaIdEntidades

	return true; // return false si hay error de sockets
}

// Para que lo use el hilo de configuraci�n
bool ModeloFactory::elegirProtagonista(SocketServidor* pSocket, int id){
	ParserYaml::stProtagonista protagonista = this->juegoElegido.escenario.protagonistas.front();
	// TODO: *** Refactorizar de acuerdo al TP2. Esta es la contraparte del VistaFactory::recibirProtagonista() ***
	//		 + Ac� es donde le paso this->listaIdEntidades a cada cliente para que sepa que ID ponerle a sus entidades vista. 
	//	     + Ademas le paso aparte el ID de la entidad que es el jugador
	//		 + Aparte le paso los id y los datos de los jugadores que se agregaron adem�s de el
	
	// Implementar toda la comunicaci�n con el Servidor para decirle el protagonista elelgido, el nombre de usuario y
	// luego de obtener una respuesta positiva del servidor devolver el protagonista elegido. (Por ahora devuelvo el primero)

	// Ac� llamo a this->crearJugador() una vez elegido por el cliente

	return true; // return false si hay error de sockets
}

bool ModeloFactory::enviarOtrosJugadores(ModeloNivel* modeloNivel,SocketServidor* pSocket){
	// TODO: Implementar. Ac� envio los datos necesarios para crear en la vista a los otros jugadores conectados (si es que hay) 

	return true; // return false si hay error de sockets
}

// Para que lo use el hilo de configuraci�n
void ModeloFactory::crearJugador(ModeloNivel* modeloNivel,SocketServidor* pSocket, int id){
// TODO: ac� tengo que agregar las l�neas:
//		ProxyModeloEntidad* pProxyEntidad = new ProxyModeloEntidad();
//		pProxyEntidad->setSocketServidor(pSocket);
//		para luego ponerlo en el construcotr de ModeloEntidad del personaje

/*	ParserYaml::stProtagonista protagonista = juego.escenarios.front().protagonistas.front();
	std::string nombre = protagonista.entidad;
	ParserYaml::stEntidad entidad = ParserYaml::getInstance().buscarStEntidad(juego.entidades,nombre);

	int alto = entidad.altoBase;
	int ancho = entidad.anchoBase;
	int velocidad = juego.configuracion.velocidadPersonaje;
	int anchoEscenario = juego.escenarios.front().tamanioX;
	int altoEscenario = juego.escenarios.front().tamanioY;
	Posicion pos;
	pos.x = protagonista.x;
	pos.y = protagonista.y;

	ModeloEntidad* pJugador = new ModeloEntidad(alto,ancho,velocidad,pos,true,altoEscenario,anchoEscenario,entidad.fps); 

	modeloNivel.agregarJugador(pJugador);
	*/
	return void();
}

void ModeloFactory::crearEntidades(ModeloNivel& modeloNivel,SocketServidor* pSocket){
	
	ModeloFactory::stModeloJuegoElegido juego = this->juegoElegido;

	std::list<ParserYaml::stEntidadDefinida> entidadesDef = juego.escenario.entidadesDefinidas;

	for (std::list<ParserYaml::stEntidadDefinida>::iterator it=entidadesDef.begin() ; it != entidadesDef.end(); it++ ){	

		ParserYaml::stEntidadDefinida entidadDef = (*it);
		std::string nombreEntidad = entidadDef.entidad;
		ParserYaml::stEntidad entidad = ParserYaml::getInstance().buscarStEntidad(juego.listaEntidades,nombreEntidad); 
		int x = entidadDef.x;
		int y = entidadDef.y;
		int alto = entidad.altoBase;
		int ancho = entidad.anchoBase;
		int velocidad = 0;
		int anchoEscenario = juego.escenario.tamanioX;
		int altoEscenario = juego.escenario.tamanioY;
		Posicion pos;
		pos.x = x;
		pos.y = y;

		ProxyModeloEntidad* pProxyEntidad = new ProxyModeloEntidad();
		pProxyEntidad->setSocketServidor(pSocket);

		// Voy guardando todos los ID's que se crean por cada entidad en uan lista, para luego pasarle esta a cada cliente as� puede numerar de la misma forma a todas las entidades vista.
		// De esta forma cada EntidadModelo y cada EntidadVista van a tener el mismo ID
		int nuevoID = Ticket::getInstance().pedirNumero();
		this->juegoElegido.listaIdEntidades.push_back(nuevoID);
		ModeloEntidad* pEntidad = new ModeloEntidad(alto,ancho,velocidad,pos,false,altoEscenario,anchoEscenario,entidad.fps,pProxyEntidad,nuevoID);
		modeloNivel.agregarEntidad(pEntidad);
	}

	

	return void();
}
