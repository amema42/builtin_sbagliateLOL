#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char **environ;

// Funzione per impostare una variabile d'ambiente personalizzata
int my_setenv(const char *name, const char *value) {
    size_t len = strlen(name) + strlen(value) + 2; // +2 per '=' e '\0'
    char *env_var = (char *)malloc(len);
    if (env_var == NULL) {
        perror("Errore di allocazione di memoria");
        return -1;
    }
    snprintf(env_var, len, "%s=%s", name, value);

    // Cerca se la variabile d'ambiente esiste già
    int index = 0;
    while (environ[index] != NULL) {
        char *env_name = strtok(environ[index], "=");
        if (env_name && strcmp(env_name, name) == 0) 
        {
            environ[index] = env_var; // Aggiorna il valore della variabile
            return 0;
        }
        index++;
    }

    // Se la variabile d'ambiente non esiste, aggiungila alla lista
    environ[index] = env_var;
    environ[index + 1] = NULL;

    return 0; // Operazione completata con successo
}

// Funzione per gestire il comando export
void my_export(const char *arg) {
    char *arg_copy = strdup(arg);
    if (arg_copy == NULL) {
        fprintf(stderr, "Errore nell'allocazione della memoria\n");
        return;
    }

    char *name = strtok(arg_copy, "=");
    char *value = strtok(NULL, "");

    if (name == NULL || value == NULL) {
        fprintf(stderr, "Errore: formato non valido per export\n");
        free(arg_copy);
        return;
    }

    if (my_setenv(name, value) != 0) {
        fprintf(stderr, "Errore nell'impostare la variabile d'ambiente\n");
    }

    free(arg_copy);
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        my_export(argv[1]);
    } else {
        for (char **env = environ; *env != NULL; ++env) {
            printf("%s\n", *env);
        }
    }

    return 0;
}
