#include "VistaFactory.h"

VistaFactory::VistaFactory(void){

}

VistaFactory::~VistaFactory(void){

}

bool VistaFactory::crearNivel(VistaNivel& vistaNivel,ControladorEvento* evento,SocketCliente* pSocket,SDL_Surface** pPantallaDestino, ProxyModeloEntidad** pProxyDestino,std::string mote,std::string personaje,bool singlePlayer){

	// Me conecto al servidor
	if( this->conectarSocket(pSocket,singlePlayer) == false ) return false;

	// Recibo los archivos desde el servidor, en caso de no ser single player
	if( singlePlayer == false ) {
		//std::cout << "(Recepcion de archivos deshabilitada para el debug)" << std::endl;
		if( this->recibirArchivos(pSocket) == false ) return false; // TODO: Descomentar para el momento de la entrega
	}

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

	// Recibo datos desde el Servidor
	if( this->recibirEscenario(juegoYaml.escenarios,pSocket) == false ) return false;
	if( this->recibirEnemigosAutomaticos(pSocket) == false ) return false;
	if( this->recibirItems(pSocket) == false ) return false;
	if( this->recibirProtagonista(pSocket,mote,personaje) == false ) return false;
	if( this->recibirOtrosJugadores(pSocket) == false ) return false;	
	if( this->esperarComienzoDeJuego(pSocket,singlePlayer) == false ) return false;	

	if( ImageLoader::getInstance().iniciarSDL() == false ) return false;
	if( VistaMusica::getInstance().iniciar() == false ) return false;
	VistaMusica::getInstance().playTheMusic();

	SDL_Surface* pPantallaSDL = ImageLoader::getInstance().levantarPantalla(this->juegoElegido.pantalla.ancho,this->juegoElegido.pantalla.alto);

	// Ac� termino los env�os uno a uno con el servidor de tipo bloqueante y empiezo a tomar desde una cola no bloqueante
	pSocket->setEnvioIndirecto();

	// Creo los elementos de la Vista
	if( this->crearElementosVista(pPantallaSDL,vistaNivel,pSocket,pPantallaDestino,pProxyDestino) == false ) return false;

	// Creo los elementos del Controlador
	if( this->crearElementosControlador(vistaNivel,evento,pSocket) == false ) return false;

	return true;
}

bool VistaFactory::conectarSocket(SocketCliente* pSocket,bool singlePlayer){

	ParserYaml::stConexion conexion;

	// Obtengo datos de conexion
	if( singlePlayer == true ){
		conexion.ip.assign(SOCKETS_LOCALHOST_IP);
		conexion.puerto = SOCKETS_LOCALHOST_PUERTO;
	}else{
		conexion = ParserYaml::getInstance().cargarConfiguracionDeConexion();
		if( conexion.conexionValida == false ){
			std::cerr << "Error al leer datos de conexion." << std::endl;
			Log::getInstance().log(1,__FILE__,__LINE__,"Error al leer datos de conexion.");
			return false;
		}
	}

	// Me conecto al servidor
	if( pSocket->iniciarCliente(conexion.ip.c_str(),conexion.puerto) == false ){
		std::cerr << "Error al iniciar la conexion cliente." << std::endl;
		Log::getInstance().log(1,__FILE__,__LINE__,"Error al iniciar la conexion cliente.");
		return false;
	}else{
		pSocket->setEnvioDirecto();
	}

	return true;
}

// Recibe del servidor todos los archivos necesarios para el funcionamiento del juego
bool VistaFactory::recibirArchivos(SocketCliente* pSocket){

	double tiempoInicial = Temporizador::getInstance().obtenerTics();

	// Recibo los archivos de imagenes
	if( this->recibirListaDeArchivos(DIRECTORIO_IMG,pSocket) == false) return false;

	// Recibo los archivos de configuracion
	if( this->recibirListaDeArchivos(DIRECTORIO_CONFIG,pSocket) == false) return false;

	// Recibo los archivos de sonido
	if( this->recibirListaDeArchivos(DIRECTORIO_SOUNDS,pSocket) == false) return false;

	double tiempoFinal = Temporizador::getInstance().obtenerTics();
	double periodoTranscurrido =  ( tiempoFinal - tiempoInicial ) / 1000;
	std::cout << "Tiempo de recepcion de archivos: " << periodoTranscurrido << " segundos. " << std::endl;

	return true;
}

