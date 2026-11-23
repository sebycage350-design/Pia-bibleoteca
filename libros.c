#include "estructuras.h"
#include "libros.h"
#include "busqueda.h"
#include "archivos.h"
#include "validaciones.h"

void encabezadoLibros(void) {
    printf("\n%-5s %-30s %-20s %-12s %s\n", "ID", "Titulo", "Autor", "Categoria", "Disp/Total");
}

void imprimirLibro(const Libro *l) {
    printf("%-5d %-30.30s %-20.20s %-12.12s %d/%d\n", l->id, l->titulo, l->autor,
           l->categoria, l->ejemplaresDisponibles, l->ejemplaresTotales);
}

static int pedirLibroActivo(const Libro *l, int n, const char *msg) {
    int i = buscarLibroPorId(l, n, leerEntero(msg, 1, ID_LIBRO_MAX));
    if (i == -1 || !l[i].activo) { puts("No existe un libro activo con ese ID."); return -1; }
    return i;
}

int agregarLibro(Libro **l, int *n, int *cap) {
    Libro nuevo = {0}, *tmp;

    /* Si la memoria falla no pediremos datos */
    if (!(tmp = crecerArreglo(*l, *n, cap, sizeof(Libro)))) return 0;
    *l = tmp;

    nuevo.id = 1;
    for (int i = 0; i < *n; i++) if ((*l)[i].id >= nuevo.id) nuevo.id = (*l)[i].id + 1;

    leerCadena("Titulo: ", nuevo.titulo, sizeof nuevo.titulo);
    leerCadena("Autor: ", nuevo.autor, sizeof nuevo.autor);
    leerCadena("Categoria: ", nuevo.categoria, sizeof nuevo.categoria);
    nuevo.ejemplaresTotales = leerEntero("Ejemplares: ", 1, EJEMPLARES_MAX);
    nuevo.ejemplaresDisponibles = nuevo.ejemplaresTotales;
    nuevo.activo = 1;

    (*l)[(*n)++] = nuevo;
    printf("Libro agregado con ID %d.\n", nuevo.id);
    return 1;
}

int modificarLibro(Libro *l, int n) {
    int i = pedirLibroActivo(l, n, "ID del libro a modificar: "), prestados;
    Libro *b;

    if (i == -1) return 0;
    b = &l[i];
    encabezadoLibros();
    imprimirLibro(b);

    switch (leerEntero("Cambiar: 1) Titulo  2) Autor  3) Categoria  4) Ejemplares  0) Cancelar: ", 0, 4)) {
        case 1: leerCadena("Nuevo titulo: ", b->titulo, sizeof b->titulo); break;
        case 2: leerCadena("Nuevo autor: ", b->autor, sizeof b->autor); break;
        case 3: leerCadena("Nueva categoria: ", b->categoria, sizeof b->categoria); break;
        case 4:
            prestados = b->ejemplaresTotales - b->ejemplaresDisponibles;
            b->ejemplaresTotales = leerEntero("Nuevo total de ejemplares: ",
                                              prestados ? prestados : 1, EJEMPLARES_MAX);
            b->ejemplaresDisponibles = b->ejemplaresTotales - prestados;
            break;
        default: return 0;
    }
    puts("Libro actualizado.");
    return 1;
}

int eliminarLibro(Libro *l, int n) {
    int i = pedirLibroActivo(l, n, "ID del libro a dar de baja: ");

    if (i == -1) return 0;
    if (l[i].ejemplaresDisponibles < l[i].ejemplaresTotales) {
        puts("No se puede: hay ejemplares prestados.");
        return 0;
    }
    if (leerEntero("Seguro? 1) Si  0) No: ", 0, 1)) {
        l[i].activo = 0;
        puts("Libro dado de baja.");
        return 1;
    }
    return 0;
}

void mostrarLibros(const Libro *l, int n) {
    int mostrados = 0;
    for (int i = 0; i < n; i++) {
        if (!l[i].activo) continue;
        if (!mostrados++) encabezadoLibros();
        imprimirLibro(&l[i]);
    }
    if (!mostrados) puts("No hay libros registrados.");
}
