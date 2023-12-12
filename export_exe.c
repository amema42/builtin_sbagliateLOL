#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ENV_VARS 100
#define MAX_ENV_VAR_LENGTH 50

typedef struct {
    char name[MAX_ENV_VAR_LENGTH];
    char value[MAX_ENV_VAR_LENGTH];
} EnvVar;

EnvVar environment[MAX_ENV_VARS];
int num_environment_vars = 0;

void print_environment() {
    for (int i = 0; i < num_environment_vars; i++) {
        printf("%s=%s\n", environment[i].name, environment[i].value);
    }
}

int find_env_variable(const char *var_name) {
    for (int i = 0; i < num_environment_vars; i++) {
        if (strcmp(environment[i].name, var_name) == 0) {
            return i;
        }
    }
    return -1;
}

int set_env_variable(const char *var_name, const char *var_value) {
    int index = find_env_variable(var_name);
    if (index != -1) {
        strcpy(environment[index].value, var_value);
    } else {
        if (num_environment_vars < MAX_ENV_VARS) {
            strcpy(environment[num_environment_vars].name, var_name);
            strcpy(environment[num_environment_vars].value, var_value);
            num_environment_vars++;
        } else {
            fprintf(stderr, "troppe variabili\n");
            return -1;
        }
    }
    return 0;
}

int execute_export(char *command) {
    // Analizza il comando per ottenere la variabile d'ambiente
    char *token = strtok(command, " ");
    token = strtok(NULL, " ");

    if (token == NULL) {
        // Mostra tutte le variabili di ambiente se non ci sono parametri
        print_environment();
        return 0;
    }

    // Estrae il nome della variabile e il suo valore dalla stringa di comando
    char *name = strtok(token, "=");
    char *value = strtok(NULL, "=");

    if (name != NULL) {
        if (value != NULL) {
            // Se è specificato un valore, imposta/aggiorna la variabile d'ambiente
            set_env_variable(name, value);
        } else {
            // Se non è specificato un valore, cerca e mostra il valore della variabile d'ambiente
            int index = find_env_variable(name);
            if (index != -1) {
                printf("%s=%s\n", environment[index].name, environment[index].value);
            }
        }
    } else {
        fprintf(stderr, "Sintassi non valida. Utilizzo corretto: export VARIABLE[=VALORE]\n");
        return -1;
    }

    return 0;
}

int main() {
    // Esempio di utilizzo della funzione export
    char command[] = "export HOME=/home/user";
    execute_export(command);

    // Esegui la funzione con i comandi inseriti dall'utente
    char user_input[100];
    printf("Enter command: ");
    fgets(user_input, sizeof(user_input), stdin);

    // Rimuovi il carattere di nuova riga da user_input se presente
    user_input[strcspn(user_input, "\n")] = '\0';

    // Esegui il comando "export" inserito dall'utente
    execute_export(user_input);

    return 0;
}
