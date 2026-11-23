#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


#define MAT_MAX        999999999
#define ID_LIBRO_MAX   999999
#define EJEMPLARES_MAX 1000
#define ID_PRESTAMO_MAX 999999


#define MAX_LIBROS_USUARIO        3   
#define DIAS_PRESTAMO             7  
#define MULTA_POR_DIA             5   /* $ por dia   */
#define DIAS_REACTIVACION_USUARIO 30  /* plazo para reactivar a un usuario  */


#define ESTADO_ACTIVO   "activo"
#define ESTADO_DEVUELTO "devuelto"

typedef struct {
    int dia;
    int mes;
    int year;
} Fecha;

typedef struct {
    char nombre[80];
    int matricula;
    char telefono[15];
    int librosPrestados;
    int multaPendiente;
    int activo;
    Fecha fechaBaja;      /* solo tiene valor cuando activo == 0 */
} Usuario;

typedef struct {
    int id;
    char titulo[100];
    char autor[60];
    char categoria[30];
    int ejemplaresTotales;
    int ejemplaresDisponibles;
    int activo;
} Libro;

typedef struct {
    int id;
    int idLibro;
    int matricula;
    Fecha fechaPrestamo;
    Fecha fechaLimite;
    Fecha fechaDevolucion;   /* en ceros mientras el prestamo siga abierto */
    char estado[15];
    int multa;
} Prestamo;

#endif
