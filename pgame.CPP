#include <graphics.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include <stdio.h>
#include <time.h>
#include <io.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define JOY_1_X 0x01
#define JOY_1_Y 0x02

#define BUTTON_1 0x10
#define BUTTON_2 0x20

#define MAX_COUNT 10000


//************** ESTRUCTURAS *******************
struct Tipo_ca_rep {    		// CALIBRACION DE SENSORES
	long puerto;                // Y DIRECCION DE PUERTO
	int amin,amax,bmin,bmax;
}; typedef Tipo_ca_rep *Tipo_ca;
//***********************************************


//********* CABECERAS DE FUNCIONES **************
Tipo_ca Crear();
void Liberar(Tipo_ca *viej);
void InG();
void CG();
void Presentacion();
void Pegar(int pos,int color);
int joystick (int command,long int puerto);
long  Conversor(char *puertochar);
char* Inversa(long puerto);
void Presentacion2();
void Pegar2(int pos,int color);
void Marco();
void Barra_Ca(int pro);
void Calibrar (Tipo_ca calibracion);
void Abrir(Tipo_ca calibracion);
void Cuadro(int xini,int yini, int xfin, int yfin,int color);
void Ejecutar(Tipo_ca cal);
void Cuadrado (int xini,int yini, int xfin, int yfin,int color);
void Ventana (int xini,int yini, int xfin, int yfin,int color);
int Pix(int max,int min,int pos);
void Barra(int y,int y_ant,int xini,int xfin);
void Xcen_alarm(int *min,int *max);
void Menu();
//***********************************************


//********* PROGRAMA PRINCIPAL ******************
void main(void) {
  textcolor(15);
  textbackground(0);
  clrscr();
  Menu();
  clrscr();
}
//***********************************************


