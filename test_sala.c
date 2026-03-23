// test_sala.c
// ===============
// Batería de pruebas final para la biblioteca "sala.h/sala.c"
//

#include <assert.h>
#include <stdio.h>
#include "sala.h"

#define DebeSerCierto(x)	assert(x)
#define DebeSerFalso(x)		assert(!(x))

// Prototipos de las funciones de test para que el compilador las conozca
void test_ReservaBasica();
void test_TipsProfesor();
void test_Exhaustivo_Memoria();
void test_Exhaustivo_Limites();
void test_Fragmentacion();
void test_Stress();
void test_Atomicidad_Multiple();
void estado_sala();

void ejecuta_tests() {
    test_ReservaBasica();
    test_TipsProfesor();
    test_Exhaustivo_Memoria();
    test_Exhaustivo_Limites();
    test_Fragmentacion();
    test_Stress();
    test_Atomicidad_Multiple();
}

int main() {
    puts("Iniciando batería de tests completa...");
    ejecuta_tests();
    puts("\n>>> TODOS LOS TESTS COMPLETADOS CON ÉXITO <<<");
    return 0;
}

void INICIO_TEST (const char* titulo_test) {
    printf("********** batería de pruebas para %s: ", titulo_test);
    fflush(stdout);
}

void FIN_TEST (const char* titulo_test) {
    printf ("********** hecho\n");
}

// Visualización del estado de la sala
void estado_sala() {
    int total = capacidad_sala();
    if (total == -1) return;
    printf("\n--- MAPA VISUAL DE SALA ---\n");
    for (int i = 1; i <= total; i++) {
        int ocupante = estado_asiento(i);
        printf("[%s] ", (ocupante == 0) ? " " : "X"); // 0 indica libre según API
        if (i % 10 == 0) printf("\n");
    }
    printf("Resumen: %d ocupados de %d totales\n", asientos_ocupados(), total);
}

void test_ReservaBasica() {
    int mi_asiento;
    #define CAPACIDAD_BASE 500
    #define ID_PERSONA 1500

    INICIO_TEST("Reserva básica");
    crea_sala(CAPACIDAD_BASE);
    DebeSerCierto(capacidad_sala() == CAPACIDAD_BASE);
    DebeSerCierto((mi_asiento = reserva_asiento(ID_PERSONA)) >= 0);
    DebeSerCierto((asientos_libres() + asientos_ocupados()) == CAPACIDAD_BASE);
    DebeSerCierto(estado_asiento(mi_asiento) > 0);
    DebeSerCierto(libera_asiento(mi_asiento) == ID_PERSONA);
    elimina_sala();
    FIN_TEST("Reserva básica");
}

void test_TipsProfesor() {
    INICIO_TEST("Robustez (Correcciones Profesor)");

    // Capacidad negativa
    DebeSerCierto(crea_sala(-10) == -1);

    // Control de doble creación
    crea_sala(10);
    DebeSerCierto(crea_sala(5) == -1);

    // IDs inválidos y asientos inexistentes
    DebeSerCierto(reserva_asiento(-5) == -1);
    DebeSerCierto(libera_asiento(999) == -1);
    DebeSerCierto(estado_asiento(0) == -1);

    elimina_sala();
    FIN_TEST("Robustez");
}

void test_ReservaMultipleTocha() {
    INICIO_TEST("Reserva Múltiple (Todo o Nada)");
    crea_sala(10);

    int grupo_ok[] = {10, 20, 30, 40, 50};
    DebeSerCierto(reserva_multiple(5, grupo_ok) == 5);
    DebeSerCierto(asientos_ocupados() == 5);

    // Intento de reserva que excede la capacidad (No debe sentar a nadie)
    int grupo_error[] = {60, 70, 80, 90, 100, 110};
    DebeSerCierto(reserva_multiple(6, grupo_error) == -1);
    DebeSerCierto(asientos_ocupados() == 5);

    estado_sala();
    elimina_sala();
    FIN_TEST("Reserva Múltiple");
}

void test_Exhaustivo_Memoria() {
    INICIO_TEST("Ciclo de vida de Memoria");

    // Crear y destruir sala repetidamente para ver si hay fugas o punteros colgados
    for(int i=0; i<100; i++) {
        crea_sala(i+1);
        DebeSerCierto(capacidad_sala() == i+1);
        elimina_sala();
    }

    // Verificar que tras eliminar, las funciones detectan que no hay sala
    DebeSerCierto(asientos_libres() == -1);

    FIN_TEST("Ciclo de vida de Memoria");
}