bool VistaFactory::recibirListaDeArchivos(const char* directorioElegido,SocketCliente* pSocket){

	Serializadora s;
	if( pSocket->recibir(s) == false ) return false;
	if( s.size() == 0 ) return false;
	
	// Hidrato el vector de strings y recibo cada archivo
	int cantidadDeArchivos = 0;
	cantidadDeArchivos = s.getInt();
	for ( int i=0 ; i < cantidadDeArchivos ; i++ ){
		std::string rutaDestino(s.getString());
		if ( pSocket->recibirArchivo(rutaDestino.c_str()) == false) return false; // Recibo el archivo binario
	}
	return true;
}

bool VistaFactory::recibirEscenario(std::list<ParserYaml::stEscenario>& listaEscenarios,SocketCliente* pSocket){

	Serializadora s;
	if( pSocket->recibir(s) == false ) return false;
	if( s.size() == 0 ) return false;

	// Comienzo a hidratar. Hidrato el nombre de escenario
	std::string nombreEscenario = s.getString();
	this->asignarEscenarioElegido(nombreEscenario,listaEscenarios); // A partir del nombre recibido, asigno el escenario

	// Hidrato la lista de ID's serializados
	this->juegoElegido.listaIdEntidades.clear();
	int cantidadDeIds = s.getInt();
	for ( int i=0 ; i < cantidadDeIds ; i++ ){
		this->juegoElegido.listaIdEntidades.push_back(s.getInt());
	}

	return true; 
}

void VistaFactory::asignarEscenarioElegido(std::string nombreEscenario,std::list<ParserYaml::stEscenario>& listaEscenarios){
	
	for (std::list<ParserYaml::stEscenario>::iterator it=listaEscenarios.begin() ; it != listaEscenarios.end(); it++ ){
		if( (*it).nombre.compare(nombreEscenario) == 0 ){
			this->juegoElegido.escenario = (*it);
			return void();
		}
	}
	
	return void();
}

bool VistaFactory::recibirEnemigosAutomaticos(SocketCliente* pSocket){

	Serializadora s;
	if( pSocket->recibir(s) == false ) return false;
	if( s.size() == 0 ) return false;

	// Hidrato la cantidad de enemigos que me van a mandar
	int cantidadEnemigosAutomaticos = s.getInt();

	// Recibo los datos de los enemigos, a trav�s de un proxy
	ProxyModeloEntidad proxy;
	proxy.setSocketCliente(pSocket);
	ProxyModeloEntidad::stEntidad entidad;
	for ( unsigned int i=0 ; i<cantidadEnemigosAutomaticos ; i++ ){ 
		if( proxy.recibirEntidadIndividual(entidad) == false ) return false;
		this->juegoElegido.listaEnemigosAutomaticos.push_back(entidad);
	}

	return true; 
}

bool VistaFactory::recibirItems(SocketCliente* pSocket){

	Serializadora s;
	if( pSocket->recibir(s) == false ) return false;
	if( s.size() == 0 ) return false;

	// Hidrato la cantidad de items que me van a mandar
	int cantidadItems = s.getInt();

	// Recibo los datos de los items, a trav�s de un proxy
	ProxyModeloEntidad proxy;
	proxy.setSocketCliente(pSocket);
	ProxyModeloEntidad::stEntidad entidad;
	for ( unsigned int i=0 ; i<cantidadItems ; i++ ){ 
		if( proxy.recibirEntidadIndividual(entidad) == false ) return false;
		this->juegoElegido.listaItems.push_back(entidad);
	}

	return true; 
}

