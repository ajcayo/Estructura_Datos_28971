#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

int ingresarNumeros(char*);
int ingresarNumeros(char * msj){
	
	printf ("%s",msj);
	char dat[10];
	char  c;                                                                                                                                                    c;
	int i=0,valor;
	while((c=getch())!=13){
		if (c>='0'&& c<'9'){
			printf("%c",c);
			dat[i++]=c;
		}
		
	}
	dat[i]='\0';
	valor=atoi(dat);
    return valor;   
}
int main (int argc, char** argv){
	int valor;
	valor=ingresarNumeros("Ingrese el numero:");
	printf ("El dato ingresado es: %d",valor);
	return 0;
}
    