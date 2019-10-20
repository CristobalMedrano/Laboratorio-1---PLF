#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define EXISTS 1
#define NO_EXISTS 0
#define YES 1
#define NO 0
#define TRUE 1
#define FALSE 0
#define ERROR -1
#define FINISHED 0
#define MAX_LEN_CHAR 10

static char* Pascal[] = 
    {   "letter",
        "digit",
        ".",
        "E",
        "+",
        "-",
        "NIL",
        "'",
        "character",
        "(",
        ")",
        ",",
        "..",
        "↑",
        "PACKED",
        "ARRAY",
        "[",
        "]",
        "OF",
        "FILE",
        "SET",
        "RECORD",
        "END",
        ";",
        ":",
        "CASE",
        "NOT",
        "*",
        "/",
        "DIV",
        "MOD",
        "AND",
        "OR",
        "=",
        "<",
        ">",
        "#",
        "<=",
        ">=",
        "IN",
        "FUNCTION",
        "PROCEDURE",
        ":=",
        "BEGIN",
        "IF",
        "THEN",
        "ELSE",
        "WHILE",
        "DO",
        "REPEAT",
        "UNTIL",
        "DOWNTO",
        "FOR",
        "TO",
        "WITH",
        "GOTO",
        "LABEL",
        "CONST",
        "TYPE",
        "VAR"
    };

typedef struct Line
{
    int len;
    char* currentLine;

} Line;

typedef struct Word
{
    char* word;
    struct Word* nextWord;

} Word;


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
Line*   readFile(const char* inputFile, int* lenDocument);
int     totalCharInFile(FILE *currentFile);
int     totalLineInFile(FILE* currentFile, int count);
Line*   createDocument(int lineCount, int charCount);
Line*   setLineDocument(Line* document, char* line, int pos);
void    showDocument(Line* document, int line);
void    freeDocument(Line* document, int countLine);
char*   charDetector(char* currentString, char* currentLine, char* stringDetected);
int     sameString(char* s1, char* s2);
char*   charToString(char currentChar);
int     isInPascal(char* currentString);
int     analizadorLexico(int argc, char const *argv[]);
char*   concatString(char* currentString, char* nextChar);
Word*   detectorLexico(Line* document, int lenDocument);
Word*   createListWordNode();
Word*   insertWord(Word* listWord, char* word);
int     lenListWord(Word* L);
void    showListWord(Word* L);
Word*   freeListWordNode(Word* L, int pos);
void    freeBadString(char* string);
int     saveLexico(Word* saveLexico, const char* outputFile);
int     saveEmptyFile(const char* outputFile);

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

Line* readFile(const char* inputFile, int* lenDocument)
{   
    FILE* currentFile = openFile(inputFile);
    if (NULL != currentFile)
    {      
        int count           = totalCharInFile(currentFile);            
        int lineCount       = totalLineInFile(currentFile, count);
        Line* newDocument   = createDocument(lineCount, count);
        if (NULL != newDocument)
        {
            int line = 0;
            while (0 == feof(currentFile))
            {
                char* currentLine   = (char*)malloc((count+2)*sizeof(char));
                fscanf(currentFile, "%s", currentLine);
                #ifdef DEBUG
                    printf("%s\n", currentLine);
                #endif
                newDocument         = setLineDocument(newDocument, currentLine, line);
                *lenDocument        = *lenDocument + 1;
                line++;
            }                
            closeFile(currentFile, inputFile);
            return newDocument;
        }
        closeFile(currentFile, inputFile);
        return NULL;    
    }
    return NULL;
}

int totalCharInFile(FILE* currentFile)
{
    if (NULL == currentFile)
    {
        return 0;
    }
    
    int count = 0;
    while (0 == feof(currentFile))
    {
        char countLine[MAX_LEN_CHAR] = "";
        fscanf(currentFile, "%c ", countLine);
        count++;
    }
    rewind(currentFile);
    #ifdef DEBUG
        printf("total count = %d\n", count);
    #endif
    return count;
}

int totalLineInFile(FILE* currentFile, int count)
{
    if (NULL == currentFile && 0 >= count)
    {
        return 0;
    }
    
    int totalLine = 0;
    while (0 == feof(currentFile))
    {
        char* currentLine = (char*)malloc((count+2)*sizeof(char));
        fscanf(currentFile, "%s", currentLine);
        if (NULL != currentLine)
        {
            free(currentLine);
        }
        totalLine++;
    }
    rewind(currentFile);
    #ifdef DEBUG
        printf("total line = %d\n", totalLine);
    #endif
    return totalLine;
}