// Recibe los datos del protagonista elegido y los setea, pero no lo crea (se hace luego, en crearNivel() )
bool VistaFactory::recibirProtagonista(SocketCliente* pSocket,std::string nombreUsuario,std::string nombrePersonaje){

	// Serializo los nombres de usuario y protagonista, luego lo envio al servidor
	Serializadora s;
	s.addString(nombreUsuario);
	s.addString(nombrePersonaje);
	if( pSocket->enviar(s) == false ) return false;

	// Recibo al protagonista, y dejo cargado los datos para su posterior creaci�n
	ProxyModeloEntidad::stEntidad entidad;
	ProxyModeloEntidad proxy;
	proxy.setSocketCliente(pSocket);
	if( proxy.recibirEntidadIndividual(entidad) == false ) return false;	// Si hubo error de sockets me voy
	std::string matriz;
	if( proxy.recibirMatrix(matriz) == false ) return false;	// Si hubo error de sockets me voy
	ProxyModeloEntidad::cargarMatriz(entidad,matriz);
	this->juegoElegido.entidadJugador = entidad;	

	this->matriz = matriz;

	if( nombreUsuario.compare(entidad.nombreJugador) != 0 ){
		std::cout << "El nombre de usuario selecionado ya existe, su usuario ha sido cambiado a: " << entidad.nombreJugador << std::endl;
		Log::getInstance().log(1,__FILE__,__LINE__,"El nombre de usuario selecionado ya existe, su usuario ha sido cambiado a: ",entidad.nombreJugador);
	}
	
	if( nombrePersonaje.compare(entidad.nombreEntidad) != 0 ){
		std::cout << "El personaje selecionado no se encuentra disponible, su personaje ha sido cambiado a: " << entidad.nombreEntidad << std::endl;
		Log::getInstance().log(1,__FILE__,__LINE__,"El personaje selecionado no se encuentra disponible, su personaje ha sido cambiado a: ",entidad.nombreEntidad);
	}

	return true;
}

// Obtiene los datos de todos los jugadores que no son controlados por este cliente, para luego poder crearlos
bool VistaFactory::recibirOtrosJugadores(SocketCliente* pSocket){

	Serializadora s;
	if( pSocket->recibir(s) == false ) return false;
	if( s.size() == 0 ) return false;

	// Hidrato la cantidad de personajes que me van a mandar
	int cantidadOtrosJugadores = s.getInt();

	// Recibo los datos de los jugadores, a trav�s de un proxy
	ProxyModeloEntidad proxy;
	proxy.setSocketCliente(pSocket);
	ProxyModeloEntidad::stEntidad entidad;
	for ( unsigned int i=0 ; i<cantidadOtrosJugadores ; i++ ){ 
		if( proxy.recibirEntidadIndividual(entidad) == false ) return false;
		this->juegoElegido.listaOtrosJugadores.push_back(entidad);
	}

	return true; 
}

bool VistaFactory::crearElementosVista(SDL_Surface* pPantallaSDL, VistaNivel& vistaNivel,SocketCliente* pSocket, SDL_Surface** pPantallaDestino, ProxyModeloEntidad** pProxyDestino){

	// Inicio y seteo de SDL
	(*pPantallaDestino) = pPantallaSDL;

	// Seteo medidas de pantalla
	vistaNivel.setAltoDePantallaEnPixel(this->juegoElegido.pantalla.alto);
	vistaNivel.setAltoDePantallaEnPixel(this->juegoElegido.pantalla.ancho);

	// Seteo el ProxyModeloEntidad
	ProxyModeloEntidad* pProxyEntidad = new ProxyModeloEntidad();
	pProxyEntidad->setSocketCliente(pSocket);
	(*pProxyDestino) = pProxyEntidad;

	// Creo al protagonista y a las entidades (no creo a otros jugadores)
	this->crearJugadorConScroll(vistaNivel,pPantallaSDL);
	this->crearOtrosJugadores(vistaNivel);
	this->crearEnemigosAutomaticos(vistaNivel);
	this->crearItems(vistaNivel);
	this->crearEntidadesEstaticas(vistaNivel);

	return true;
}

