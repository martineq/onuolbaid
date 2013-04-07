#include "Pruebas.h"

Pruebas::Pruebas(void){

}

Pruebas::~Pruebas(void){

}


void Pruebas::PruebaSdl(void){

	// Estructura para la superficie gr�fica, donde se va a dibujar
	SDL_Surface *pantalla, *temp, *sprite, *pasto;

	// Define un �rea rectangular
	SDL_Rect rcSprite, rcPasto;

	// Estructura para los eventos generales (teclado, mouse, etc.)
	SDL_Event event;

	Uint8 *estadoTecla;
	int colorkey, finJuego;

	// Escribo una linea en el archivo de log
	Log::getInstance().log(1,__FILE__,__LINE__,"Inicio de prueba SDL");

	// Inicio SDL
	SDL_Init(SDL_INIT_VIDEO);

	// Seteo el nombre en la barra
	SDL_WM_SetCaption("..::Prueba SDL::..", "<Prueba SDL>");

	// Creo la ventana
	pantalla = SDL_SetVideoMode(PANTALLA_ANCHO, PANTALLA_ALTO, 0, 0);

	// Carga el sprite
	temp   = SDL_LoadBMP(SDL_RUTA_SPRITE);
	sprite = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	// Seteo el color del sprite  y lo transforma en RLE (Run-length encoding)
	colorkey = SDL_MapRGB(pantalla->format, 255, 0, 255);
	SDL_SetColorKey(sprite, SDL_SRCCOLORKEY | SDL_RLEACCEL, colorkey);

	// Cargo el pasto
	temp  = SDL_LoadBMP(SDL_RUTA_PASTO);
	pasto = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	// Seteo la posici�n del sprite
	rcSprite.x = 0;
	rcSprite.y = 0;

	// Pongo fin del juego en falso
	finJuego = 0;

	// Tomo las acciones
	while (!finJuego)
	{
	// Miro si hay un evento <event>
	if (SDL_PollEvent(&event)) {
	  // Encontr� un evento
	  switch (event.type) {
		// Se presion� el bot�n de salir
		case SDL_QUIT:
		  finJuego = 1;
		  break;

		// Manejo eventos del teclado
		case SDL_KEYDOWN:
		  switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
			case SDLK_q:
			  finJuego = 1;
			  break;
		  }
		  break;
	  }
	}

	// Manejo el movimiento del Sprite
	estadoTecla = SDL_GetKeyState(NULL);
	if (estadoTecla[SDLK_LEFT] ) {
	  rcSprite.x -= 2;
	}
	if (estadoTecla[SDLK_RIGHT] ) {
	  rcSprite.x += 2;
	}
	if (estadoTecla[SDLK_UP] ) {
	  rcSprite.y -= 2;
	}
	if (estadoTecla[SDLK_DOWN] ) {
	  rcSprite.y += 2;
	}

	// Hago conincidir con los bordes de la pantalla
	if ( rcSprite.x < 0 ) {
	  rcSprite.x = 0;
	}
	else if ( rcSprite.x > PANTALLA_ANCHO-TAMANIO_SPRITE ) {
	  rcSprite.x = PANTALLA_ANCHO-TAMANIO_SPRITE;
	}
	if ( rcSprite.y < 0 ) {
	  rcSprite.y = 0;
	}
	else if ( rcSprite.y > PANTALLA_ALTO-TAMANIO_SPRITE ) {
	  rcSprite.y = PANTALLA_ALTO-TAMANIO_SPRITE;
	}

	// Dibujo el pasto
	for (int x = 0; x < PANTALLA_ANCHO / TAMANIO_SPRITE; x++) {
	  for (int y = 0; y < PANTALLA_ALTO / TAMANIO_SPRITE; y++) {
		rcPasto.x = x * TAMANIO_SPRITE;
		rcPasto.y = y * TAMANIO_SPRITE;
		SDL_BlitSurface(pasto, NULL, pantalla, &rcPasto);
	  }
	}

	// Dibujo el sprite
	SDL_BlitSurface(sprite, NULL, pantalla, &rcSprite);

	// Refresco la pantalla
	SDL_UpdateRect(pantalla, 0, 0, 0, 0);
	}

	// Limpio la superficie
	SDL_FreeSurface(sprite);
	SDL_FreeSurface(pasto);

	// Salgo del SDL
	SDL_Quit();

	Log::getInstance().log(1,__FILE__,__LINE__,"Fin de prueba SDL");

}

void Pruebas::PruebaYAML(void){
	ParserYaml::getInstance().cargarConfiguracionDeJuego();
	ParserYaml::getInstance().cargarConfiguracionDeJuego();
}

void Pruebas::PruebaHilos(void){

	HiloDePrueba hiloUno;
	HiloDePrueba hiloDos;

	// Lanzo los hilos. Si lo necesito puedo pasar por par�metro un void*
	// para luego ser usado en la implementaci�n del run(void*) HiloDePrueba
	hiloUno.start(NULL);
	hiloDos.start(NULL);
	
	// Espero a que terminen los hilos
	hiloUno.join();
	hiloDos.join();

	char c;
	std::cin >> c;

	return void();
}

