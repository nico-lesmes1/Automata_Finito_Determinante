#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100       // longitud máxima de una línea
#define MAX_T 100     // máximo número de transiciones

int main(void) {
    /* 1. Leer configuración ------------------------------------------ */
    FILE *f = fopen("conf.txt", "r");
    if (!f) { puts("No se pudo abrir conf.txt"); return 1; }

    char line[MAX];
    char estados[MAX][MAX];
    char alfabeto[MAX];
    char trans_origen[MAX_T][MAX];
    char trans_simbolo[MAX_T];
    char trans_destino[MAX_T][MAX];
    char inicial[MAX], finales[MAX][MAX];
    int n_estados=0, n_alfabeto=0, n_trans=0, n_finales=0;

    /* 1.1 estados (línea 2) */
    fgets(line, MAX, f);               /* salto comentario */
    fgets(line, MAX, f);               /* leer estados */
    char *tok = strtok(line, ",\n");
    while (tok) { strcpy(estados[n_estados++], tok); tok = strtok(NULL, ",\n"); }

    /* 1.2 alfabeto (línea 4) */
    fgets(line, MAX, f);               /* salto comentario */
    fgets(line, MAX, f);               /* leer alfabeto */
    tok = strtok(line, ",\n");
    while (tok) { alfabeto[n_alfabeto++] = tok[0]; tok = strtok(NULL, ",\n"); }

    /* 1.3 transiciones (líneas 6-9) */
    fgets(line, MAX, f);               /* salto comentario */
    while (fgets(line, MAX, f) && line[0] != 'q') {   /* hasta la línea del inicial */
        char o[MAX], d[MAX]; char s;
        sscanf(line, "%[^,],%c,%s", o, &s, d);
        strcpy(trans_origen[n_trans], o);
        trans_simbolo[n_trans] = s;
        strcpy(trans_destino[n_trans], d);
        n_trans++;
    }
    /* 1.4 estado inicial (última línea antes de finales) */
    sscanf(line, "%s", inicial);

    /* 1.5 estados finales */
    fgets(line, MAX, f);               /* salto comentario */
    tok = strtok(line, ",\n");
    while (tok) { strcpy(finales[n_finales++], tok); tok = strtok(NULL, ",\n"); }
    fclose(f);

    /* 2. Simular ----------------------------------------------------- */
    FILE *c = fopen("cadenas.txt", "r");
    if (!c) { puts("No se pudo abrir cadenas.txt"); return 1; }

    char cad[MAX];
    while (fgets(cad, MAX, c)) {
        cad[strcspn(cad, "\n")] = 0;   /* quitar \n */
        char estado[MAX];
        strcpy(estado, inicial);

        for (int i = 0; cad[i]; ++i) {
            char simb = cad[i];
            char nuevo[MAX] = "";
            /* buscar transición */
            for (int t = 0; t < n_trans; ++t) {
                if (strcmp(trans_origen[t], estado) == 0 && trans_simbolo[t] == simb) {
                    strcpy(nuevo, trans_destino[t]);
                    break;
                }
            }
            if (nuevo[0] == 0) { strcpy(estado, "ERROR"); break; }
            strcpy(estado, nuevo);
        }

        /* ¿está en finales? */
        int acepta = 0;
        for (int f = 0; f < n_finales; ++f)
            if (strcmp(estado, finales[f]) == 0) { acepta = 1; break; }

        printf("%s : %s\n", cad, acepta ? "acepto" : "no acepto");
    }
    fclose(c);
    return 0;
}