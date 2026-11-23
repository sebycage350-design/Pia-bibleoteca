#include "estructuras.h"
#include "reportes.h"
#include "busqueda.h"
#include "fechas.h"

static const char *tituloDe(const Libro *l, int nl, int id) {
    int i = buscarLibroPorId(l, nl, id);
    return i == -1 ? "(libro no encontrado)" : l[i].titulo;
}

static const char *nombreDe(const Usuario *u, int nu, int matricula) {
    int i = buscarUsuarioPorMatricula(u, nu, matricula);
    return i == -1 ? "(usuario no encontrado)" : u[i].nombre;
}

static int esActivo(const Prestamo *p) { return strcmp(p->estado, ESTADO_ACTIVO) == 0; }

static void encabezado(FILE *f, const char *titulo) {
    char hoy[11];
    fechaATexto(fechaActual(), hoy);
    fprintf(f, "\n%s\nGenerado: %s\n\n", titulo, hoy);
}


static void escribirPrestados(FILE *f, const Prestamo *p, int np, const Libro *l, int nl,
                              const Usuario *u, int nu) {
    char pres[11], lim[11];
    int total = 0;

    encabezado(f, "REPORTE DE LIBROS PRESTADOS");
    fprintf(f, "%-9s %-28s %-10s %-22s %-11s %s\n", "Prestamo", "Libro", "Matricula", "Usuario", "Prestado", "Limite");
    for (const Prestamo *q = p; q < p + np; q++) {
        if (!esActivo(q)) continue;
        fechaATexto(q->fechaPrestamo, pres);
        fechaATexto(q->fechaLimite, lim);
        fprintf(f, "%-9d %-28.28s %-10d %-22.22s %-11s %s\n", q->id, tituloDe(l, nl, q->idLibro),
                q->matricula, nombreDe(u, nu, q->matricula), pres, lim);
        total++;
    }
    fprintf(f, "\nTotal de libros prestados: %d\n", total);
}

void reportePrestados(const Prestamo *p, int np, const Libro *l, int nl, const Usuario *u, int nu) {
    FILE *f = fopen(ARCHIVO_REP_PRESTADOS, "w");
    if (!f) { printf("No se pudo crear %s.\n", ARCHIVO_REP_PRESTADOS); return; }
    escribirPrestados(f, p, np, l, nl, u, nu);
    fclose(f);
    escribirPrestados(stdout, p, np, l, nl, u, nu);
    printf("\nReporte guardado en %s\n", ARCHIVO_REP_PRESTADOS);
}


static void escribirVencidos(FILE *f, const Prestamo *p, int np, const Libro *l, int nl,
                             const Usuario *u, int nu) {
    Fecha hoy = fechaActual();
    char lim[11];
    int total = 0, dias;

    encabezado(f, "REPORTE DE PRESTAMOS VENCIDOS");
    fprintf(f, "%-9s %-28s %-10s %-22s %-11s %-8s %s\n", "Prestamo", "Libro", "Matricula", "Usuario",
            "Limite", "Retraso", "Multa");
    for (const Prestamo *q = p; q < p + np; q++) {
        if (!esActivo(q)) continue;
        dias = diferenciaDias(q->fechaLimite, hoy);
        if (dias <= 0) continue;
        fechaATexto(q->fechaLimite, lim);
        fprintf(f, "%-9d %-28.28s %-10d %-22.22s %-11s %-3d dias  $%d\n", q->id, tituloDe(l, nl, q->idLibro),
                q->matricula, nombreDe(u, nu, q->matricula), lim, dias, dias * MULTA_POR_DIA);
        total++;
    }
    fprintf(f, "\nTotal de prestamos vencidos: %d\n", total);
    fprintf(f, "(La multa mostrada es la acumulada hasta hoy; se cobra al devolver el libro.)\n");
}

void reporteVencidos(const Prestamo *p, int np, const Libro *l, int nl, const Usuario *u, int nu) {
    FILE *f = fopen(ARCHIVO_REP_VENCIDOS, "w");
    if (!f) { printf("No se pudo crear %s.\n", ARCHIVO_REP_VENCIDOS); return; }
    escribirVencidos(f, p, np, l, nl, u, nu);
    fclose(f);
    escribirVencidos(stdout, p, np, l, nl, u, nu);
    printf("\nReporte guardado en %s\n", ARCHIVO_REP_VENCIDOS);
}


