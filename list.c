#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h> /* Enlazar con libm.a (-lm; ref: http://stackoverflow.com/questions/10409032/why-am-i-getting-undefined-reference-to-sqrt-error-even-though-i-include-math) */

#define TRUE 1
#define FALSE 0

#define MAX_TEXT 33 /* 32 caracteres + '\0' */
#define MAX_SAMPLE 100 /* Numero máximo de elementos (jitter) */

typedef struct component {
    unsigned int id; /* Código de componente (< 100000, cinco dígitos) */
    char * manufacturer; /* Puntero a texto Fabricante --crear con malloc (tamaño MAX_TEXT) */
    float price; /* Precio */
    unsigned int n; /* Disponibilidad */
} Componente;
//typedef unsigned int Componente;

typedef struct node * Lista;
typedef struct node {
    Componente * data;
    Lista next;
} Nodo;

Lista Lista_Crear(void);

void Lista_Agregar(Lista *lista, const Componente *elemento);

Componente* Lista_Valor(const Lista *lista, unsigned pos);

void Lista_Extraer(Lista *lista);

/* Crear un array dinámico de punteros para hacer free (realloc del array si es necesario) */
//Es necesario hacer free a cada Nodo.data (no hace falta guardarlo en el array de free, se hace sobre la marcha)
void Lista_Vaciar(Lista *lista);

double mean(double *sample, unsigned int length);

double std(double *sample, unsigned int length);

int show_menu(void);

void clear(void);

void pause(void);

