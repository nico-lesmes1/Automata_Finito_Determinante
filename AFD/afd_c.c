#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 10
#define MAX_T 10

int main(void) {
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

    fgets(line, MAX, f);               
    fgets(line, MAX, f);               
    char *tok = strtok(line, ",\n");
    while (tok) { strcpy(estados[n_estados++], tok); tok = strtok(NULL, ",\n"); }

    fgets(line, MAX, f);               
    fgets(line, MAX, f);               
    tok = strtok(line, ",\n");
    while (tok) { alfabeto[n_alfabeto++] = tok[0]; tok = strtok(NULL, ",\n"); }

    fgets(line, MAX, f);               
    while (fgets(line, MAX, f) && line[0] != 'q') {   
        char o[MAX], d[MAX]; char s;
        sscanf(line, "%[^,],%c,%s", o, &s, d);
        strcpy(trans_origen[n_trans], o);
        trans_simbolo[n_trans] = s;
        strcpy(trans_destino[n_trans], d);
        n_trans++;
    }
    sscanf(line, "%s", inicial);

    fgets(line, MAX, f);               
    tok = strtok(line, ",\n");
    while (tok) { strcpy(finales[n_finales++], tok); tok = strtok(NULL, ",\n"); }
    fclose(f);

    FILE *c = fopen("cadenas.txt", "r");
    if (!c) { puts("No se pudo abrir cadenas.txt"); return 1; }

    char cad[MAX];
    while (fgets(cad, MAX, c)) {
        cad[strcspn(cad, "\n")] = 0;   
        char estado[MAX];
        strcpy(estado, inicial);

        for (int i = 0; cad[i]; ++i) {
            char simb = cad[i];
            char nuevo[MAX] = "";
            for (int t = 0; t < n_trans; ++t) {
                if (strcmp(trans_origen[t], estado) == 0 && trans_simbolo[t] == simb) {
                    strcpy(nuevo, trans_destino[t]);
                    break;
                }
            }
            if (nuevo[0] == 0) { strcpy(estado, "ERROR"); break; }
            strcpy(estado, nuevo);
        }

        int acepta = 0;
        for (int f = 0; f < n_finales; ++f)
            if (strcmp(estado, finales[f]) == 0) { acepta = 1; break; }

        printf("%s : %s\n", cad, acepta ? "acepta" : "no acepta");
    }
    fclose(c);
    return 0;

}


