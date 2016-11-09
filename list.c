#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h> /* Enlazar con libm.a (-lm; ref: http://stackoverflow.com/questions/10409032/why-am-i-getting-undefined-reference-to-sqrt-error-even-though-i-include-math) */

#define TRUE 1
#define FALSE 0

#define MAX_DIGITS 5 /* 5 dígitos */
#define MAX_LEN 32 /* 32 caracteres */
#define MAX_SAMPLE 100 /* Numero máximo de elementos (jitter) */

#define MAX_TEXT MAX_LEN + 1 /* MAX_LEN + '\0' (relativo) */

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x) /* Ref: http://stackoverflow.com/questions/240353/convert-a-preprocessor-token-to-a-string */

typedef struct component {
    unsigned int id; /* Código de componente (cinco dígitos) */
    char * manufacturer; /* Puntero a texto Fabricante --crear con malloc (tamaño MAX_TEXT) */
    float price; /* Precio */
    unsigned int n; /* Disponibilidad */
} Componente;

typedef struct node * Lista;
typedef struct node {
    Componente * data; /* Puntero a Componente */
    Lista next; /* Puntero al siguiente Nodo */
} Nodo;

Lista Lista_Crear(void);

void Lista_Agregar(Lista *lista, const Componente *elemento);

Componente* Lista_Valor(const Lista *lista, unsigned pos);

void Lista_Extraer(Lista *lista);

void Lista_Vaciar(Lista *lista);

double mean(double *sample, unsigned int length); /* Media aritmética, llamada por la función std (desviación típica) */

double std(double *sample, unsigned int length); /* desviación típica */

int show_menu(void); /* mostrar menú (User Interface) */

void clear(void); /* vacía el buffer stdin (final del buffer marcado por '\n') */

void pause(void); /* muestra un mensaje pidiendo permiso antes de continuar (antes de system("clear")) */

//TODO: handle malloc errors?
int main(int argc, char** argv)
{
    int n;
    Componente *ptr;
    Componente componente;
    Lista list = Lista_Crear();

    double sample[MAX_SAMPLE];
    clock_t start, end;

    char buffer[MAX_TEXT];
    componente.manufacturer = buffer;

    int exit = FALSE;
    int opt;

    while (!exit)
    {
        opt = show_menu();
        switch (opt)
        {
        case 1:
            printf("Código: ");
            scanf("%" TOSTRING(MAX_DIGITS) "u", &componente.id); /* MAX_DIGITS dígitos */
            clear();

            printf("Fabricante: ");
            scanf("%" TOSTRING(MAX_LEN) "s", buffer); /* MAX_TEXT - 1 caracteres (dinámico) */
            clear();

            printf("Precio: ");
            scanf("%f", &componente.price);
            clear();

            printf("Disponibilidad: ");
            scanf("%u", &componente.n);
            clear();

            Lista_Agregar(&list, &componente);

            break;

        case 2:
            Lista_Extraer(&list);

            break;

        case 3:
            printf("Posición: ");
            scanf("%u", &n);
            clear();

            ptr = Lista_Valor(&list, n);

            if (ptr != NULL)
            {
                printf("Código: %u\n", ptr->id);
                printf("Fabricante: %s\n", ptr->manufacturer);
                printf("Precio: %f\n", ptr->price);
                printf("Disponibilidad: %u\n", ptr->n);
            }
            else
                printf("Posición no válida\n");

            break;

        case 4:
            Lista_Vaciar(&list);

            break;

        case 5:
            /* Crear nuevo componente (por defecto) */
            componente.id = 99999;
            componente.manufacturer = "default";
            componente.price = 0.f;
            componente.n = 0;

            for (n = 0; n < MAX_SAMPLE; n++)
            { /* Ref: https://www.gnu.org/software/libc/manual/html_node/CPU-Time.html */
                start = clock();
                Lista_Agregar(&list, &componente);
                end = clock();

                sample[n] = ((double) (end - start)) / CLOCKS_PER_SEC * 1000.; /* milisegundos */
            }

            printf("Jitter: %f (ms)\n", std(sample, MAX_SAMPLE));

            break;

        case 6:
            exit = TRUE;

            break;

        default:
            printf("Opción no válida\n");
        }

        pause();
    }
}

