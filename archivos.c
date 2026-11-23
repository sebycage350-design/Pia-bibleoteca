#include "estructuras.h"
#include "archivos.h"

static void *cargar(const char *archivo, size_t tam, int *n, int *cap) {
    FILE *f = fopen(archivo, "rb");
    void *datos = NULL;
    long bytes;

    *n = *cap = 0;
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    bytes = ftell(f);
    rewind(f);

    if (bytes >= (long)tam) {
        *cap = (int)(bytes / (long)tam);
        datos = malloc(*cap * tam);
        if (datos) *n = (int)fread(datos, tam, *cap, f);
        else { *cap = 0; printf("No hay memoria para leer %s.\n", archivo); }
    }
    fclose(f);
    return datos;
}

static int guardar(const char *archivo, const void *datos, size_t tam, int n) {
    FILE *f = fopen(archivo, "wb");
    int ok;

    if (!f) { printf("No se pudo abrir %s para guardar.\n", archivo); return 0; }
    ok = (n == 0) || fwrite(datos, tam, n, f) == (size_t)n;
    fclose(f);
    if (!ok) printf("Error al escribir %s.\n", archivo);
    return ok;
}

Libro *cargarLibros(int *n, int *cap)       { return cargar(ARCHIVO_LIBROS, sizeof(Libro), n, cap); }
Usuario *cargarUsuarios(int *n, int *cap)   { return cargar(ARCHIVO_USUARIOS, sizeof(Usuario), n, cap); }
Prestamo *cargarPrestamos(int *n, int *cap) { return cargar(ARCHIVO_PRESTAMOS, sizeof(Prestamo), n, cap); }

int guardarLibros(const Libro *l, int n)       { return guardar(ARCHIVO_LIBROS, l, sizeof(Libro), n); }
int guardarUsuarios(const Usuario *u, int n)   { return guardar(ARCHIVO_USUARIOS, u, sizeof(Usuario), n); }
int guardarPrestamos(const Prestamo *p, int n) { return guardar(ARCHIVO_PRESTAMOS, p, sizeof(Prestamo), n); }

void *crecerArreglo(void *arr, int n, int *cap, size_t tam) {
    int nuevaCap;
    void *tmp;

    if (n < *cap) return arr;
    nuevaCap = *cap ? *cap * 2 : 4;
    tmp = realloc(arr, nuevaCap * tam);
    if (!tmp) { puts("No hay memoria."); return NULL; }
    *cap = nuevaCap;
    return tmp;
}
