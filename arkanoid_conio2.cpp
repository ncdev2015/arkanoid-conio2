#include <iostream>
#include <ctime>
#include <constream>
#include <cstdlib>
#include <stdlib.h>
#define N 21 // Dimension X del recuadro
#define M 81 // Dimension Y del recuadro
#define TAMANIO 7 // longitud del ladrillo
#define NLADRILLOS 10 // cantidad de ladrillos
#define SEPARACION 1 // Separacion entre bloques
#define POSINICIAL 7 // Posicion en X donde comienza el primer bloque

using namespace std;
using namespace conio;

typedef struct {
	char ladrillo[TAMANIO];
	int color;
	int nivel; // nivel 2: 2 golpes para borrar el bloque; nivel 1: 1 golpe para borrar el bloque
	int posX;
	int posY;
} Bloque;

void dibujarLadrillo(Bloque unBloque);
void activarColision(Bloque *unBloque, int * px, int* py, int* diy, int pxa, int pya, int *puntaje);
void inicializarBloques(Bloque bloques[NLADRILLOS], int *posInicial);
void dibujarBloques(Bloque bloques[NLADRILLOS]);
void activarColisionBloques(Bloque bloques[NLADRILLOS], int *px, int *py, int *diy, int pxa, int pya, int *puntaje);
void cambiarColor(Bloque *unBloque, int color);
void desaparecerDelMapa(Bloque *unBloque);
void modificarBloque(Bloque *unBloque, int *px, int *py, int *diy, int pxa, int pya);
bool bloquesEstanBorrados(Bloque bloques[NLADRILLOS]);

int main(int argc, char *argv[]) {
	bool salir = false, gano = false;
	int puntaje = 0;
	int vidas = 5;
	int posInicial = POSINICIAL;
	Bloque bloques[NLADRILLOS];	
	inicializarBloques(bloques, &posInicial);
	
	// Mapa donde rebotara la pelotita	
	char map[N][M] = {
		"²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²",
		"²                                                                            ²",
		"²                                                                            ²",
		"²                                                                            ²",
		"²                                                                            ²",
		"²                                                                            ²",
		"²                                                                            ²",
		"²                                                                            ²",
		"²                                                                            ²",
		"²                                                                            ²",
		"²                                                                            ²",
		"²                                                                            ²",
		"²                                                                            ²",
		"²                                                                            ²",
		"²                                                                            ²",
		"²                                                                            ²",
		"²                                                                            ²",
		"²                                                                            ²",
		"²                                                                            ²",
		"²                                                                            ²",
		"²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²²"
	};
	
	int i, j;
	
	// limites de los bordes del mapa
	int bizq = 3;
	int bsup = 3;
	int bder = 78;
	int binf = 21;
	
	int x; // coordenada x de la posicion de la barra
	int y; // coordenada y de la posicion de la barra
	int tam = 6; // tamanio de la barra
	
	char t; // caracter de la tecla
	
	int px, py; // coordenadas de la pelota
	int pxa, pya; // coordenadas de la pelota anterior posicion
	int dix, diy; // direccion de la pelota
	
	clock_t paso = CLOCKS_PER_SEC/10; // cada cuanto se permite mover la pelotita -  n veces por segundo
	clock_t tempo; // variable para ajustar la velocidad de movimiento de la pelotita
	
	// hacemos desaparecer el cursor
	_setcursortype(_NOCURSOR);
	
	srand( time( NULL ) );
	
	// nroAleatorio entre 1 y 10
	int nroRandom = 1 + rand() % 10;
	
	// posicion inicial de la pelotita
	py = 17;
	px = 39;
	
	// direccion inicial de la pelotita	
	if (nroRandom % 2 == 0)
		dix = 1;
	else
		dix = -1;
	
	diy = -1;
	
	// inicializamos la variable tempo con el reloj
	tempo = clock();
	
	// posicion de la barra
	x = 37;
	y = 18;
	
	// imprimimos el recuadro
	for( i=0; i<N; ++i )
	{
		for( j=0; j<M; ++j )
		{
			gotoxy( 2+j , 2+i );
			cout << map[i][j];
		}
	}
	
	// bucle principal del juego
	while ( !salir && !gano) {		
		// impresion de la barra, coloreamos 6 espacios de rojo
		cout << setxy( x, y ) << setbk( LIGHTRED ) << "      " << setbk( BLACK) << setxy( 200,200 );
		
		// borramos el extremo izquierdo
		if ( x > bizq ) {
			cout << setxy( x - 1 , y ) << " ";
		}
		
		// borramos el extremo derecho
		if ( x < bder - tam + 1 ) {
			cout << setxy( x + tam, y ) << " ";
		}
		
		// deteccion del teclado
		if ( kbhit() ) {
			// leemos la tecla
			t = getch();
			
			// nos movemos a la izquierda
			if ( t == 75 ) {
				--x;
				
				// si nos pasamos del borde izquierdo de la pantalla
				if ( x < bizq ) {
					x = bizq;
				}
			}
			
			if ( t == 77 ) {
				++x;
				
				// si nos pasamos del borde derecho de la pantalla
				if ( x > bder - tam + 1) {
					x = bder - tam + 1;
				}
			}
		}
		
		// movimiento de la pelotita
		if ( tempo + paso < clock() ) {
			
			// guardamos la posicion anterior de la pelotita para borrarla
			pxa = px;
			pya = py;
			
			// avanzamos la pelotita
			px = px + dix;
			py = py + diy;
			
			// vemos si llega al borde derecho
			if ( px > bder ) {
				// rebote en el borde derecho
				dix = -dix;
				px = bder;
			}
			
			// vemos si llega al borde izquierdo
			if ( px < bizq ) {
				// rebote en el borde derecho
				dix = -dix;
				px = bizq;
			}
			
			// vemos si llega al borde superior
			if ( py < bsup ) {
				// rebote arriba
				diy = -diy;
				py = bsup;
			}
			
			// vemos si llega al borde inferior
			if ( py > binf ) {
				diy = -diy;
				py = binf;
				--vidas;
			}
			
			// rebote en la barra, vemos si entra dentro de la barra
			if ( px >= x && px <= x + tam && py == y  ) {
				diy = -diy;
				// devolvemos la posicion anterior
				px = pxa;
				py = pya;
			}
			
			// borramos la posicion anterior
			cout << setxy( pxa, pya ) << " "; 
			// imprimimos la pelotita
			cout << setxy( px, py ) << "O" << setbk( BLACK ) << setxy( 200, 200 );
			// tomamos el tiempo para saber la proxima vez que moveremos la pelotita
			tempo = clock();
			
			// imprimimos los ladrillos
			dibujarBloques(bloques);
			activarColisionBloques(bloques,&px, &py, &diy, pxa, pya, &puntaje);
			
			cout << setxy(2,24) << "PUNTAJE: " << puntaje << "\t" << "VIDAS: " << vidas;
			
			if (vidas <= 0) {
				salir = true;
			}
			
			if ( bloquesEstanBorrados(bloques) ) {
				gano = true;
			}
		}
	}
	
	system("cls");
	if (vidas <= 0) 
		cout << "Fin del juego, Perdiste!" << endl;
	
	if (gano)
		cout << "Felicitaciones! GANASTE!! Puntaje: " << puntaje << endl;
	system("pause");
	
	return 0;
}

