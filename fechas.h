#ifndef FECHAS_H
#define FECHAS_H

#include "estructuras.h"

Fecha fechaActual(void);
int   diferenciaDias(Fecha desde, Fecha hasta);   
Fecha sumarDias(Fecha f, int dias);
void  fechaATexto(Fecha f, char *dest);         

#endif
