#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
using namespace std;
void encerar(float**, int,int );
void ingresar(float**,int,int );
void imprimir(float**,int,int );
void procesar(float*, float,float*,int,int );

void encerar(float **mat,int fila, int col){
    int i,j;
    for(i=0;i<fila;i++){
         for(j=0;j<col;j++){
            mat[i][j]=0;
        }
    }
}

void imprimir(float **mat,int fila, int col){
    int i,j;
    for(i=0;i<fila;i++){
         for(j=0;j<col;j++){
            cout<< mat[i][j] <<" ";
        }
        cout<<endl;
    }

}
void ingresar(float **mat,int fila, int col){
    int i,j,valor;
    for(i=0;i<fila;i++){
         for(j=0;j<col;j++){
            cout<<"Ingrese ["<<i<<"]["<<j<<"]:";
            cin>>mat[i][j];
        }
    }

}
int main()
{
    int fila=3,col=3;
    float **mat;
    mat =(float**)malloc(sizeof(float)*fila);
    for(int i=0;i<fila;i++){
        mat[i]=(float*)malloc(sizeof(float)*col);
    }
    encerar(mat,fila,col);
    imprimir(mat,fila,col);
    ingresar(mat,fila,col);
    imprimir(mat,fila,col);
}