//********** DESARROLLO DE FUNCIONES ************
Tipo_ca Crear() {
	Tipo_ca nuev;

	nuev=(Tipo_ca) malloc(sizeof(struct Tipo_ca_rep));
	if (!nuev) {
		clrscr();
		cprintf("Error, no hay memoria.\n");
		exit(1);
	}
	nuev->puerto=0x201;
	nuev->amin=-1;
	nuev->amax=nuev->bmin=nuev->bmax=0;
	return(nuev);
}
//-----------------------------------------------
void Liberar(Tipo_ca *viej) {
	free(*viej);
	(*viej)=NULL;
}
//-----------------------------------------------
void InG() {
// INICIA EL PROGRAMA EN MODO GRAFICO

	int gdriver = DETECT, gmode, errorcode;

	initgraph(&gdriver, &gmode, "");
	errorcode = graphresult();

	if (errorcode != grOk)  {

		clrscr();
		textcolor(4);
		printf("Error grafico: %s\n", grapherrormsg(errorcode));
		printf("Presione una tecla para salir:");
		getch();
		exit(1);
	}

}
//-----------------------------------------------
void CG() {
// RESTAURA EL MODO GRAFICO QUE HABIA
// ANTERIOR AL USADO ACTUALMENTE

	restorecrtmode();

}
//-----------------------------------------------
void Presentacion() {
// IMPRIME POR PANTALLA EL MENU DEL PROGRAMA

	int i;

	textbackground(0); // FONDO NEGRO
	clrscr();
	textbackground(4); // FONDO ROJO
	textcolor(15);     // LETRAS BLANCO
	gotoxy(22,6);  cprintf("ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿");
	gotoxy(22,7);  cprintf("³          CALIBRAR          ³");
	gotoxy(22,8);  cprintf("ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´");
	gotoxy(22,9);  cprintf("³     ABRIR  CALIBRACION     ³");
	gotoxy(22,10); cprintf("ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´");
	gotoxy(22,11); cprintf("³          EJECUTAR          ³");
	gotoxy(22,12); cprintf("ÃÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´");
	gotoxy(22,13); cprintf("³            SALIR           ³");
	gotoxy(22,14); cprintf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ");
	textcolor(7); // LETRAS GRISES
	gotoxy(23,15);  cprintf("ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ");

	for (i=7;i<=15;i++) {
		gotoxy(52,i);
		cprintf("ÛÛ");
	}
	textcolor(15);  // LETRAS BLANCAS

}
//-----------------------------------------------
void Pegar(int pos,int color) {
// REPRESENTACION EN PANTALLA DE LA OPCION ELEGIDA
// POR EL USUARIO. "pos" ES LA SELECCION ELEGIDA POR EL USUARIO

	textbackground(color);
	switch (pos) {

		case 1:
			gotoxy(32,7);
			cprintf(" CALIBRAR ");
			break;
		case 2:
			gotoxy(27,9);
			cprintf(" ABRIR  CALIBRACION ");
			break;
		case 3:
			gotoxy(32,11);
			cprintf(" EJECUTAR ");
			break;
		case 4:
			gotoxy(34,13);
			cprintf(" SALIR ");
			break;
		}

}
//-----------------------------------------------
int joystick (int command, long int puerto) {
// FUNCION QUE CAPTURA LOS VALORES DEL PUERTO SEGUN EL
// COMANDO INTRODUCIDO POR COMMAND Y LA VARIABLE PUERTO

  unsigned int count=0;

  if ( command < 0x10 ) {

	disable();
	outportb(puerto,command);

	while ( inportb(puerto)&command&&count < MAX_COUNT )

	  count++;

	enable();
	return count;

  } else {

	return((inportb(puerto)&command?0:1));

  }

}
//-----------------------------------------------
long Conversor(char *puertochar){
// CONVIERTE UNA CADENA DE CARACTERES QUE REPRESENTA UN
// NUMERO HEXADECIMAL EN UN NUMERO DECIMAL

	int tama,i,j,exp;
	char hexa[15];
	long resultado=0;

	strcpy(hexa,"0123456789ABCDEF");

	tama= strlen(puertochar);
	exp=tama-1;

	for (i=0;i<tama;i++) {

		j=0;

		while ( hexa[j]!=toupper(puertochar[i]) ) j++;

		resultado=resultado + (j * pow(16,exp) );
		exp--;

	}
	return(resultado);

}
//-----------------------------------------------
char* Inversa(long puerto)  {
// CONVIERTE UN NUMERO EN UNA CADENA DE CARACTERES
// DE TIPO HEXADECIMAL

	char hexa[17];
	char *resultado;
	long i=3;
	long cociente,resto;

	strcpy(resultado,"0000");

	if ( puerto < 0xffff ) {
		strcpy(hexa,"0123456789ABCDEF");

		while ( puerto > 15 ) {
			 cociente=puerto/16;
			 resto=puerto-(cociente*16);
			 puerto=cociente;
			 resultado[i]=hexa[resto];
			 i--;
		}

		resultado[i]=hexa[puerto];
	}

	return(resultado);
}
//-----------------------------------------------
void Presentacion2() {
// IMPRIME POR PANTALLA UN SUBMENU

	int i;

	textbackground(0);
	clrscr();
	textbackground(4);
	textcolor(15);

	gotoxy(15,6);  cprintf("ÚÄÄÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÄ¿");
	gotoxy(15,7);  cprintf("³ PUERTO   ³         ³");
	gotoxy(15,8);  cprintf("ÃÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄ´");
	gotoxy(15,9);  cprintf("³ CALIBRAR ³         ³");
	gotoxy(15,10); cprintf("ÃÄÄÄÄÄÄÄÄÄÄÅÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿");
	gotoxy(15,11); cprintf("³ GUARDAR  ³                                    ³");
	gotoxy(15,12); cprintf("ÀÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ");
}
//-----------------------------------------------
void Pegar2(int pos,int color) {
// REPRESENTACION EN PANTALLA DE LA OPCION ELEGIDA
// POR EL USUARIO. "pos" ES LA SELECCION ELEGIDA POR EL USUARIO

	textbackground(color);
	switch (pos) {

		case 1:
			gotoxy(16,7);
			cprintf(" PUERTO ");
			break;
		case 2:
			gotoxy(16,9);
			cprintf(" CALIBRAR ");
			break;
		case 3:
			gotoxy(16,11);
			cprintf(" GUARDAR ");
			break;

		}

}
//-----------------------------------------------
void Marco() {
// ESCRIBE UN MARCO EN PANTALLA
	gotoxy(15,17); cprintf("ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿");
	gotoxy(15,18); cprintf("³                                         ³");
	gotoxy(15,19); cprintf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ");

}
//-----------------------------------------------
void Barra_Ca(int pro){
// IMPRIME LA PROGRESION DE UNA BARRA
	int valor=0;

	valor=pro/60; // valor 47
	if ( ( valor >= 18 ) && ( valor <=54 ) ){
		gotoxy(valor,18);
		cprintf("Û");
	}


}
//-----------------------------------------------
void Calibrar(Tipo_ca calibracion) {
// DEVUELVE LA CALIBRACION DE LOS SENSORES Y LA DIRECCION DEL PUERTO
// INTRODUCIDA POR EL PUERTO

	int valor=0, pos=1,i=0;
	time_t comienzo;
	int opc=' ';
	char puertochar[10],ruta[34];
	FILE *fcal;
	Tipo_ca aux;


	aux=Crear();
	Presentacion2();
	textcolor(15);
	Pegar2(1,1);
	gotoxy(27,7);
	cprintf("        ");
	gotoxy(27,7);
	aux->amin=calibracion->amin;
	aux->amax=calibracion->amax;
	aux->bmin=calibracion->bmin;
	aux->bmax=calibracion->bmax;
	aux->puerto=calibracion->puerto;

 	cprintf(" %s",Inversa(calibracion->puerto));
	calibracion->amin=aux->amin;
	calibracion->amax=aux->amax;
	calibracion->bmin=aux->bmin;
	calibracion->bmax=aux->bmax;
	calibracion->puerto=aux->puerto;


 	if (calibracion->amin == -1 ) {   // SI NO ESTA CALIBRADO

		gotoxy(27,9); cprintf(" NO     ");  // IMPRIME NO

	} else {                          // SINO

		gotoxy(27,9); cprintf(" SI    ");   // IMPRIME SI

	}

	gotoxy(27,11); cprintf(" NO                               ");

	while (  ( opc != '\x1B') ) {

		opc=getch();

		if ( ( opc == 'H' ) && ( pos > 1 ) ) {

			pos--;
			Pegar2(pos,1);
			Pegar2(pos+1,4);

		}

		if ( ( opc == 'P' ) && ( pos < 3 ) ) {

			pos++;
			Pegar2(pos,1);
			Pegar2(pos-1,4);

		}


		if ( opc == '\r') {

			switch (pos) {

				case 1:   //CONFIGURAR EL PUERTO

					textbackground(1);
					gotoxy(28,7);
					cprintf("       ");
					_setcursortype(_SOLIDCURSOR);
					gotoxy(28,7);
					gets(puertochar);
					_setcursortype(_NOCURSOR);
					calibracion->puerto=Conversor(puertochar);
					break;

				case 2:  //CALIBRAR LOS SENSORES

					/****** SENSOR_A ******/

					_setcursortype(_NOCURSOR);
					gotoxy(15,15);
					textbackground(0);
					cprintf("Pulse una tecla para calibrar el SENSOR_A");
					getch();
					textbackground(1);
					Marco();
					calibracion->amin=joystick(JOY_1_X,calibracion->puerto);
					calibracion->amax=calibracion->amin;
					textcolor(14);
					i=0;
					comienzo=time(NULL);

					while ( ( time(NULL)-comienzo ) <= 4 ) {
						  // CALIBRA EL SENSOR EN 4 SEGUNDOS
						valor=joystick(JOY_1_X,calibracion->puerto);

						if ( valor < calibracion->amin ) calibracion->amin=valor;

						if ( valor > calibracion->amax ) calibracion->amax=valor;

						i++;
						Barra_Ca(i);

					}

					if ( calibracion->amax < 9500 )
						calibracion->amax= calibracion->amax+500;

					if ( calibracion->amin > 200 )
						calibracion ->amin=calibracion->amin-200;

					/****** SENSOR_A ******/

					gotoxy(15,15);
					textcolor(15);
					textbackground(0);
					cprintf("Pulse una tecla para calibrar el SENSOR_B");
					gotoxy(15,17);
					cprintf("                                           ");
					gotoxy(15,18);
					cprintf("                                           ");
					gotoxy(15,19);
					cprintf("                                           ");
					getch();
					textbackground(1);
					Marco();

					calibracion->bmin=joystick(JOY_1_Y,calibracion->puerto);
					calibracion->bmax=calibracion->bmin;
					i=0;
					textcolor(14);
					comienzo=time(NULL);

					while ( ( time(NULL)-comienzo ) <= 4 ) {
							 // CALIBRA EL SENSOR EN 4 SEGUNDOS

						valor=joystick(JOY_1_Y,calibracion->puerto);

						if ( valor < calibracion->bmin ) calibracion->bmin=valor;

						if ( valor > calibracion->bmax ) calibracion->bmax=valor;

						i++;
						Barra_Ca(i);

					}

					if ( calibracion->bmax < 9500 )
						calibracion->bmax=calibracion->bmax+500;

					if ( calibracion->bmin > 200 )
						calibracion->bmin=calibracion->bmin-200;

					textbackground(0);
					gotoxy(15,15);
					cprintf("                                         ");
					gotoxy(15,17);
					cprintf("                                           ");
					gotoxy(15,18);
					cprintf("                                           ");
					gotoxy(15,19);
					cprintf("                                           ");
					textbackground(1);
					textcolor(15);
					gotoxy(27,9); cprintf(" SI     ");


					break;

				case 3:  //GUARDAR LA CONFIGURACION

					textbackground(1);
					gotoxy(28,11);
					cprintf("                 ");

					_setcursortype(_SOLIDCURSOR);
					gotoxy(28,11);
					gets(ruta);                 // PIDE LA RUTA DEL FICHERO
					_setcursortype(_NOCURSOR);  // A GUARDAR

					if ( ( fcal= fopen(ruta,"wb" ) ) == NULL ) {
												   // ABRE EL FICHERO
						gotoxy(15,20);
						textbackground(0);
						textcolor(4);
						cprintf("ERROR. NO SE PUEDE ABRIR EL FICHERO");
						gotoxy(15,21);
						cprintf("PULSE UNA TECLA PARA SALIR.");
						opc=getch();

					}else{   // GUARDA EL FICHERO

						fwrite(&calibracion->puerto,sizeof(calibracion->puerto),1,fcal);
						fwrite(&calibracion->amin,sizeof(calibracion->amin),1,fcal);
						fwrite(&calibracion->amax,sizeof(calibracion->amax),1,fcal);
						fwrite(&calibracion->bmin,sizeof(calibracion->bmin),1,fcal);
						fwrite(&calibracion->bmax,sizeof(calibracion->bmax),1,fcal);
						fclose(fcal);
						textbackground(0);
						textbackground(1);
						gotoxy(27,11); cprintf(" SI              ");
					}

					break;

				}



		}


	}
	Liberar(&aux);

}
//-----------------------------------------------
void Abrir(Tipo_ca calibracion) {
// RECUPERA LA CALIBRACION DE UN FICHERO

	char ruta[80];
	FILE *fcal;

	textbackground(0);
	clrscr();
	textcolor(15);
	gotoxy(15,7); cprintf("Fichero:");
	textbackground(4);
	gotoxy(15,8);  cprintf("ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿");
	gotoxy(15,9);  cprintf("³                          ³");
	gotoxy(15,10); cprintf("ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ");
	textbackground(1);
	gotoxy(16,9);  cprintf("                          ");
	_setcursortype(_SOLIDCURSOR);
	gotoxy(16,9);
	gets(ruta);
	_setcursortype(_NOCURSOR);
	if ( ( fcal= fopen(ruta,"rb" ) ) == NULL ) {
									// SE ABRE EL FICHERO
		gotoxy(15,20);
		textbackground(0);
		textcolor(4);
		cprintf("ERROR. NO SE PUEDE ABRIR EL FICHERO");
		gotoxy(15,21);
		cprintf("PULSE UNA TECLA PARA SALIR.");
		getch();

	}else{                 // SE LEE EL FICHERO
		fread(&calibracion->puerto,sizeof(calibracion->puerto),1,fcal);
		fread(&calibracion->amin,sizeof(calibracion->amin),1,fcal);
		fread(&calibracion->amax,sizeof(calibracion->amax),1,fcal);
		fread(&calibracion->bmin,sizeof(calibracion->bmin),1,fcal);
		fread(&calibracion->bmax,sizeof(calibracion->bmax),1,fcal);
		fclose(fcal);
	}
}
//-----------------------------------------------
void Cuadro(int xini,int yini, int xfin, int yfin,int color) {
// IMPRIME UN CUADRO EN MODO GRAFICO INTRODUCIENDOLE
// LAS COORDENADAS Y EL COLOR
	setcolor(color);

	line(xini, yini,xini,yfin);
	line(xini,yini,xfin,yini);
	line(xfin,yini,xfin,yfin);
	line(xfin,yfin,xini,yfin);

}
//-----------------------------------------------
void Ventana (int xini,int yini, int xfin, int yfin,int color) {
// IMPRIME EN MODO GRAFICO UNA VENTANA INTRODUCIENDOLE
// LAS COORDENADAS Y EL COLOR

	int i;

	setcolor(color);

	for (i=xini;i<=xfin;i++) line(i, yini,i,yfin);

}
//-----------------------------------------------
int Pix(int max,int min,int pos) {
// DEVUELVE EL NUMERO DE LINEAS QUE DEBE DE PINTAR LA BARRA
// INTRODUCIENDOLE EL MAXIMO, EL MINIMO Y LA POSICION DEL SENSOR
	unsigned long int rango,pintar,npix;
	unsigned long int xcen;

	rango=max-min;  // RANGO DE LA CALIBRACION
	pintar=pos-min;
	xcen=pintar*10; // PORCENTAJE DE LA BARRA A IMPRIMIR
	xcen=xcen*10;
	xcen=xcen/rango;
	xcen=100-xcen;
	npix=(xcen*435)/100;
	npix=457-npix;  // NUMERO DE LINEAS A IMPRIMIR
	return(npix);

}
//-----------------------------------------------
void Barra(int y,int y_ant,int xini,int xfin) {
// IMPRIME UNA BARRA EN MODO GRAFICO
	int i;

	if ( y < y_ant ) {  // SI LA POSICION ES MAYOR QUE LA POSCICION ANTERIOR

		setcolor(YELLOW);   // CRECE LA BARRA

		for (i=y_ant;i>y;i--) line(xini,i,xfin,i);   // IMPRIME LINEAS
													 // AMARILLAS
	} else {           // SINO

		setcolor(BLACK);    // DECRECE LA BARRA

		for (i=y_ant;i<y;i++) line(xini,i,xfin,i);   // IMPRIME LINEAS
	}                                                // NEGRAS

}
//-----------------------------------------------
void Xcen_alarm(int *min,int *max) {
// DEVUELVE LOS LIMITES DE LA ALARMA
	unsigned int minn,maxx;
	int bien=0;

	while ( ! bien ) {

		clrscr();
		gotoxy(5,5); cprintf("ESCRIBA UN PORCENTAJE DE 0 A 100");
		gotoxy(5,6); cprintf("ALARMA MINIMA: ");
		_setcursortype(_SOLIDCURSOR);
		scanf("%d",&minn);
		gotoxy(5,7); cprintf("ALARMA MAXIMA: ");
		scanf("%d",&maxx);
		_setcursortype(_NOCURSOR);

		if ( minn >= maxx ) {

			gotoxy(5,9);
			cprintf("ESCRIBE LOS NUMEROS CORRECTAMENTE.");
			gotoxy(5,10);
			cprintf("PULSE UNA TECLA PARA CONTINUAR");
			getch();

		} else
			bien=1;

	}

	*min=minn;
	*max=maxx;

}
//-----------------------------------------------
void Ejecutar (Tipo_ca cal) {
// FUNCION QUE REPRESENTA LOS VALORES DEL SENSOR MEDIANTE
// UNA BARRA
	int bot1ant=0,bot1act=0,bot2ant=0,bot2act=0;
	int sensor_a=1,sensor_ant_a=0;
	int sensor,pixe,pixe_ant;
	int min=20,max=80;
	double al_a_min,al_a_max,al_b_min,al_b_max;
	unsigned int pix_min=87,pix_max=348;
	int opc= ' ';



	textbackground(BLACK);
	clrscr();
	textcolor(WHITE);
	gotoxy(5,5); cprintf("ALARMA MINIMA: 20%");    // CAMBIAR ALARMAS
	gotoxy(5,7); cprintf("ALARMA MAXIMA: 80%");
	gotoxy(5,9); cprintf("DESEA CAMBIAR LAS ALARMAS?? (S/N)");
	while ( ( ! ( toupper(opc) == 'S')  ) && ( ! ( toupper(opc) == 'N' ) ) )
		opc=getch();

	if ( toupper(opc) == 'S' ) Xcen_alarm(&min,&max);

	pix_min = min * 435;       /******************/
	pix_min = pix_min / 100;   /*  PIXEL MINIMO  */
	pix_min = 457 - pix_min;   /******************/

	pix_max = max * 435;       /******************/
	pix_max = pix_max / 100;   /*  PIXEL MAXIMO  */
	pix_max = 457 - pix_max;   /******************/

	al_a_min = cal->amax - cal->amin;  /************/
	al_a_min = max * al_a_min;         /*  LIMITE  */
	al_a_min = al_a_min / 100;         /* MINIMO_A */
	al_a_min = cal->amax - al_a_min;   /************/

	al_a_max = cal->amax - cal->amin;  /************/
	al_a_max = min * al_a_max;         /*  LIMETE  */
	al_a_max = al_a_max / 100;         /* MAXIMO_A */
	al_a_max = cal->amax - al_a_max;   /************/

	al_b_min = cal->bmax - cal->bmin;  /************/
	al_b_min = max * al_b_min;         /*  LIMITE  */
	al_b_min = al_b_min / 100;         /* MINIMO_B */
	al_b_min = cal->bmax - al_b_min;   /************/

	al_b_max = cal->bmax - cal->bmin;  /************/
	al_b_max = min * al_b_max;         /*  LIMITE  */
	al_b_max = al_b_max / 100;         /* MAXIMO_B */
	al_b_max = cal->bmax - al_b_max;   /************/

	clrscr();
	InG();
	_setcursortype(_NOCURSOR);
	Cuadro(0,0,250,479,WHITE);    // MARCO DEL SENSOR_A
	Cuadro(252,0,502,479,WHITE);  // MARCO DEL SENSOR_B
	Cuadro(505,0,639,479,RED);    // MARCO DE LOS BOTONES

	Cuadro(30,20,80,459,WHITE);
	setcolor(LIGHTRED);
	outtextxy(130,20,"SENSOR_A");

	Cuadro(282,20,332,459,LIGHTGRAY);
	setcolor(RED);
	outtextxy(382,20,"SENSOR_B");

	setcolor(CYAN);
	outtextxy(545,20,"BOTON_1");
	outtextxy(545,125,"BOTON_2");

	Ventana(545,50,600,70,GREEN);     // BOTON_1
	Ventana(545,155,600,175,GREEN);   // BOTON_2

	while ( opc != '\x1B' ) {   // MIENTRAS NO SE PULSE ESCAPE HACER

		if ( ( sensor_a ) && !(sensor_a == sensor_ant_a) ) {
			sensor_ant_a=sensor_a;
			Cuadro(0,0,250,479,WHITE);  // MARCO DEL SENSOR_A
			Cuadro(30,20,80,459,WHITE); // BARRA DEL SENSOR_A
			setcolor(LIGHTRED);
			outtextxy(130,20,"SENSOR_A");

			Cuadro(252,0,502,479,LIGHTGRAY);   // MARCO DEL SENSOR_B
			Cuadro(282,20,332,459,LIGHTGRAY);  // BARRA DEL SENSOR_B
			setcolor(RED);
			outtextxy(382,20,"SENSOR_B");
			setcolor(BLACK);
			outtextxy(367,60,"ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ");
			outtextxy(367,75,"ÛÛÛÛÛÛÛÛÛÛÛÛÛÛ");
			Ventana(285,22,329,457,BLACK);

		} else if ( ! ( sensor_a == sensor_ant_a ) ) {
			sensor_ant_a=sensor_a;
			Cuadro(0,0,250,479,LIGHTGRAY);  // MARCO DEL SENSOR_A
			Cuadro(30,20,80,459,LIGHTGRAY); // BARRA DEL SENSOR_A
			setcolor(RED);
			outtextxy(130,20,"SENSOR_A");
			setcolor(BLACK);
			outtextxy(115,60,"ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ");
			outtextxy(115,75,"ÛÛÛÛÛÛÛÛÛÛÛÛÛÛ");
			Ventana(33,22,77,457,BLACK);

			Cuadro(252,0,502,479,WHITE);  // MARCO DEL SENSOR_B
			Cuadro(282,20,332,459,WHITE); // BARRA DEL SENSOR_B
			setcolor(LIGHTRED);
			outtextxy(382,20,"SENSOR_B");

		}

		   setcolor(LIGHTRED);
		   line(25,pix_max,85,pix_max);    // ALARMA MAX DEL SENSOR_A
		   line(25,pix_min,85,pix_min);    // ALARMA MIN DEL SENSOR_A
		   line(277,pix_max,337,pix_max);  // ALARMA MAX DEL SENSOR_B
		   line(277,pix_min,337,pix_min);  // ALARMA MIN DEL SENSOR_B
		   pixe_ant=457;

		   while (!kbhit()) {  // MIENTRAS NO SE PULSE UNA TECLA HACER

			bot2act=joystick(BUTTON_2,cal->puerto);  // BOTON_2

			if ( bot2act != bot2ant ) { // SI EL BOTON PULSADO
										// ES IGUAL AL ANTERIOR HACER
				bot2ant=bot2act;
							   // SI ESTA PULSADO, IMPRIMIE UN CUADRO ROJO
				if (bot2act) { Ventana(545,50,600,70,RED);
							   // SI NO UN CUADRO VERDE
				} else Ventana(545,50,600,70,GREEN);

			}

			bot1act=joystick(BUTTON_1,cal->puerto);  // BOTON_1

			if( bot1act != bot1ant ) {

				bot1ant=bot1act;
				if (bot1act) { Ventana(545,155,600,175,RED);

				} else Ventana(545,155,600,175,GREEN);


			}

			if ( sensor_a ) {   // SI ESTA ACTIVADO EL SENSOR_A HACER

				sensor=joystick(JOY_1_X,cal->puerto);
									// CONTROL PARA QUE NO SOBREPASE LOS
				if ( sensor > cal->amax ) sensor=cal->amax; // LIMITES

				if ( sensor < cal->amin ) sensor=cal->amin;

				pixe=Pix(cal->amax,cal->amin,sensor);
				Barra(pixe,pixe_ant,33,77);
				pixe_ant=pixe;
				setcolor(LIGHTRED);
				line(25,pix_max,85,pix_max);    // ALARMA MAX DEL SENSOR_A
				line(25,pix_min,85,pix_min);    // ALARMA MIN DEL SENSOR_A

				if (sensor > al_a_max ) {  // SI SOBREPASA LA ALARMA  MAXIMA

					printf("\a");          // ACTIVARLA
					setcolor(LIGHTRED);
					outtextxy(115,60,"LUMINOSIDAD BAJA");
					outtextxy(115,75,"ENCENDER LUCES");

				} else {                   // SINO

					if (sensor < al_a_min ) {  // SI SOBREPARAS LA ALARMA
											   // MINIMA
						printf("\a");
						setcolor(LIGHTRED);    // ACTIVARLA
						outtextxy(115,60,"LUMINOSIDAD ALTA");
						outtextxy(115,75,"APAGAR LUCES");

					} else {               // SINO
						setcolor(BLACK);   // NO MOSTRAR NADA
						outtextxy(115,60,"ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ");
						outtextxy(115,75,"ÛÛÛÛÛÛÛÛÛÛÛÛÛÛ");

					}
				}

			} else {   // SINO (ESTA ACTIVADO EL SENSOR_B) HACER

				sensor=joystick(JOY_1_Y,cal->puerto);

				if ( sensor > cal->bmax ) sensor=cal->bmax;

				if ( sensor < cal->bmin ) sensor=cal->bmin;

				pixe=Pix(cal->bmax,cal->bmin,sensor);
				Barra(pixe,pixe_ant,285,329);
				pixe_ant=pixe;
				setcolor(LIGHTRED);
				line(277,pix_max,337,pix_max);  // ALARMA MAX DEL SENSOR_B
				line(277,pix_min,337,pix_min);  // ALARMA MIN DEL SENSOR_B

				if ( sensor > al_b_max ) {

					printf("\a");
					setcolor(LIGHTRED);
					outtextxy(367,60,"LUMINOSIDAD BAJA");
					outtextxy(367,75,"ENCENDER LUCES");

				} else {

					if ( sensor < al_b_min ) {

						printf("\a");
						setcolor(LIGHTRED);
						outtextxy(367,60,"LUMINOSIDAD ALTA");
						outtextxy(367,75,"APAGAR LUCES");

					} else {

						setcolor(BLACK);
						outtextxy(367,60,"ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ");
						outtextxy(367,75,"ÛÛÛÛÛÛÛÛÛÛÛÛÛÛ");

					}
				}

			}

		   }

		   opc=getch();   // SI SE PULSA EL TABULADOR

		   if ( opc== '\t' ) sensor_a = !sensor_a;  // CAMBIAR DE SENSOR

		}

		CG();
}
//-----------------------------------------------
void Menu() {
// MENU PRINCIPAL DONDE EL USUARIO SELECCIONA LA OPCION QUE DESEE

	int pos=1,salir=0;
	int opc;
	Tipo_ca calibracion;


	calibracion=Crear();
	_setcursortype(_NOCURSOR);
	Presentacion();
	textcolor(15);
	textbackground(1);
	Pegar(1,1);
	opc=' ';


	while (  ( opc!= '\x1B') && ( !salir ) ) {


		opc=getch();

		if ( ( opc == 'H' ) && ( pos > 1 ) ) {  // ABAJO

			pos--;
			Pegar(pos,1);
			Pegar(pos+1,4);

		}

		if ( ( opc == 'P' ) && ( pos < 4 ) ) {  // ARRIBA

			pos++;
			Pegar(pos,1);
			Pegar(pos-1,4);

		}



		if ( opc == '\r') {

			switch (pos) {

				case 1:  // CALIBRAR EL SENSOR

					Calibrar(calibracion);
					Presentacion();
					Pegar(1,1);
					pos=1;
					_setcursortype(_NOCURSOR);
					break;

				case 2:  // ABRIR UN FICHERO

					Abrir(calibracion);
					Presentacion();
					Pegar(1,1);
					pos=1;
					_setcursortype(_NOCURSOR);
					break;

				case 3:  // EJECUTAR LA APLICACION

					Ejecutar(calibracion);
					Presentacion();
					Pegar(1,1);
					pos=1;
					_setcursortype(_NOCURSOR);
					break;

				case 4:  // SALIR DEL PROGRAMA

					salir=1;
					break;

				}


		}


	}


	Liberar(&calibracion);

}