void VistaFactory::crearJugadorConScroll(VistaNivel& vistaNivel,SDL_Surface* pantalla){
 
	std::string nombreProtagonista = this->juegoElegido.entidadJugador.nombreEntidad;
	ParserYaml::stEntidad entidadProtagonista = ParserYaml::getInstance().buscarStEntidad(this->juegoElegido.listaEntidades,nombreProtagonista);
	ParserYaml::stProtagonista protgonistaDefinido = ParserYaml::getInstance().buscarStProtagonista(this->juegoElegido.escenario,nombreProtagonista);

	// Valores tomados desde el protagonista selecionado (y cargado) anteriormente
	double x = (double)this->juegoElegido.entidadJugador.posicionX;
	double y = (double)this->juegoElegido.entidadJugador.posicionY;
	int id = this->juegoElegido.entidadJugador.id;
	int direccion = this->juegoElegido.entidadJugador.accion;
	int escudo = this->juegoElegido.entidadJugador.escudo;
	int tipoEntidad = TIPO_ENTIDAD_JUGADOR;
	
	bool tieneGolem = this->juegoElegido.entidadJugador.tieneGolem;
	bool tieneHechizo = this->juegoElegido.entidadJugador.tieneHechizoHielo;
	int bombas = this->juegoElegido.entidadJugador.cantidadBombas;

	
	// Valores tomados desde la entidad
	double alto = (double)entidadProtagonista.altoBase;
	double ancho = (double)entidadProtagonista.anchoBase;
	double posicionReferenciaX = (double)entidadProtagonista.pixelReferenciaX;
	double posicionReferenciaY = (double)entidadProtagonista.pixelReferenciaY;
	double fps = (double)entidadProtagonista.fps;
	double delay = (double)entidadProtagonista.delay;
	std::string nombre = entidadProtagonista.nombre;
	std::list<std::list<std::string>> listaAnimaciones = entidadProtagonista.imagenes;

	// Valores tomados desde el protgonistaDefinido
	int vida = 	protgonistaDefinido.vida;
	int magia = protgonistaDefinido.mana;

	// Valores tomados desde el escenario
	double tamanioX = (double)this->juegoElegido.escenario.tamanioX;
	double tamanioY = (double)this->juegoElegido.escenario.tamanioY;	

	VistaEntidad* pJugador = new VistaEntidad(x,y,alto,ancho,posicionReferenciaX,posicionReferenciaY,this->juegoElegido.entidadJugador.rangoVision,fps,delay,listaAnimaciones,true,tamanioX,tamanioY,id,nombre,false,direccion,this->juegoElegido.entidadJugador.nombreJugador,vida,tipoEntidad,magia, escudo,tieneGolem,tieneHechizo,bombas,id);
	VistaScroll* pScroll = new VistaScroll(x,y,this->juegoElegido.pantalla.alto,this->juegoElegido.pantalla.ancho,tamanioX,tamanioY,pantalla,id);	// Tomo el mismo x,y,velocidad que el personaje
	pJugador->esMiJugador(true);
	vistaNivel.agregarJugador(pJugador);
	vistaNivel.agregarScroll(pScroll);
	vistaNivel.agregarTamanioNivel(tamanioX,tamanioY);

	return void();
}

void VistaFactory::crearOtrosJugadores(VistaNivel& vistaNivel){

	std::list<ProxyModeloEntidad::stEntidad> listaOtrosJugadores = this->juegoElegido.listaOtrosJugadores;

	for (std::list<ProxyModeloEntidad::stEntidad>::iterator it=listaOtrosJugadores.begin() ; it != listaOtrosJugadores.end(); it++ ){
		ProxyModeloEntidad::stEntidad entidad = (*it);
		this->crearJugadorSinScroll(vistaNivel,entidad);
	}

	return void();
}