void dibujarLadrillo(Bloque unBloque) {
	textcolor(unBloque.color);			
	cout << setxy(unBloque.posX,unBloque.posY) << unBloque.ladrillo << setxy(200,200);
	textcolor(LIGHTGRAY);
}

void activarColision(Bloque *unBloque, int *px, int *py, int *diy, int pxa, int pya, int *puntaje ) {
	// Activamos la colision solamente si el nivel del bloque es mayor a cero
	if ( (*unBloque).nivel > 0 ) {
		if ( *px >= (*unBloque).posX && *px <= ((*unBloque).posX + TAMANIO - 2) && *py == (*unBloque).posY ) {
			
			*diy = -*diy;
			*px = pxa;
			*py = pya;
			
			// Obtenemos el puntaje
			if ( (*unBloque).nivel == 2 ) {
				*puntaje += 20;
			}
			
			if ( (*unBloque).nivel == 1 ) {
				*puntaje += 10;
			}
			
			modificarBloque(unBloque, px, py, diy, pxa, pya);
		}
	}
}

void inicializarBloques(Bloque bloques[NLADRILLOS], int *posInicial) {
	int i;
	bloques[0].posX = 0;
	
	for (i=0; i<NLADRILLOS; i++) {
		strcpy(bloques[i].ladrillo,"²²²²²²");
		if  (i % 2 == 0) {
			bloques[i].color = LIGHTBLUE;
			bloques[i].nivel = 2;
		}
		else {
			bloques[i].color = LIGHTCYAN;
			bloques[i].nivel = 1;
		}
		
		bloques[i].posX = *posInicial;
		*posInicial += (TAMANIO - 1) + SEPARACION;
		bloques[i].posY = 5 ;
	}
}

void dibujarBloques(Bloque bloques[NLADRILLOS]) {
	int i;
	
	for (i=0; i<NLADRILLOS; i++) {
		dibujarLadrillo(bloques[i]);
	}
}

void activarColisionBloques(Bloque bloques[NLADRILLOS], int *px, int *py, int *diy, int pxa, int pya, int *puntaje) {
	int i;
	
	for (i=0; i<NLADRILLOS; i++) {
		activarColision(&bloques[i], px, py, diy, pxa, pya, puntaje);
	}
}

void cambiarColor(Bloque *unBloque, int color) {
	(*unBloque).color = color;
}

void desaparecerDelMapa(Bloque *unBloque) {
	cambiarColor(unBloque, BLACK);
}

void modificarBloque(Bloque *unBloque, int *px, int *py, int *diy, int pxa, int pya) {
	(*unBloque).nivel--;
	
	// Color CYAN
	if ( (*unBloque).nivel == 1) {
		cambiarColor(unBloque, LIGHTCYAN);
	}
	// Color BLACK
	if ( (*unBloque).nivel == 0 ) {
		desaparecerDelMapa(unBloque);
	}
}

bool bloquesEstanBorrados(Bloque bloques[NLADRILLOS]) {
	bool salir = false;
	int i;
	
	for (i=0; i<NLADRILLOS && !salir; i++) {
		// Si hay algun bloque con nivel distinto de cero
		if ( bloques[i].nivel != 0 ) {
			salir = true;
		}
	}
	
	return !salir;
}
