#include "./Cliente.h"

Cliente::Cliente(void){

}

Cliente::~Cliente(void){

}

bool Cliente::iniciar(void){

	SocketCliente* pSocket = &(this->socket);
	
	// Instancia el nivel en vistaJuego y el scroll con su proxy en ControladorEvento
	if( this->vistaJuego.iniciar(pSocket,this->controladorJuego.getControladorLoop()->getControladorEvento()) == false ) return false;
	
	return true;
}

void Cliente::loop(void){

	int fps = 50;
	int delay = 1000/fps;	
	bool quit = false;		
	while (quit == false){
		int tickViejo = SDL_GetTicks();		

		if( this->controladorJuego.loop() == false) quit = true;
		if( this->vistaJuego.loop() == false) quit = true;

		int intervaloTranscurrido = SDL_GetTicks() - tickViejo;
		if (intervaloTranscurrido < delay){
			SDL_Delay(delay - intervaloTranscurrido);
		}
	}
	 return void();
}

bool Cliente::correrJuego(void){
	if( this->iniciar() == true ){
		this->loop();
		// TODO: �Ac� cerraria todo lo de sockets? Ver eso
	}else{
		Log::getInstance().log(1,__FILE__,__LINE__,"Error al iniciar el juego el cliente.");
		return false;
	}
	return true;
}

void Cliente::destruirEntidades(void){
	return this->vistaJuego.destruirEntidades();
}