#ifndef LIBROS_H
#define LIBROS_H

#include "estructuras.h"

/* Las funciones que devuelven  1 si se cambio algo y 0 si se
   cancelo o fallo */
int agregarLibro(Libro **l, int *n, int *cap);
int modificarLibro(Libro *l, int n);
int eliminarLibro(Libro *l, int n);
void mostrarLibros(const Libro *l, int n);

void encabezadoLibros(void);
void imprimirLibro(const Libro *l);

#endif
