#include "estructuras.h"
#include "validaciones.h"

static void leerLinea(char *buf, int tam) {
    if (!fgets(buf, tam, stdin)) { puts("\nEntrada cerrada."); exit(1); }
    if (!strchr(buf, '\n')) { int c; while ((c = getchar()) != '\n' && c != EOF) {} }
    buf[strcspn(buf, "\n")] = 0;
}

int leerEntero(const char *msg, int min, int max) {
    char buf[64], *fin;
    long v;
    for (;;) {
        printf("%s", msg);
        leerLinea(buf, sizeof buf);
        v = strtol(buf, &fin, 10);
        while (isspace((unsigned char)*fin)) fin++;
        if (fin == buf || *fin) printf("  Numero invalido.\n");
        else if (v < min || v > max) printf("  Debe estar entre %d y %d.\n", min, max);
        else return (int)v;
    }
}

void leerCadena(const char *msg, char *dest, int tam) {
    char buf[256], *ini;
    int len;
    for (;;) {
        printf("%s", msg);
        leerLinea(buf, sizeof buf);
        for (ini = buf; isspace((unsigned char)*ini); ini++) {}
        len = (int)strlen(ini);
        while (len > 0 && isspace((unsigned char)ini[len - 1])) ini[--len] = 0;
        if (!len) printf("  No puede quedar vacio.\n");
        else if (len >= tam) printf("  Maximo %d caracteres.\n", tam - 1);
        else { memcpy(dest, ini, len + 1); return; }
    }
}