//TODO: handle malloc errors?
int main(int argc, char** argv)
{
    int n;
    Componente *ptr;
    Componente componente;
    Lista list = Lista_Crear();

    double sample[MAX_SAMPLE];
    clock_t start, end;

    char format[MAX_TEXT];
    char buffer[MAX_TEXT];

    /* Ref: http://stackoverflow.com/questions/1621394/how-to-prevent-scanf-causing-a-buffer-overflow-in-c */
    snprintf(format, MAX_TEXT - 1, "%%%ds", MAX_TEXT - 1); /* eq. "%32s" (with MAX_TEXT == 33) */
    componente.manufacturer = buffer;

    int exit = FALSE;
    int opt;

    while (!exit)
    {
        opt = show_menu();
        switch (opt)
        {
        case 1:
            //TODO: comprobación 5 dígitos (< 100000)
            printf("Código: ");
            scanf("%u", &componente.id);
            clear();

            printf("Fabricante: ");
            scanf(format, buffer);
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
            /* Crear nuevo componente */
            componente.id = 99999;
            componente.manufacturer = "default";
            componente.price = 0.f;
            componente.n = 0;

            for (n = 0; n < MAX_SAMPLE; n++)
            { /* Ref: https://www.gnu.org/software/libc/manual/html_node/CPU-Time.html */
                start = clock();
                Lista_Agregar(&list, &componente);
                end = clock();

                sample[n] = ((double) (end - start)) / CLOCKS_PER_SEC * 1000.;
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

//int main(int argc, char** argv)
//{
//    int x;
//    char manf[MAX_TEXT];
//    const char test[MAX_TEXT] = "probando...";
//    Componente componente;
//    Lista list = Lista_Crear();

//    printf("00 Hello World\n");
//    scanf("%d", &x);
//    clear();

//    manf[32] = 13;

////    char *buffer = 0;
////    scanf("%ms", &buffer);
////    printf("alloc len: %u\n", (unsigned) strlen(buffer));

//    scanf("%32s", manf);
//    printf("len: %u\n", (unsigned) strlen(manf));
//    printf("manf[32]: %u\n", (unsigned) manf[32]);

//    char c;
//    scanf("%c", &c);
//    int ret = scanf("%c", &c);
//    printf("next: %u\n", (unsigned) c);
//    printf("ret: %d\n", ret);

//    componente.id = (unsigned int) x;
//    componente.manufacturer = manf;

//    clock_t start, end;

////    start = clock();
//    //Lista_Agregar(&list, &componente);
//    time_t start_t, end_t;
//    double slept;

//    time(&start_t);
//    do
//    {
//        time(&end_t);
//    } while ((slept = difftime(end_t, start_t)) < 1);

//    start_t = end_t;
//    start = clock();
//    do
//    {
//        end = clock();
//        time(&end_t);
//    } while ((slept = difftime(end_t, start_t)) < 5);

//    double elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;

//    printf("Elapsed (clock): %g\n", elapsed*1e3); //milliseconds
//    printf("Elapsed (time): %g\n", slept*1e3); //milliseconds

////    double elapsed = difftime(end, start);
////    printf("Elapsed time (ms): %g\n", elapsed*1e3);

//    Lista_Agregar(&list, &componente);
//    memcpy(manf, test, strlen(test)+1);
//    Lista_Agregar(&list, &componente);

//    if (list == NULL)
//    {
//        printf("NULL\n");
//    }
//    else
//    {
//        Componente * ptr;

////        printf("%d\n", (*list->data)); //first
////        printf("%d\n", (*(list->next->data))); //second

////        printf("%s\n", list->data->manufacturer); //first
////        printf("%s\n", list->next->data->manufacturer); //second

//        ptr = Lista_Valor(&list, 0);
//        if (ptr != NULL)
//            printf("%s\n", ptr->manufacturer); //first

//        ptr = Lista_Valor(&list, 1);
//        if (ptr != NULL)
//            printf("%s\n", ptr->manufacturer); //second

////        ptr = Lista_Valor(&list, 2);
////        if (ptr != NULL)
////            printf("%s\n", ptr->manufacturer); //third
////        else
////            printf("NULL\n");

////        Lista_Extraer(&list);
////        Lista_Extraer(&list);
//        Lista_Vaciar(&list);

//        if (list == NULL)
//            printf("Lista vacia...\n");

////        ptr = Lista_Valor(&list, 0);
////        if (ptr != NULL)
////            printf("%s\n", ptr->manufacturer); //second
////        else
////            printf("NULL\n");
//    }

//    return 0;
//}

Lista Lista_Crear(void)
{
    return (NULL); /* Devuelve un puntero no válido (lista vacia) */
}

void Lista_Agregar(Lista *lista, const Componente *elemento) /* agrega una COPIA de elemento al final de la lista */
{
    size_t len;
    Componente * componente; /* Variable auxiliar (componente eq. (*lista)->data) */
    void * ptr;

    while (*lista != NULL)  /* Recore *lista */
        lista = &((*lista)->next);

    ptr = malloc(sizeof(Nodo));
    *lista = ptr;
    printf("malloc: %p\n", ptr);
//    *lista = malloc(sizeof(Nodo)); /* Añade un nuevo nodo al final */

    /* Copia *elemento en *componente */
    ptr = malloc(sizeof(Componente));
    componente = ptr;
    printf("malloc: %p\n", ptr);
//    componente = malloc(sizeof(Componente));
    componente->id = elemento->id;
    componente->price = elemento->price;
    componente->n = elemento->n;

    /* Copia elemento->manufacturer a un nuevo buffer */
    len = strlen(elemento->manufacturer);
    ptr = malloc(len + 1); /* strlen + '\0' */
    componente->manufacturer = ptr;
    printf("malloc: %p\n", ptr);
//    componente->manufacturer = malloc(MAX_TEXT);
    strcpy(componente->manufacturer, elemento->manufacturer); //Ref: strcpy man

    (*lista)->data = componente; /* Asigna la dirección del nuevo componente */
    (*lista)->next = NULL; /* Indica el fin de lista */
}

Componente* Lista_Valor(const Lista *lista, unsigned pos) /* Si pos >= n (donde n es el número de elementos), devuelve NULL */
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
    void * ptr;

    if (*lista != NULL) /* Si la lista no está vacía... */
    {
        while ((*lista)->next != NULL) /* Recore *lista hasta el Nodo anterior al último elemento */
            lista = &((*lista)->next);

        ptr = (*lista)->data->manufacturer;
        free(ptr);
        printf("free: %p\n", ptr);
//        free((*lista)->data->manufacturer);

        ptr = (*lista)->data;
        free(ptr);
        printf("free: %p\n", ptr);
//        free((*lista)->data);

        ptr = *lista;
        free(ptr);
        printf("free: %p\n", ptr);
//        free(*lista);

        *lista = NULL; /* Nuevo fin de la lista */
    }
}

void Lista_Vaciar(Lista *lista)
{
    /* Lazy way... */
//    while (*lista != NULL)
//        Lista_Extraer(lista);

    void * ptr;

    Lista next;
    while (*lista != NULL)
    {
        next = (*lista)->next;

        ptr = (*lista)->data->manufacturer;
        free(ptr);
        printf("free: %p\n", ptr);
//        free((*lista)->data->manufacturer);

        ptr = (*lista)->data;
        free(ptr);
        printf("free: %p\n", ptr);
//        free((*lista)->data);

        ptr = *lista;
        free(ptr);
        printf("free: %p\n", ptr);
//        free(*lista);

        *lista = next;
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
