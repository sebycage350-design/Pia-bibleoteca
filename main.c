#include "estructuras.h"
#include "archivos.h"
#include "libros.h"
#include "usuarios.h"
#include "busqueda.h"
#include "prestamos.h"
#include "reportes.h"
#include "validaciones.h"


typedef struct {
    Libro *libros;       int nLibros, capLibros;
    Usuario *usuarios;   int nUsuarios, capUsuarios;
    Prestamo *prestamos; int nPrestamos, capPrestamos;
} Datos;


static int guardarDatos(const Datos *d, int libros, int usuarios, int prestamos) {
    int ok = 1;
    if (libros)    ok &= guardarLibros(d->libros, d->nLibros);
    if (usuarios)  ok &= guardarUsuarios(d->usuarios, d->nUsuarios);
    if (prestamos) ok &= guardarPrestamos(d->prestamos, d->nPrestamos);
    if (!ok) puts("ATENCION: no se pudieron guardar los cambios. No cierres el programa e intenta de nuevo.");
    return ok;
}

static void menuLibros(Datos *d) {
    int op;
    do {
        puts("\n--- LIBROS ---");
        puts("1) Agregar  2) Modificar  3) Dar de baja  4) Mostrar  0) Volver");
        op = leerEntero("Opcion: ", 0, 4);
        switch (op) {
            case 1: if (agregarLibro(&d->libros, &d->nLibros, &d->capLibros)) guardarDatos(d, 1, 0, 0); break;
            case 2: if (modificarLibro(d->libros, d->nLibros)) guardarDatos(d, 1, 0, 0); break;
            case 3: if (eliminarLibro(d->libros, d->nLibros)) guardarDatos(d, 1, 0, 0); break;
            case 4: mostrarLibros(d->libros, d->nLibros); break;
        }
    } while (op != 0);
}

static void menuUsuarios(Datos *d) {
    int op;
    do {
        puts("\n--- USUARIOS ---");
        puts("1) Agregar  2) Modificar  3) Dar de baja  4) Mostrar  0) Volver");
        op = leerEntero("Opcion: ", 0, 4);
        switch (op) {
            case 1: if (agregarUsuario(&d->usuarios, &d->nUsuarios, &d->capUsuarios)) guardarDatos(d, 0, 1, 0); break;
            case 2: if (modificarUsuario(d->usuarios, d->nUsuarios)) guardarDatos(d, 0, 1, 0); break;
            case 3: if (eliminarUsuario(d->usuarios, d->nUsuarios)) guardarDatos(d, 0, 1, 0); break;
            case 4: mostrarUsuarios(d->usuarios, d->nUsuarios); break;
        }
    } while (op != 0);
}

static void menuPrestamos(Datos *d) {
    int op;
    do {
        puts("\n--- PRESTAMOS Y DEVOLUCIONES ---");
        puts("1) Prestar libro  2) Devolver libro  3) Pagar multa  0) Volver");
        op = leerEntero("Opcion: ", 0, 3);
        switch (op) {
            case 1:
                if (prestarLibro(d->libros, d->nLibros, d->usuarios, d->nUsuarios,
                                 &d->prestamos, &d->nPrestamos, &d->capPrestamos)) {
                    guardarDatos(d, 1, 1, 1);
                    generarComprobante(&d->prestamos[d->nPrestamos - 1], d->libros, d->nLibros,
                                       d->usuarios, d->nUsuarios);
                }
                break;
            case 2:
                if (devolverLibro(d->libros, d->nLibros, d->usuarios, d->nUsuarios,
                                  d->prestamos, d->nPrestamos))
                    guardarDatos(d, 1, 1, 1);
                break;
            case 3:
                if (pagarMulta(d->usuarios, d->nUsuarios)) guardarDatos(d, 0, 1, 0);
                break;
        }
    } while (op != 0);
}

static void menuBusqueda(const Datos *d) {
    int op;
    do {
        puts("\n--- BUSQUEDA Y ORDENAMIENTO ---");
        puts("1) Libro por titulo  2) Libro por autor  3) Libro por categoria");
        puts("4) Usuario  5) Listar libros ordenados  0) Volver");
        op = leerEntero("Opcion: ", 0, 5);
        switch (op) {
            case 1: buscarLibroPorTitulo(d->libros, d->nLibros); break;
            case 2: buscarLibroPorAutor(d->libros, d->nLibros); break;
            case 3: buscarLibroPorCategoria(d->libros, d->nLibros); break;
            case 4: buscarUsuario(d->usuarios, d->nUsuarios); break;
            case 5:
                mostrarLibrosOrdenados(d->libros, d->nLibros,
                    leerEntero("Ordenar por: 1) Titulo  2) Autor  3) Categoria: ", 1, 3));
                break;
        }
    } while (op != 0);
}

static void menuReportes(const Datos *d) {
    int op;
    do {
        puts("\n--- REPORTES ---");
        puts("1) Libros prestados  2) Prestamos vencidos  3) Usuarios con multa");
        puts("4) Libros mas solicitados  0) Volver");
        op = leerEntero("Opcion: ", 0, 4);
        switch (op) {
            case 1: reportePrestados(d->prestamos, d->nPrestamos, d->libros, d->nLibros, d->usuarios, d->nUsuarios); break;
            case 2: reporteVencidos(d->prestamos, d->nPrestamos, d->libros, d->nLibros, d->usuarios, d->nUsuarios); break;
            case 3: reporteMultas(d->usuarios, d->nUsuarios); break;
            case 4: reporteMasSolicitados(d->prestamos, d->nPrestamos, d->libros, d->nLibros); break;
        }
    } while (op != 0);
}

static void menuPrincipal(Datos *d) {
    int op;
    do {
        puts("\n===== BIBLIOTECA UANL =====");
        puts("1) Libros");
        puts("2) Usuarios");
        puts("3) Prestamos y devoluciones");
        puts("4) Busqueda y ordenamiento");
        puts("5) Reportes");
        puts("0) Salir");
        op = leerEntero("Opcion: ", 0, 5);
        switch (op) {
            case 1: menuLibros(d); break;
            case 2: menuUsuarios(d); break;
            case 3: menuPrestamos(d); break;
            case 4: menuBusqueda(d); break;
            case 5: menuReportes(d); break;
        }
    } while (op != 0);
}

int main(void) {
    Datos d;

    d.libros = cargarLibros(&d.nLibros, &d.capLibros);
    d.usuarios = cargarUsuarios(&d.nUsuarios, &d.capUsuarios);
    d.prestamos = cargarPrestamos(&d.nPrestamos, &d.capPrestamos);

    menuPrincipal(&d);

    free(d.libros);
    free(d.usuarios);
    free(d.prestamos);
    puts("Hasta luego.");
    return 0;
}