void VistaFactory::crearJugadorSinScroll(VistaNivel& vistaNivel,ProxyModeloEntidad::stEntidad& entidad){

	std::string nombreJugador = entidad.nombreEntidad;
	ParserYaml::stEntidad entidadJugador = ParserYaml::getInstance().buscarStEntidad(this->juegoElegido.listaEntidades,nombreJugador);

	// Valores tomados desde el proxy
	int id = entidad.id;
	double x = (double)entidad.posicionX;
	double y = (double)entidad.posicionY;	
	int magia = entidad.magia;
	int vida = entidad.vida;
	int escudo = entidad.escudo;
	int tipoEntidad = entidad.tipoEntidad;

	// Valores tomados desde la entidad
	double alto = (double)entidadJugador.altoBase;
	double ancho = (double)entidadJugador.anchoBase;
	double posicionReferenciaX = (double)entidadJugador.pixelReferenciaX;
	double posicionReferenciaY = (double)entidadJugador.pixelReferenciaY;
	double fps = (double)entidadJugador.fps;
	double delay = (double)entidadJugador.delay;
	std::list<std::list<std::string>> listaAnimaciones = entidadJugador.imagenes;
	std::string nombre = entidadJugador.nombre;

	// Valores tomados desde el escenario elegido
	double tamanioX = (double)this->juegoElegido.escenario.tamanioX;
	double tamanioY = (double)this->juegoElegido.escenario.tamanioY;

	bool esJugador = true;

	VistaEntidad* pJugador = new VistaEntidad(x,y,alto,ancho,posicionReferenciaX,posicionReferenciaY,entidad.rangoVision,fps,delay,listaAnimaciones,esJugador,tamanioX,tamanioY,id,nombre,entidad.estaCongelado,entidad.accion,entidad.nombreJugador,vida,tipoEntidad,magia,escudo,false,false,0,this->juegoElegido.entidadJugador.id);
	vistaNivel.agregarOtroJugador(pJugador);

	if (entidadJugador.nombre == STRING_BOMBA) {
		list<string> lista = entidadJugador.imagenes.front();
		string dirBomba = lista.front();
		SDL_Surface* temp = ImageLoader::getInstance().load_image(dirBomba);
		vistaNivel.setPngBomba(ImageLoader::getInstance().stretch(temp,50,50,dirBomba));
		SDL_FreeSurface(temp);
	}

	if (entidadJugador.nombre == STRING_HECHIZO_HIELO) {
		list<string> lista = entidadJugador.imagenes.front();
		string dirMagia = lista.front();
		SDL_Surface* temp = ImageLoader::getInstance().load_image(dirMagia);
		vistaNivel.setPngMagia(ImageLoader::getInstance().stretch(temp,50,50,dirMagia));
		SDL_FreeSurface(temp);
	}

	if (entidadJugador.nombre == STRING_GOLEM) {
		list<string> lista = entidadJugador.imagenes.front();
		string dirGolem = lista.front();
		SDL_Surface* temp = ImageLoader::getInstance().load_image(dirGolem);
		vistaNivel.setPngGolem(ImageLoader::getInstance().stretch(temp,50,50,dirGolem));
		SDL_FreeSurface(temp);		
	}



	return void();
}

void VistaFactory::crearEnemigosAutomaticos(VistaNivel& vistaNivel){
	
	std::list<ProxyModeloEntidad::stEntidad> listaEnemigos = this->juegoElegido.listaEnemigosAutomaticos;

	for (std::list<ProxyModeloEntidad::stEntidad>::iterator it=listaEnemigos.begin() ; it != listaEnemigos.end(); it++ ){
		ProxyModeloEntidad::stEntidad entidad = (*it);
		this->crearJugadorSinScroll(vistaNivel,entidad);
	}

	return void();
}

void VistaFactory::crearItems(VistaNivel& vistaNivel){

	std::list<ProxyModeloEntidad::stEntidad> listaItems = this->juegoElegido.listaItems;

	for (std::list<ProxyModeloEntidad::stEntidad>::iterator it=listaItems.begin() ; it != listaItems.end(); it++ ){
		ProxyModeloEntidad::stEntidad entidad = (*it);
		this->crearJugadorSinScroll(vistaNivel,entidad);
	}

	return void();
}

