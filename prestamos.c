#include "estructuras.h"
#include "prestamos.h"
#include "busqueda.h"
#include "archivos.h"
#include "fechas.h"
#include "validaciones.h"

int calcularMulta(Fecha limite, Fecha devolucion) {
    int dias = diferenciaDias(limite, devolucion);
    return dias > 0 ? dias * MULTA_POR_DIA : 0;
}

static int pedirUsuarioActivo(const Usuario *u, int nu) {
    int i = buscarUsuarioPorMatricula(u, nu, leerEntero("Matricula del usuario: ", 1, MAT_MAX));
    if (i == -1 || !u[i].activo) { puts("No existe un usuario activo con esa matricula."); return -1; }
    return i;
}

int prestarLibro(Libro *l, int nl, Usuario *u, int nu, Prestamo **p, int *np, int *cap) {
    Prestamo nuevo = {0}, *tmp;
    Usuario *us;
    Libro *li;
    Fecha hoy = fechaActual();
    char limite[11];
    int i;

    /* no se modifica nada si no hay donde guardar el prestamo. */
    if (!(tmp = crecerArreglo(*p, *np, cap, sizeof(Prestamo)))) return 0;
    *p = tmp;

    if ((i = pedirUsuarioActivo(u, nu)) == -1) return 0;
    us = &u[i];
    if (us->multaPendiente > 0) {
        printf("No se puede: el usuario tiene una multa pendiente de $%d.\n", us->multaPendiente);
        return 0;
    }
    if (us->librosPrestados >= MAX_LIBROS_USUARIO) {
        printf("No se puede: el usuario ya tiene %d libros prestados (maximo).\n", MAX_LIBROS_USUARIO);
        return 0;
    }

    i = buscarLibroPorId(l, nl, leerEntero("ID del libro: ", 1, ID_LIBRO_MAX));
    if (i == -1 || !l[i].activo) { puts("No existe un libro activo con ese ID."); return 0; }
    li = &l[i];
    if (li->ejemplaresDisponibles < 1) { puts("No se puede: no hay ejemplares disponibles."); return 0; }

    nuevo.id = 1;
    for (i = 0; i < *np; i++) if ((*p)[i].id >= nuevo.id) nuevo.id = (*p)[i].id + 1;
    nuevo.idLibro = li->id;
    nuevo.matricula = us->matricula;
    nuevo.fechaPrestamo = hoy;
    nuevo.fechaLimite = sumarDias(hoy, DIAS_PRESTAMO);
    strcpy(nuevo.estado, ESTADO_ACTIVO);

    li->ejemplaresDisponibles--;
    us->librosPrestados++;
    (*p)[(*np)++] = nuevo;

    fechaATexto(nuevo.fechaLimite, limite);
    printf("Prestamo %d registrado. Fecha limite de devolucion: %s\n", nuevo.id, limite);
    return 1;
}

int devolverLibro(Libro *l, int nl, Usuario *u, int nu, Prestamo *p, int np) {
    Prestamo *pr = NULL;
    Fecha hoy = fechaActual();
    char limite[11];
    int iu, il, id, hallados = 0;

    if ((iu = pedirUsuarioActivo(u, nu)) == -1) return 0;

    for (int k = 0; k < np; k++) {
        if (p[k].matricula != u[iu].matricula || strcmp(p[k].estado, ESTADO_ACTIVO) != 0) continue;
        if (!hallados++) printf("\n%-9s %-30s %s\n", "Prestamo", "Libro", "Fecha limite");
        il = buscarLibroPorId(l, nl, p[k].idLibro);
        fechaATexto(p[k].fechaLimite, limite);
        printf("%-9d %-30.30s %s\n", p[k].id, il == -1 ? "(libro no encontrado)" : l[il].titulo, limite);
    }
    if (!hallados) { puts("Ese usuario no tiene libros prestados."); return 0; }

    id = leerEntero("ID del prestamo a devolver (0 para cancelar): ", 0, ID_PRESTAMO_MAX);
    if (id == 0) return 0;
    for (int k = 0; k < np; k++) {
        if (p[k].id == id && p[k].matricula == u[iu].matricula && strcmp(p[k].estado, ESTADO_ACTIVO) == 0) {
            pr = &p[k];
            break;
        }
    }
    if (!pr) { puts("Ese ID no es un prestamo activo de este usuario."); return 0; }

    il = buscarLibroPorId(l, nl, pr->idLibro);
    if (il == -1) { puts("Error: el libro de este prestamo no existe en el archivo."); return 0; }

    pr->fechaDevolucion = hoy;
    pr->multa = calcularMulta(pr->fechaLimite, hoy);
    strcpy(pr->estado, ESTADO_DEVUELTO);
    l[il].ejemplaresDisponibles++;
    u[iu].librosPrestados--;
    u[iu].multaPendiente += pr->multa;

    if (pr->multa > 0)
        printf("Libro devuelto con %d dias de retraso. Multa: $%d.\n", pr->multa / MULTA_POR_DIA, pr->multa);
    else
        puts("Libro devuelto a tiempo. Sin multa.");
    return 1;
}

int pagarMulta(Usuario *u, int nu) {
    int i = pedirUsuarioActivo(u, nu);

    if (i == -1) return 0;
    if (u[i].multaPendiente == 0) { puts("Ese usuario no tiene multas pendientes."); return 0; }

    printf("%s debe $%d.\n", u[i].nombre, u[i].multaPendiente);
    if (leerEntero("Registrar el pago completo? 1) Si  0) No: ", 0, 1)) {
        u[i].multaPendiente = 0;
        puts("Pago registrado.");
        return 1;
    }
    return 0;
}