Lista Lista_Crear(void)
{
    return (NULL); /* Devuelve un puntero no válido (lista vacia) */
}

void Lista_Agregar(Lista *lista, const Componente *elemento) /* agrega una COPIA de elemento al final de la lista */
{
    size_t len;
    Componente * componente; /* Variable auxiliar (componente eq. (*lista)->data) */

    while (*lista != NULL)  /* Recore *lista */
        lista = &((*lista)->next);

    *lista = malloc(sizeof(Nodo)); /* Añade un nuevo nodo al final */

    /* Copia *elemento en *componente */
    componente = malloc(sizeof(Componente));
    componente->id = elemento->id;
    componente->price = elemento->price;
    componente->n = elemento->n;

    /* Copia elemento->manufacturer a un nuevo buffer */
    len = strlen(elemento->manufacturer);
    componente->manufacturer = malloc(len + 1); /* strlen + '\0' */
    strcpy(componente->manufacturer, elemento->manufacturer); //Ref: https://linux.die.net/man/3/strcpy

    (*lista)->data = componente; /* Asigna la dirección del nuevo componente */
    (*lista)->next = NULL; /* Indica el fin de lista */
}

Componente* Lista_Valor(const Lista *lista, unsigned pos) /* Si pos >= n (donde n es el número de elementos) o p < 0, devuelve NULL */
{
    int i = 0;
    while ((*lista != NULL) && (i++ < pos)) /* Recore *lista hasta elemento i o fin de lista */
        lista = &((*lista)->next);

    if (*lista != NULL)
        return ((*lista)->data); /* Devuelve el puntero a Componente */
    else
        return (NULL); /* Devuelve puntero no válido si pos >= n */
}

void Lista_Extraer(Lista *lista)
{
    if (*lista != NULL) /* Si la lista no está vacía... */
    {
        while ((*lista)->next != NULL) /* Recore *lista hasta el último elemento (*lista apunta al último Nodo) */
            lista = &((*lista)->next);

        free((*lista)->data->manufacturer);
        free((*lista)->data);
        free(*lista);

        *lista = NULL; /* Nuevo fin de la lista */
    }
}

void Lista_Vaciar(Lista *lista)
{
    /* Lazy way... */
//    while (*lista != NULL)
//        Lista_Extraer(lista);

    Lista next;
    while (*lista != NULL) /* Recore *lista */
    {
        next = (*lista)->next; /* Necesario para asegurar que no pertemos el puntero al siguiente Nodo */

        free((*lista)->data->manufacturer);
        free((*lista)->data);
        free(*lista);

        *lista = next; /* Siguiente Nodo */
    }
}

double mean(double *sample, unsigned int length)
{ /* Ref: https://es.mathworks.com/help/matlab/ref/mean.html */
    unsigned int i;
    double sum = 0.0;

    if (length == 0)
        return NAN;

    for (i = 0; i < length; i++)
        sum += sample[i];

    return (sum/((double) length));
}

double std(double *sample, unsigned int length)
{ /* Ref: https://es.mathworks.com/help/matlab/ref/std.html */
    unsigned int i;
    double sum = 0.0, m;

    if (length == 0)
        return NAN;
    else if (length == 1)
        return 0;

    m = mean(sample, length);

    for (i = 0; i < length; i++)
        sum += pow(sample[i] - m, 2);

    return (sqrt(sum/((double) length - 1)));
}

int show_menu(void)
{
    int opt;

    system("clear");

    printf("1. Añadir componente a la lista\n");
    printf("2. Borrar componente de la lista\n");
    printf("3. Imprimir en pantalla componente\n");
    printf("4. Vaciar lista\n");
    printf("5. Medir jitter\n");
    printf("6. Salir\n");
    printf("Opción: ");

    scanf("%d", &opt);
    clear();

    return (opt);
}

void clear(void)
{ /* Ref: http://stackoverflow.com/questions/7898215/how-to-clear-input-buffer-in-c */
    int ch;

    do
       ch = getchar();
    while ((ch != '\n') && (ch != EOF));
}

void pause(void)
{
    int ch;

    printf("Presione [ENTER] para continuar...");
    ch = getchar();
    if ((ch != '\n') && (ch != EOF))
        clear();
}
