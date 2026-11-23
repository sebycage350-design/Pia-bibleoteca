#ifndef BUSQUEDA_H
#define BUSQUEDA_H

#include "estructuras.h"

#define ORDEN_TITULO    1
#define ORDEN_AUTOR     2
#define ORDEN_CATEGORIA 3

int buscarLibroPorId(const Libro *l, int n, int id);
int buscarUsuarioPorMatricula(const Usuario *u, int n, int matricula);

void buscarLibroPorTitulo(const Libro *l, int n);
void buscarLibroPorAutor(const Libro *l, int n);
void buscarLibroPorCategoria(const Libro *l, int n);
void buscarUsuario(const Usuario *u, int n);

/*libros activos ordenados, pero no se modifica el arreglo original */
void mostrarLibrosOrdenados(const Libro *l, int n, int criterio);

#endif
