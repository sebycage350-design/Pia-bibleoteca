#include "estructuras.h"
#include "busqueda.h"
#include "usuarios.h"
#include "libros.h"
#include "validaciones.h"

static int igualN(const char *a, const char *b, size_t n) {
    while (n--) if (tolower((unsigned char)*a++) != tolower((unsigned char)*b++)) return 0;
    return 1;
}

static int contiene(const char *t, const char *b) {
    size_t n = strlen(b);
    for (; *t; t++) if (igualN(t, b, n)) return 1;
    return 0;
}

static int comparar(const char *a, const char *b) {
    while (*a && tolower((unsigned char)*a) == tolower((unsigned char)*b)) a++, b++;
    return tolower((unsigned char)*a) - tolower((unsigned char)*b);
}


static const char *campo(const Libro *l, int criterio) {
    switch (criterio) {
        case ORDEN_AUTOR:     return l->autor;
        case ORDEN_CATEGORIA: return l->categoria;
        default:              return l->titulo;
    }
}

int buscarLibroPorId(const Libro *l, int n, int id) {
    for (int i = 0; i < n; i++) if (l[i].id == id) return i;
    return -1;
}

int buscarUsuarioPorMatricula(const Usuario *u, int n, int matricula) {
    for (int i = 0; i < n; i++) if (u[i].matricula == matricula) return i;
    return -1;
}

static void buscarLibro(const Libro *l, int n, int criterio) {
    char txt[100];
    int hallados = 0;
    const char *msg = criterio == ORDEN_AUTOR ? "Autor a buscar: "
                    : criterio == ORDEN_CATEGORIA ? "Categoria a buscar: "
                    : "Titulo a buscar: ";

    leerCadena(msg, txt, sizeof txt);
    for (int i = 0; i < n; i++) {
        if (l[i].activo && contiene(campo(&l[i], criterio), txt)) {
            if (!hallados++) encabezadoLibros();
            imprimirLibro(&l[i]);
        }
    }
    if (!hallados) puts("No se encontraron libros.");
}

void buscarLibroPorTitulo(const Libro *l, int n)    { buscarLibro(l, n, ORDEN_TITULO); }
void buscarLibroPorAutor(const Libro *l, int n)     { buscarLibro(l, n, ORDEN_AUTOR); }
void buscarLibroPorCategoria(const Libro *l, int n) { buscarLibro(l, n, ORDEN_CATEGORIA); }

void buscarUsuario(const Usuario *u, int n) {
    int op = leerEntero("Buscar por: 1) Matricula  2) Nombre  0) Cancelar: ", 0, 2), hallados = 0;
    if (op == 1) {
        int i = buscarUsuarioPorMatricula(u, n, leerEntero("Matricula: ", 1, MAT_MAX));
        if (i != -1 && u[i].activo) { encabezadoUsuarios(); imprimirUsuario(&u[i]); }
        else puts("No existe un usuario activo con esa matricula.");
    } else if (op == 2) {
        char txt[80];
        leerCadena("Nombre a buscar: ", txt, sizeof txt);
        for (int i = 0; i < n; i++) {
            if (u[i].activo && contiene(u[i].nombre, txt)) {
                if (!hallados++) encabezadoUsuarios();
                imprimirUsuario(&u[i]);
            }
        }
        if (!hallados) puts("No se encontro ningun usuario con ese nombre.");
    }
}

void mostrarLibrosOrdenados(const Libro *l, int n, int criterio) {
    const Libro **vista, **fin, **i, **j, **menor, *tmp;
    int activos = 0, k = 0;

    for (int x = 0; x < n; x++) if (l[x].activo) activos++;
    if (!activos) { puts("No hay libros registrados."); return; }

    vista = malloc(activos * sizeof *vista);
    if (!vista) { puts("No hay memoria."); return; }
    for (int x = 0; x < n; x++) if (l[x].activo) vista[k++] = &l[x];

    fin = vista + activos;
    for (i = vista; i < fin - 1; i++) {
        menor = i;
        for (j = i + 1; j < fin; j++)
            if (comparar(campo(*j, criterio), campo(*menor, criterio)) < 0) menor = j;
        if (menor != i) { tmp = *i; *i = *menor; *menor = tmp; }
    }

    encabezadoLibros();
    for (i = vista; i < fin; i++) imprimirLibro(*i);
    free(vista);
}
