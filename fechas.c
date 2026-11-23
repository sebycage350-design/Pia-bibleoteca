#include "estructuras.h"
#include "fechas.h"

static time_t aTiempo(Fecha f) {
    struct tm t = {0};
    t.tm_mday = f.dia;
    t.tm_mon = f.mes - 1;
    t.tm_year = f.year - 1900;
    t.tm_hour = 12;          
    t.tm_isdst = -1;
    return mktime(&t);
}

static Fecha deTm(const struct tm *t) {
    Fecha f;
    f.dia = t->tm_mday;
    f.mes = t->tm_mon + 1;
    f.year = t->tm_year + 1900;
    return f;
}

Fecha fechaActual(void) {
    time_t ahora = time(NULL);
    return deTm(localtime(&ahora));
}

int diferenciaDias(Fecha desde, Fecha hasta) {
    double d = difftime(aTiempo(hasta), aTiempo(desde)) / 86400.0;
    return (int)(d + (d >= 0 ? 0.5 : -0.5));
}

Fecha sumarDias(Fecha f, int dias) {
    struct tm t = {0};
    t.tm_mday = f.dia + dias;    
    t.tm_mon = f.mes - 1;
    t.tm_year = f.year - 1900;
    t.tm_hour = 12;
    t.tm_isdst = -1;
    mktime(&t);
    return deTm(&t);
}

void fechaATexto(Fecha f, char *dest) {
    snprintf(dest, 11, "%02d/%02d/%04d", f.dia, f.mes, f.year);
}
