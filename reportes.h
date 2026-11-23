#ifndef REPORTES_H
#define REPORTES_H

#include "estructuras.h"

#define ARCHIVO_REP_PRESTADOS "reporte_prestados.txt"
#define ARCHIVO_REP_VENCIDOS  "reporte_vencidos.txt"
#define ARCHIVO_REP_MULTAS    "reporte_multas.txt"
#define ARCHIVO_REP_POPULARES "reporte_populares.txt"
#define TOP_POPULARES 10


void reportePrestados(const Prestamo *p, int np, const Libro *l, int nl, const Usuario *u, int nu);
void reporteVencidos(const Prestamo *p, int np, const Libro *l, int nl, const Usuario *u, int nu);
void reporteMultas(const Usuario *u, int nu);
void reporteMasSolicitados(const Prestamo *p, int np, const Libro *l, int nl);

/* comprobante_prestamo_<id>.txt para cada prestamo */
void generarComprobante(const Prestamo *pr, const Libro *l, int nl, const Usuario *u, int nu);

#endif
