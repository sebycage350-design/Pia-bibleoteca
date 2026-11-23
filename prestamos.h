#ifndef PRESTAMOS_H
#define PRESTAMOS_H

#include "estructuras.h"


int prestarLibro(Libro *l, int nl, Usuario *u, int nu, Prestamo **p, int *np, int *cap);
int devolverLibro(Libro *l, int nl, Usuario *u, int nu, Prestamo *p, int np);
int pagarMulta(Usuario *u, int nu);


int calcularMulta(Fecha limite, Fecha devolucion);

#endif