Line* createDocument(int lineCount, int charCount)
{
    if (0 >= charCount)
    {
        return NULL;
    }
    
    Line* newDocument = (Line*)malloc(sizeof(Line)*lineCount);
    if (NULL != newDocument)
    {
        int line = 0;
        int i = 0;
        for (i = 0; i < lineCount; i++)
        {
            newDocument[line].currentLine   = NULL;
            newDocument[line].len           = 0; 
        }
        #ifdef DEBUG
            printf("Documento creado\n");
        #endif
        return newDocument;
    }
    return NULL;
}

Line* setLineDocument(Line* document, char* line, int pos)
{
    if (NULL != document)
    {
        document[pos].currentLine = line;
        document[pos].len = (int)strlen(line);
        return document;
    }
    return NULL;
}

void showDocument(Line* document, int line)
{
    if (NULL != document && 0 <= line)
    {
        int pos = 0;
        for (pos = 0; pos < line; pos++)
        { 
            #ifdef DEBUG
                printf("\nChar %s\n", document[pos].currentLine);
                printf("len %d\n", document[pos].len);
            #endif
        }
    }
}

void freeDocument(Line* document, int countLine)
{
    if (NULL != document && 0 < countLine)
    {
        int i = 0;
        for (i = 0; i < countLine; i++)
        {
            if (NULL != document[i].currentLine)
            {
                free(document[i].currentLine);
            }
        }
        free(document);
    }
}

Word* createListWordNode()
{
    Word* listWord = (Word*)malloc(sizeof(Word));
    if (NULL != listWord)
    {
        listWord->word      = NULL;
        listWord->nextWord  = NULL;
        return listWord;
    }
    return NULL;
}

Word* insertWord(Word* listWord, char* word)
{
    Word* new = createListWordNode();
    Word* aux = createListWordNode();
    if (NULL != new)
    {
        if (NULL == listWord)
        {
            listWord = new;
            new->word = word;
            new->nextWord = NULL;
            return listWord;
        }
        aux = listWord;
        while (NULL != aux->nextWord)
        {
            aux = aux->nextWord;
        }
        aux->nextWord = new;
        new->word = word;
        new->nextWord = NULL;
        return listWord;        
    }
    return listWord;    
}

int lenListWord(Word* L)
{
	int i = 0;
	Word* index = L;
	while(index != NULL)
	{
		index = index->nextWord;
		i++;
	}
	return i;
}

void freeListWord(Word* L)
{
    Word* node = L;
    Word* temp = NULL;
    while (NULL != node)
    {
        temp = node;
        node = node->nextWord;
        if (NULL != temp->word)
        {
            free(temp->word);
        }
        free(temp);
    } 
}

Word* detectorLexico(Line* document, int lenDocument)
{
    if (NULL != document && lenDocument >= 0)
    {
        Word* listWord = NULL;
        int i = 0;
        for (i = 0; i < lenDocument; i++)
        {
            int j = 0;
            while (j < document[i].len)
            {
                char* currentLine   = document[i].currentLine;
                char* firstChar     = charToString(currentLine[j]);
                #ifdef DEBUG
                    printf("%s %s\n", firstChar, currentLine);
                #endif
                
                char* firstLine     = &currentLine[j+1];
                char* detectedChar  = charDetector(firstChar, firstLine, "");

                if (NULL != detectedChar)
                {
                    #ifdef DEBUG
                        printf(" %s %p\n", detectedChar, detectedChar);
                    #endif
                    listWord = insertWord(listWord, detectedChar);
                    j = j + (int)strlen(detectedChar) - 1;
                }
                j++;
            }
        }
        return listWord;
    }
    return NULL;
}

void showListWord(Word* L)
{   
    if(L != NULL)
	{
        Word *aux = createListWordNode();
        aux=L;
        while(aux!=NULL)
        {
            printf("%s\n", aux->word);
            aux = aux->nextWord;
        }
	}
}

char* charToString(char currentChar)
{   
    #ifdef DEBUG
        printf("%c\n", currentChar);
    #endif
	char* string = (char*)malloc(sizeof(char)*256);
	if (NULL != string)
    {
        string[0] = currentChar;
        string[1] = '\0';
        return string;
    }
	return NULL;
}