void test_Exhaustivo_Limites() {
    INICIO_TEST("Límites y Casos Absurdos");

    // --- TEST CON SALA NO CREADA ---
    // Casi todas las funciones deberían devolver -1 si 'asientos' es NULL
    DebeSerCierto(capacidad_sala() == -1);
    DebeSerCierto(asientos_libres() == -1);
    DebeSerCierto(asientos_ocupados() == -1);
    DebeSerCierto(reserva_asiento(100) == -1);
    DebeSerCierto(libera_asiento(1) == -1);
    DebeSerCierto(estado_asiento(1) == -1);
    DebeSerCierto(elimina_sala() == -1); // No se puede borrar lo que no existe

    // --- TEST DE INDICES ---
    crea_sala(10);
    DebeSerCierto(estado_asiento(0) == -1);   // El primer asiento es el 1, no el 0
    DebeSerCierto(estado_asiento(11) == -1);  // Fuera de rango superior
    DebeSerCierto(libera_asiento(0) == -1);   // Índice inválido

    // --- TEST DE RE-LIBERACIÓN ---
    reserva_asiento(50); // Se reserva el 1
    DebeSerCierto(libera_asiento(1) == 50);   // Primera liberación: OK
    DebeSerCierto(libera_asiento(1) == -1);   // Segunda liberación: ERROR (ya está libre)

    // --- TEST DE LLENADO TOTAL ---
    for(int i=0; i<10; i++) reserva_asiento(i+1); // Llenamos los 9 que quedan
    printf("Libres: %d\n", asientos_libres());
    DebeSerCierto(asientos_libres() == 0);
    DebeSerCierto(reserva_asiento(999) == -1); // Sala llena, debe fallar

    elimina_sala();
    FIN_TEST("Límites y Casos Absurdos");
}

void test_Exhaustivo_ReservaMultiple() {
    INICIO_TEST("Reserva Múltiple (Transaccionalidad)");

    crea_sala(10);
    int personas[] = {10, 20, 30};

    // Reserva normal
    DebeSerCierto(reserva_multiple(3, personas) == 3);
    DebeSerCierto(asientos_ocupados() == 3);

    // Intentar reservar más de lo que queda (Quedan 7 libres, pedimos 8)
    // Según tu código actual, esto debería devolver -1 y NO sentar a nadie
    int grupo_grande[] = {1, 2, 3, 4, 5, 6, 7, 8};
    DebeSerCierto(reserva_multiple(8, grupo_grande) == -1);
    DebeSerCierto(asientos_ocupados() == 3); // Siguen siendo 3, no se movió nadie

    // Reserva de 0 personas o puntero nulo
    DebeSerCierto(reserva_multiple(0, personas) == -1);
    DebeSerCierto(reserva_multiple(2, NULL) == -1);

    elimina_sala();
    FIN_TEST("Reserva Múltiple");
}

void test_Identidades() {
    INICIO_TEST("Identidades y IDs conflictivos");
    crea_sala(10);

    // Sentar a la misma persona en varios sitios (debería permitirse según tu lógica actual)
    int id_juan = 500;
    DebeSerCierto(reserva_asiento(id_juan) == 1);
    DebeSerCierto(reserva_asiento(id_juan) == 2);

    // Intentar reservar con ID 0 (si tu código considera 0 como 'libre', esto es un peligro)
    // El ID debe ser > 0 siempre.
    DebeSerCierto(reserva_asiento(0) == -1);

    // Intentar reservar con ID negativo
    DebeSerCierto(reserva_asiento(-1) == -1);

    elimina_sala();
    FIN_TEST("Identidades");
}

void test_Fragmentacion() {
    INICIO_TEST("Fragmentación de asientos");
    crea_sala(5);

    // Llenamos la sala: [10, 20, 30, 40, 50]
    for(int i=1; i<=5; i++) reserva_asiento(i*10);

    // Vaciamos asientos alternos: [Libre, 20, Libre, 40, Libre]
    libera_asiento(1);
    libera_asiento(3);
    libera_asiento(5);

    // Ahora pedimos una reserva. Debería ocupar el primer hueco (el 1)
    DebeSerCierto(reserva_asiento(99) == 1);
    // La siguiente reserva debería ir al asiento 3
    DebeSerCierto(reserva_asiento(88) == 3);

    elimina_sala();
    FIN_TEST("Fragmentación");
}

void test_Stress() {
    INICIO_TEST("Stress y Gran Capacidad");

    int gran_capacidad = 10000;
    DebeSerCierto(crea_sala(gran_capacidad) == gran_capacidad);

    // Llenar 10.000 asientos
    for(int i=1; i<=gran_capacidad; i++) {
        if(reserva_asiento(i) == -1) {
            printf("Fallo en reserva masiva asiento %d\n", i);
            DebeSerCierto(0);
        }
    }

    DebeSerCierto(asientos_libres() == 0);
    DebeSerCierto(asientos_ocupados() == gran_capacidad);

    // Liberar todos los asientos uno a uno
    for(int i=1; i<=gran_capacidad; i++) {
        DebeSerCierto(libera_asiento(i) == i);
    }

    DebeSerCierto(asientos_libres() == gran_capacidad);
    elimina_sala();
    FIN_TEST("Stress");
}

void test_Atomicidad_Multiple() {
    INICIO_TEST("Atomicidad de Reserva Múltiple");
    crea_sala(10);

    // Dejamos solo 2 asientos libres al final
    for(int i=1; i<=8; i++) reserva_asiento(i);

    int grupo[] = {101, 102, 103, 104};
    // Intentamos sentar a 4 personas donde solo caben 2
    int resultado = reserva_multiple(4, grupo);

    // Si la implementación es correcta, resultado debe ser -1
    // Y el número de ocupados debe seguir siendo 8
    DebeSerCierto(resultado == -1);
    DebeSerCierto(asientos_ocupados() == 8);

    elimina_sala();
    FIN_TEST("Atomicidad");
}