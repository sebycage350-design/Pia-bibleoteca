#include "estructuras.h"
#include "usuarios.h"
#include "busqueda.h"
#include "archivos.h"
#include "fechas.h"
#include "validaciones.h"

static void leerTelefono(char *dest, int tam) {
    for (;;) {
        leerCadena("Telefono (10 digitos): ", dest, tam);
        if (strlen(dest) == 10 && strspn(dest, "0123456789") == 10) return;
        puts("  Deben ser 10 digitos, sin espacios ni guiones.");
    }
}

static int pedirUsuarioActivo(const Usuario *u, int n, const char *msg) {
    int i = buscarUsuarioPorMatricula(u, n, leerEntero(msg, 1, MAT_MAX));
    if (i == -1 || !u[i].activo) { puts("No existe un usuario activo con esa matricula."); return -1; }
    return i;
}

static int mismoNombre(const char *a, const char *b) {
    while (*a && tolower((unsigned char)*a) == tolower((unsigned char)*b)) a++, b++;
    return *a == *b;
}

/* La matricula ya existe en el archivo. Solo se puede reactivar al mismo
   usuario y dentro de DIAS_REACTIVACION_USUARIO dias despues de su baja. */
static int reactivarUsuario(Usuario *x) {
    char nombre[80];

    if (x->activo) { puts("Esa matricula ya esta registrada."); return 0; }

    if (diferenciaDias(x->fechaBaja, fechaActual()) > DIAS_REACTIVACION_USUARIO) {
        printf("Esa matricula fue dada de baja hace mas de %d dias y no se puede volver a usar.\n",
               DIAS_REACTIVACION_USUARIO);
        return 0;
    }

    puts("Esa matricula pertenece a un usuario dado de baja que aun puede reactivarse.");
    leerCadena("Nombre completo del usuario: ", nombre, sizeof nombre);
    if (!mismoNombre(nombre, x->nombre)) {
        puts("El nombre no coincide. Esa matricula no se puede asignar a otra persona.");
        return 0;
    }

    leerTelefono(x->telefono, sizeof x->telefono);
    x->activo = 1;
    x->fechaBaja = (Fecha){0, 0, 0};
    puts("Usuario reactivado.");
    return 1;
}

void encabezadoUsuarios(void) {
    printf("\n%-10s %-30s %-12s %-7s %s\n", "Matricula", "Nombre", "Telefono", "Libros", "Multa");
}

void imprimirUsuario(const Usuario *u) {
    printf("%-10d %-30.30s %-12s %-7d $%d\n", u->matricula, u->nombre, u->telefono,
           u->librosPrestados, u->multaPendiente);
}

int agregarUsuario(Usuario **u, int *n, int *cap) {
    Usuario nuevo = {0}, *tmp;
    int i;

    
    if (!(tmp = crecerArreglo(*u, *n, cap, sizeof(Usuario)))) return 0;
    *u = tmp;

    nuevo.matricula = leerEntero("Matricula: ", 1, MAT_MAX);
    if ((i = buscarUsuarioPorMatricula(*u, *n, nuevo.matricula)) != -1)
        return reactivarUsuario(&(*u)[i]);

    leerCadena("Nombre: ", nuevo.nombre, sizeof nuevo.nombre);
    leerTelefono(nuevo.telefono, sizeof nuevo.telefono);
    nuevo.activo = 1;

    (*u)[(*n)++] = nuevo;
    puts("Usuario agregado.");
    return 1;
}

int modificarUsuario(Usuario *u, int n) {
    int i = pedirUsuarioActivo(u, n, "Matricula del usuario a modificar: ");
    if (i == -1) return 0;
    encabezadoUsuarios();
    imprimirUsuario(&u[i]);
    switch (leerEntero("Cambiar: 1) Nombre  2) Telefono  0) Cancelar: ", 0, 2)) {
        case 1: leerCadena("Nuevo nombre: ", u[i].nombre, sizeof u[i].nombre); break;
        case 2: leerTelefono(u[i].telefono, sizeof u[i].telefono); break;
        default: return 0;
    }
    puts("Usuario actualizado.");
    return 1;
}

int eliminarUsuario(Usuario *u, int n) {
    int i = pedirUsuarioActivo(u, n, "Matricula del usuario a dar de baja: ");
    if (i == -1) return 0;
    if (u[i].librosPrestados > 0) { puts("No se puede: todavia tiene libros prestados."); return 0; }
    if (u[i].multaPendiente > 0)  { puts("No se puede: tiene una multa pendiente."); return 0; }
    if (leerEntero("Seguro? 1) Si  0) No: ", 0, 1)) {
        u[i].activo = 0;
        u[i].fechaBaja = fechaActual();
        printf("Usuario dado de baja. Puede reactivarse durante %d dias.\n", DIAS_REACTIVACION_USUARIO);
        return 1;
    }
    return 0;
}

void mostrarUsuarios(const Usuario *u, int n) {
    int mostrados = 0;
    for (int i = 0; i < n; i++) {
        if (!u[i].activo) continue;
        if (!mostrados++) encabezadoUsuarios();
        imprimirUsuario(&u[i]);
    }
    if (!mostrados) puts("No hay usuarios registrados.");
}