void Pruebas::PruebaAnimacion(){
	ImageLoader::getInstance().iniciarSDL();
//	double PANTALLA_ANCHO = 800;
//	double PANTALLA_ALTO = 600;
//	double TAMANIO_SPRITE = 10;
	double ALTO_SPRITE = 70;
	double ANCHO_SPRITE = 50;
	SDL_Surface* pantalla = ImageLoader::getInstance().levantarPantalla(PANTALLA_ANCHO,PANTALLA_ALTO);
	SDL_Surface* temp, *pasto;
	list<string> lista;	
	list<list<string>> listaAnimaciones;
	lista.push_back("./img/SORA_S1.bmp");
	lista.push_back("./img/SORA_S2.bmp");
	lista.push_back("./img/SORA_S3.bmp");
	lista.push_back("./img/SORA_S4.bmp");
	lista.push_back("./img/SORA_S5.bmp");
	lista.push_back("./img/SORA_S6.bmp");
	lista.push_back("./img/SORA_S7.bmp");
	lista.push_back("./img/SORA_S8.bmp");
	lista.push_back("./img/SORA_S9.bmp");

	listaAnimaciones.push_back(lista);
/*	lista.push_back("./img/SORA_N1.bmp");
	lista.push_back("./img/SORA_N2.bmp");
	lista.push_back("./img/SORA_N3.bmp");
	lista.push_back("./img/SORA_N4.bmp");
	lista.push_back("./img/SORA_N5.bmp");
	lista.push_back("./img/SORA_N6.bmp");
	lista.push_back("./img/SORA_N7.bmp");
	lista.push_back("./img/SORA_N8.bmp");
*/
	list<string> listaE;
	listaE.push_back("./img/SORA_E1.png");
	listaE.push_back("./img/SORA_E2.png");
	listaE.push_back("./img/SORA_E3.png");
	listaE.push_back("./img/SORA_E4.png");
	listaE.push_back("./img/SORA_E5.png");
	listaE.push_back("./img/SORA_E6.png");
	listaE.push_back("./img/SORA_E7.png");
	listaE.push_back("./img/SORA_E8.png");
	listaAnimaciones.push_back(listaE);


	VistaEntidad* vEntidad = new VistaEntidad(0,0,70,50,0,0,15,100,listaAnimaciones,true);
	vEntidad->setPantalla(pantalla);
	//VistaAnimaciones* animaciones = new VistaAnimaciones();
	//animaciones->agregar(lista.front(),lista,200,50,70,15);
	//animaciones->agregar(listaE.front(),listaE,200,50,70,15);
	//VistaAnimacion* animacion = new VistaAnimacion(lista,1000,ANCHO_SPRITE,ALTO_SPRITE,true,10);
	//VistaAnimacion* animacion = animaciones->get(lista.front());
	//animaciones->setPantalla(pantalla);
	//animacion->graficar(0,0);
	SDL_Rect rcPasto;

	// Estructura para los eventos generales (teclado, mouse, etc.)
	SDL_Event event;

	Uint8 *estadoTecla;
	int colorkey, finJuego;


	temp  = SDL_LoadBMP(SDL_RUTA_PASTO);
	pasto = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);

	// Pongo fin del juego en falso
	finJuego = 0;
	double x = 0;
	double y = 0;

	// Tomo las acciones
	while (!finJuego)
	{
	// Miro si hay un evento <event>
	if (SDL_PollEvent(&event)) {
	  // Encontr� un evento
	  switch (event.type) {
		// Se presion� el bot�n de salir
		case SDL_QUIT:
		  finJuego = 1;
		  break;

		// Manejo eventos del teclado
		case SDL_KEYDOWN:
		  switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
			case SDLK_q:
			  finJuego = 1;
			  break;
		  }
		  break;
	  }
	}

	// Manejo el movimiento del Sprite
	estadoTecla = SDL_GetKeyState(NULL);
	if (estadoTecla[SDLK_LEFT] ) {
	  x -= 1;
	}
	if (estadoTecla[SDLK_RIGHT] ) {
	  x += 1;
	  vEntidad->setAnimacion(listaE.front());
	}
	if (estadoTecla[SDLK_UP] ) {
	  y -= 1;
	}
	if (estadoTecla[SDLK_DOWN] ) {
	  y += 1;
	  vEntidad->setAnimacion(lista.front());
	}

	// Hago conincidir con los bordes de la pantalla
	if ( x < 0 ) {
	  x = 0;
	}
	else if ( x > PANTALLA_ANCHO-ANCHO_SPRITE ) {
	  x = PANTALLA_ANCHO-ANCHO_SPRITE;
	}
	if ( y < 0 ) {
	  y = 0;
	}
	else if ( y > PANTALLA_ALTO-ALTO_SPRITE ) {
	  y = PANTALLA_ALTO-ALTO_SPRITE;
	}

	// Dibujo el pasto
	for (int x = 0; x < PANTALLA_ANCHO / TAMANIO_SPRITE; x++) {
	  for (int y = 0; y < PANTALLA_ALTO / TAMANIO_SPRITE; y++) {
		rcPasto.x = x * TAMANIO_SPRITE;
		rcPasto.y = y * TAMANIO_SPRITE;
		SDL_BlitSurface(pasto, NULL, pantalla, &rcPasto);
	  }
	}

	// Dibujo el sprite
	vEntidad->graficar();
	// Refresco la pantalla
	SDL_UpdateRect(pantalla, 0, 0, 0, 0);
	}

//	delete animacion;
	// Limpio la superficie
	SDL_FreeSurface(pasto);

	// Salgo del SDL
	SDL_Quit();

}