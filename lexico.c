#include <stdio.h>
#include <stdlib.h>

#define EXISTS 1
#define NO_EXISTS 0
#define YES 1
#define NO 0
#define TRUE 1
#define FALSE 0

void    errorParamType1();
void    errorParamType2();
void    errorParamType3();
void    errorParamType4();
void    errorParamType5();
int     wrongInputParam(int argc);
FILE*   openFile(const char* fileName);
int     closeFile(FILE* file, const char* fileName);
int     checkFile(const char* fileName);
int     wrongInputFileParam(char const *argv[]);

/*  Type 1

    C:\>lexico.exe
    Error: Faltan parámetros.
    Uso: lexico.exe archivo_entrada archivo_salida
    
    C:\>
*/
void errorParamType1()
{
    printf("Error: Faltan par%cmetros.\n", 160);
    printf("Uso: lexico.exe archivo_entrada archivo_salida\n");
}

/*  Type 2

    C:\> lexico.exe archivo
    Error: Falta parámetro.
    Uso: lexico.exe archivo_entrada archivo_salida
    
    C:\>
*/
void errorParamType2()
{
    printf("Error: Falta par%cmetro.\n", 160);
    printf("Uso: lexico.exe archivo_entrada archivo_salida\n");
}

/*  Type 3

    C:\> lexico.exe archivo_1 archivo_2 archivo_3
    Error: Demasiados parámetros.
    Uso: lexico.exe archivo_entrada archivo_salida
        
    C:\>
*/
void errorParamType3()
{
    printf("Demasiados par%cmetros.\n", 160);
    printf("Uso: lexico.exe archivo_entrada archivo_salida\n");
}

/*  Type 4

    C:\> lexico.exe archivo_entrada archivo_salida
    Error: El archivo de entrada no existe.
        
    C:\>
*/
void errorParamType4()
{
    printf("Error: El archivo de entrada no existe.\n");
}

/*  Type 5

    C:\> lexico.exe archivo_entrada archivo_salida
    Error: El archivo de salida ya existe.
    
    C:\>
*/
void errorParamType5()
{
    printf("Error: El archivo de salida ya existe.\n");
}

int wrongInputParam(int argc)
{
    if (1 == argc)
    {
        errorParamType1();
        return YES;
    }
    else if (2 == argc)
    {
        errorParamType2();
        return YES;
    }
    else if (3 < argc)
    {
        errorParamType3();
        return YES;
    }

    #ifdef DEBUG
        printf("No hay errores en la cantidad de parametros.\n");
    #endif
    
    return NO;
}

FILE* openFile(const char* fileName)
{
    FILE* currentFile = fopen(fileName, "rb");
    if(NULL != currentFile)
    {
        #ifdef DEBUG
            printf("Archivo '%s' abierto\n", fileName);
        #endif
        return currentFile;
    }
    #ifdef DEBUG
        printf("Error: El archivo '%s' no existe.\n", fileName);
    #endif
    return NULL;
}

int closeFile(FILE* file, const char* fileName)
{
    if (NULL != file && NULL != fileName) 
    {
        if(0 == fclose(file))
        {
            #ifdef DEBUG
                printf("Archivo '%s' cerrado.\n", fileName);
            #endif
            return YES;
        }
    }
    #ifdef DEBUG
        printf("Error al cerrar el archivo '%s'.\n", fileName);
    #endif
    return NO;
}

int checkFile(const char* fileName)
{
    FILE* currentFile = openFile(fileName);
    if (NULL != currentFile)
    {
        #ifdef DEBUG
            printf("El archivo existe\n");
        #endif
        closeFile(currentFile, fileName);
        return EXISTS;
    }
    else
    {
        #ifdef DEBUG
            printf("El archivo no existe\n");
        #endif
        return NO_EXISTS;
    }  
}

int wrongInputFileParam(char const *argv[])
{
    if (NO_EXISTS == checkFile(argv[1]))
    {
        errorParamType4();
        return YES;
    }
    else if (EXISTS == checkFile(argv[2]))
    {
        errorParamType5();
        return YES;
    }  

    #ifdef DEBUG
        printf("No hay errores con los archivos ingresados.\n");
    #endif
    
    return NO;
}

int main(int argc, char const *argv[])
{
    #ifdef DEBUG
        printf("Debug Mode.\n");
    #endif

    if (NO == wrongInputParam(argc))
    {
        if (NO == wrongInputFileParam(argv))
        {
            #ifdef DEBUG
                printf("Iniciando ejecucion sin errores.\n");
            #endif
        }
    }
    
    return 0;
}