void VistaFactory::crearEntidadesEstaticas(VistaNivel& vistaNivel){

	std::list<ParserYaml::stEntidadDefinida> entidadesDef = this->juegoElegido.escenario.entidadesDefinidas;
	std::list<int>idEntidadesDef = this->juegoElegido.listaIdEntidades;

	for (std::list<ParserYaml::stEntidadDefinida>::iterator it=entidadesDef.begin() ; it != entidadesDef.end(); it++ ){	

		ParserYaml::stEntidadDefinida entidadDef = (*it);
		std::string nombreEntidad = entidadDef.entidad;
		ParserYaml::stEntidad entidad = ParserYaml::getInstance().buscarStEntidad(this->juegoElegido.listaEntidades,nombreEntidad);

		// Valores tomados desde la entidad definida
		double x = (double)entidadDef.x;
		double y = (double)entidadDef.y;

		// Valores tomados desde la entidad
		double alto = (double)entidad.altoBase;
		double ancho = (double)entidad.anchoBase;
		double posicionReferenciaX = (double)entidad.pixelReferenciaX;
		double posicionReferenciaY = (double)entidad.pixelReferenciaY;
		double fps = (double)entidad.fps;
		double delay = (double)entidad.delay;
		std::list<std::list<std::string>> listaAnimaciones = entidad.imagenes;
		std::string nombre = entidad.nombre;

		// Valores tomados desde el escenario elegido
		double tamanioX = (double)this->juegoElegido.escenario.tamanioX;
		double tamanioY = (double)this->juegoElegido.escenario.tamanioY;

		// Valores tomados desde el servidor, cargados anteriormente en una variable
		int id = idEntidadesDef.front();

		// Valores fijos
		int vida = 1;
		int magia = 0;
		int escudo = 0;
		int tipoEntidad = TIPO_ENTIDAD_ESTATICO;

		VistaEntidad* pEntidad = new VistaEntidad(x,y,alto,ancho,posicionReferenciaX,posicionReferenciaY,this->juegoElegido.entidadJugador.rangoVision,fps,delay,listaAnimaciones,false,tamanioX,tamanioY,id,nombre,false,0,this->juegoElegido.entidadJugador.nombreJugador,vida,tipoEntidad,magia,escudo,false,false,0,this->juegoElegido.entidadJugador.id);
		vistaNivel.agregarEntidad(pEntidad);

		idEntidadesDef.pop_front();	// Una vez que us� el ID lo destruyo para tener acceso al pr�ximo
	}

	return void();
}

bool VistaFactory::crearElementosControlador(VistaNivel& vistaNivel,ControladorEvento* evento,SocketCliente* pSocket){

	// Seteo el ID del jugador en el Evento
	evento->setIdJugador(this->juegoElegido.entidadJugador.id);

	// Creo el Scroll
	this->crearControladorScroll(evento,vistaNivel);
	
	// Creo el Proxy
	this->crearProxyControladorEvento(evento,pSocket);

	// Vinculo el VistaScroll con el ControladorScroll
	this->vincularScroll(vistaNivel,evento);

	return true;
}

void VistaFactory::crearControladorScroll(ControladorEvento* evento,VistaNivel& vistaNivel){
	
	// Valores tomados desde el protagonista selecionado (y cargado) anteriormente
	int x = this->juegoElegido.entidadJugador.posicionX;
	int y = this->juegoElegido.entidadJugador.posicionY;

	// Valores tomados desde el escenario elegido
	int anchoEscenario = this->juegoElegido.escenario.tamanioX;
	int altoEscenario = this->juegoElegido.escenario.tamanioY;

	ControladorScroll* pScroll = new ControladorScroll(this->juegoElegido.pantalla.ancho,this->juegoElegido.pantalla.alto,anchoEscenario,altoEscenario,this->juegoElegido.configuracion.margenScroll,x,y);
	evento->setControladorScroll(pScroll, this->juegoElegido.escenario.tamanioX, this->juegoElegido.escenario.tamanioY);

	// Agrego una referencia al ControladorScroll, s�lo para poder reiniciarlo
	vistaNivel.setControladorScroll(pScroll);

	return void();
}

void VistaFactory::crearProxyControladorEvento(ControladorEvento* evento,SocketCliente* pSocket){

	ProxyControladorEvento* pProxyEvento = new ProxyControladorEvento();
	pProxyEvento->setSocketCliente(pSocket);
	evento->setProxyEvento(pProxyEvento);

	return void();
}

void VistaFactory::vincularScroll(VistaNivel& vistaNivel,ControladorEvento* evento){

	ControladorScroll* pCtrlScroll = evento->getControladorScroll();
	VistaScroll* pVistaScroll = vistaNivel.getScroll();

	pCtrlScroll->agregarObservador(pVistaScroll);

	return void();
}

std::string VistaFactory::getMatriz(){
	return this->matriz;
}

bool VistaFactory::esperarComienzoDeJuego(SocketCliente* pSocket,bool singlePlayer){
	if( singlePlayer == false) std::cout << "Esperando conexion de jugadores rivales..." << std::endl;
	Serializadora s;
	return pSocket->recibir(s);
}