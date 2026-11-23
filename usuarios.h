#ifndef USUARIOS_H
#define USUARIOS_H

#include "estructuras.h"


int agregarUsuario(Usuario **u, int *n, int *cap);
int modificarUsuario(Usuario *u, int n);
int eliminarUsuario(Usuario *u, int n);
void mostrarUsuarios(const Usuario *u, int n);

void encabezadoUsuarios(void);
void imprimirUsuario(const Usuario *u);

#endif