static void escribirMultas(FILE *f, const Usuario *u, int nu) {
    int total = 0, suma = 0;

    encabezado(f, "REPORTE DE USUARIOS CON MULTA");
    fprintf(f, "%-10s %-30s %-12s %s\n", "Matricula", "Nombre", "Telefono", "Multa");
    for (const Usuario *x = u; x < u + nu; x++) {
        if (!x->activo || x->multaPendiente <= 0) continue;
        fprintf(f, "%-10d %-30.30s %-12s $%d\n", x->matricula, x->nombre, x->telefono, x->multaPendiente);
        total++;
        suma += x->multaPendiente;
    }
    fprintf(f, "\nUsuarios con multa: %d\nTotal por cobrar: $%d\n", total, suma);
}

void reporteMultas(const Usuario *u, int nu) {
    FILE *f = fopen(ARCHIVO_REP_MULTAS, "w");
    if (!f) { printf("No se pudo crear %s.\n", ARCHIVO_REP_MULTAS); return; }
    escribirMultas(f, u, nu);
    fclose(f);
    escribirMultas(stdout, u, nu);
    printf("\nReporte guardado en %s\n", ARCHIVO_REP_MULTAS);
}


static void escribirPopulares(FILE *f, const Libro *l, const int *cuenta, const int *orden, int k) {
    int lugar = 0;

    encabezado(f, "REPORTE DE LIBROS MAS SOLICITADOS");
    fprintf(f, "%-6s %-5s %-30s %-20s %s\n", "Lugar", "ID", "Titulo", "Autor", "Veces prestado");
    for (const int *o = orden; o < orden + k && lugar < TOP_POPULARES; o++) {
        fprintf(f, "%-6d %-5d %-30.30s %-20.20s %d\n", ++lugar, l[*o].id, l[*o].titulo, l[*o].autor, cuenta[*o]);
    }
    if (!k) fprintf(f, "Todavia no hay prestamos registrados.\n");
}

void reporteMasSolicitados(const Prestamo *p, int np, const Libro *l, int nl) {
    int *cuenta, *orden, *i, *j, *mayor, tmp, k = 0, idx;
    FILE *f;

    if (nl == 0) { puts("No hay libros registrados."); return; }
    cuenta = calloc(nl, sizeof *cuenta);
    orden = malloc(nl * sizeof *orden);
    if (!cuenta || !orden) { puts("No hay memoria."); free(cuenta); free(orden); return; }

    for (const Prestamo *q = p; q < p + np; q++) {      /* cuantas veces se presto cada libro */
        idx = buscarLibroPorId(l, nl, q->idLibro);
        if (idx != -1) cuenta[idx]++;
    }
    for (idx = 0; idx < nl; idx++) if (cuenta[idx] > 0) orden[k++] = idx;

    for (i = orden; i < orden + k - 1; i++) {            /* orden de mayor a menor */
        mayor = i;
        for (j = i + 1; j < orden + k; j++) if (cuenta[*j] > cuenta[*mayor]) mayor = j;
        if (mayor != i) { tmp = *i; *i = *mayor; *mayor = tmp; }
    }

    f = fopen(ARCHIVO_REP_POPULARES, "w");
    if (!f) {
        printf("No se pudo crear %s.\n", ARCHIVO_REP_POPULARES);
    } else {
        escribirPopulares(f, l, cuenta, orden, k);
        fclose(f);
    }
    escribirPopulares(stdout, l, cuenta, orden, k);
    if (f) printf("\nReporte guardado en %s\n", ARCHIVO_REP_POPULARES);
    free(cuenta);
    free(orden);
}


void generarComprobante(const Prestamo *pr, const Libro *l, int nl, const Usuario *u, int nu) {
    char nombre[48], pres[11], lim[11];
    FILE *f;

    snprintf(nombre, sizeof nombre, "comprobante_prestamo_%d.txt", pr->id);
    f = fopen(nombre, "w");
    if (!f) { printf("No se pudo crear %s.\n", nombre); return; }

    fechaATexto(pr->fechaPrestamo, pres);
    fechaATexto(pr->fechaLimite, lim);
    fprintf(f, "========================================\n");
    fprintf(f, "       COMPROBANTE DE PRESTAMO\n");
    fprintf(f, "========================================\n");
    fprintf(f, "Prestamo No.:   %d\n", pr->id);
    fprintf(f, "Usuario:        %s\n", nombreDe(u, nu, pr->matricula));
    fprintf(f, "Matricula:      %d\n", pr->matricula);
    fprintf(f, "Libro:          %s\n", tituloDe(l, nl, pr->idLibro));
    fprintf(f, "ID del libro:   %d\n", pr->idLibro);
    fprintf(f, "Fecha prestamo: %s\n", pres);
    fprintf(f, "Devolver antes: %s\n", lim);
    fprintf(f, "----------------------------------------\n");
    fprintf(f, "Multa por retraso: $%d por dia.\n", MULTA_POR_DIA);
    fprintf(f, "========================================\n");
    fclose(f);
    printf("Comprobante guardado en %s\n", nombre);
}