char* charDetector(char* currentString, char* currentLine, char* stringDetected)
{
    #ifdef DEBUG
            printf("%s\n", currentString);
        #endif
    // El caso base es cuando la linea a leer se queda sin caracteres o cuando el largo del string a
    // leer supera al maximo caracter a ser detectado.
    if (0 == strlen(currentLine) || MAX_LEN_CHAR < (int)strlen(currentString))
    {
        // Esta verificacion se hace en caso de que el ultimo string detectado, si forme parte
        // del lexico a leer.
        if (YES == isInPascal(currentString))
        {
            return currentString;
        }
        if (YES == isInPascal(stringDetected))
        {
            return stringDetected;
        }
        return NULL;
    }

    char* nextChar = charToString(currentLine[0]);
    char* nextLine = &currentLine[1];

    if (YES == isInPascal(currentString))
    {
        #ifdef DEBUG
            printf("%s\n", stringDetected);
        #endif
        stringDetected = currentString;
    }
    return charDetector(concatString(currentString, nextChar), nextLine, stringDetected);
}

int isInPascal(char* currentString)
{
    if (MAX_LEN_CHAR < (int)strlen(currentString))
    {
        return NO;
    }
    
    int lenPascal = (int)(sizeof(Pascal)/sizeof(Pascal[0]));
    int i = 0;
    while (i < lenPascal)
    {
        if (YES == sameString(currentString, Pascal[i]))
        {
            return YES;
        }
        i++;
    }
    return NO;    
}

char* concatString(char* currentString, char* nextChar)
{
    if (NULL == currentString || NULL == nextChar)
    {
        return NULL;
    }

    char* src = (char*)malloc(sizeof(char)*(MAX_LEN_CHAR+5));
    strcpy(src, currentString);
    strcat(src, nextChar);
    return src;
}

int sameString(char* s1, char* s2)
{
    if (0 == strcmp(s1, s2))
    {
        return YES;
    }
    return NO;
}

int saveLexico(Word* list, const char* outputFile)
{
    if (NULL != list)
    {
        FILE* newFile = fopen(outputFile, "wb");
        if (NULL != newFile)
        {
            Word *aux = createListWordNode();
            aux = list;
            while(aux!=NULL)
            {
                if (YES == sameString(aux->word, Pascal[13]))
                {
                    fprintf(newFile, "%s\n", aux->word);
                }
                else
                {
                    int i = 0;
                    while (aux->word[i])
                    {
                        fprintf(newFile, "%c", toupper(aux->word[i]));
                        i++;
                    }
                    fprintf(newFile, "\n");
                }
                aux = aux->nextWord;
            }      
            return YES;
        }
    }
    return NO;
}

int saveEmptyFile(const char* outputFile)
{
    FILE* newFile = fopen(outputFile, "wb");
    if (NULL != newFile)
    {
        fprintf(newFile, "\n");
        return YES;
    }      
    return NO;
}


int analizadorLexico(int argc, char const *argv[])
{
    if (NO == wrongInputParam(argc))
    {
        if (NO == wrongInputFileParam(argv))
        {
            #ifdef DEBUG
                printf("Iniciando ejecucion sin errores.\n");
            #endif
            int lenDocument = 0;
            Line* document = readFile(argv[1], &lenDocument);
            if (0 == lenDocument)
            {
                saveEmptyFile(argv[2]);
            }
            else
            {
                if (NULL != document)
                {
                    Word* listWord = detectorLexico(document, lenDocument);
                    if (NULL != listWord)
                    {
                        #ifdef DEBUG
                            showListWord(listWord);
                        #endif
                        int saved = saveLexico(listWord, argv[2]);
                        if (YES == saved)
                        {
                            freeDocument(document, lenDocument);            
                            return YES;
                        }
                        freeDocument(document, lenDocument);
                        return NO;
                    }
                    else
                    {
                        saveEmptyFile(argv[2]);
                        freeDocument(document, lenDocument);
                        return YES;
                    }
                }
                return NO;
            }
        }
        return NO;
    }
    return NO;
}

int main(int argc, char const *argv[])
{
    #ifdef DEBUG
        printf("Debug Mode.\n");
    #endif
    if (NO == analizadorLexico(argc, argv))
    {
        #ifdef DEBUG
            printf("Error en la ejecucion.%c\n", 24);
        #endif
        return ERROR;
    }    
    return FINISHED;
}

