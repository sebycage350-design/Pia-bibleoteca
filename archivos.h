#ifndef ARCHIVOS_H
#define ARCHIVOS_H

#include "estructuras.h"

#define ARCHIVO_LIBROS    "libros.dat"
#define ARCHIVO_USUARIOS  "usuarios.dat"
#define ARCHIVO_PRESTAMOS "prestamos.dat"

Libro    *cargarLibros(int *n, int *cap);
Usuario  *cargarUsuarios(int *n, int *cap);
Prestamo *cargarPrestamos(int *n, int *cap);

int guardarLibros(const Libro *l, int n);
int guardarUsuarios(const Usuario *u, int n);
int guardarPrestamos(const Prestamo *p, int n);

void *crecerArreglo(void *arr, int n, int *cap, size_t tam);

#endif
