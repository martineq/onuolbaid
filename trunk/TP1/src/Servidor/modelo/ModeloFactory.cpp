#include "ModeloFactory.h"

ModeloFactory::ModeloFactory(void){

}

ModeloFactory::~ModeloFactory(void){

}

bool ModeloFactory::crearModeloNivel(ModeloNivel& modeloNivel){
	
	ParserYaml::stJuego juego;
	juego = ParserYaml::getInstance().cargarConfiguracionDeJuego();

	if( juego.juegoValido == false ) return false;
	
	modeloNivel.setAltoNivel(juego.escenarios.front().tamanioX);
	modeloNivel.setAnchoNivel(juego.escenarios.front().tamanioY);
	this->crearJugadorConScroll(juego,modeloNivel);
	this->crearEntidades(juego,modeloNivel);

	return true;
}

void ModeloFactory::crearJugadorConScroll(ParserYaml::stJuego juego, ModeloNivel& modeloNivel){

	ParserYaml::stProtagonista protagonista = juego.escenarios.front().protagonistas.front();
	std::string nombre = protagonista.entidad;
	ParserYaml::stEntidad entidad = ParserYaml::getInstance().buscarStEntidad(juego,nombre);

	int x = protagonista.x;
	int y = protagonista.y;
	int alto = entidad.altoBase;
	int ancho = entidad.anchoBase;
	int velocidad = juego.configuracion.velocidadPersonaje;
	int altoPantalla = juego.escenarios.front().tamanioX;
	int anchoPantalla = juego.escenarios.front().tamanioY;
	Posicion pos;
	pos.x = x;
	pos.y = y;

	ModeloEntidad* pJugador = new ModeloEntidad(alto,ancho,velocidad,pos,true); 
	ModeloScroll* pScroll = new ModeloScroll(juego.pantalla.alto,juego.pantalla.ancho,altoPantalla,anchoPantalla,juego.configuracion.margenScroll,velocidad,x,y,pJugador->obtenerId());  // Tomo el mismo x,y,velocidad que el personaje

	modeloNivel.agregarJugador(pJugador);
	modeloNivel.agregarScroll(pScroll); 

	return void();
}

void ModeloFactory::crearEntidades(ParserYaml::stJuego juego, ModeloNivel& modeloNivel){
	
	std::list<ParserYaml::stEntidadDefinida> entidadesDef = juego.escenarios.front().entidadesDefinidas;
	std::string nombre = juego.escenarios.front().protagonistas.front().entidad;	

	for (std::list<ParserYaml::stEntidadDefinida>::iterator it=entidadesDef.begin() ; it != entidadesDef.end(); it++ ){	

		ParserYaml::stEntidadDefinida entidadDef = (*it);
		std::string nombre = entidadDef.entidad;
		ParserYaml::stEntidad entidad = ParserYaml::getInstance().buscarStEntidad(juego,nombre);

		int x = entidadDef.x;
		int y = entidadDef.y;
		int alto = entidad.altoBase;
		int ancho = entidad.anchoBase;
		int velocidad = 0;
		Posicion pos;
		pos.x = x;
		pos.y = y;

		ModeloEntidad* pEntidad = new ModeloEntidad(alto,ancho,velocidad,pos,true); 

		modeloNivel.agregarEntidad(pEntidad);
	}

	return void();
}