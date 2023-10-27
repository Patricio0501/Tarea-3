#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "list.h"

void loadDocuments(TrieNode* root, const char* filenames, BookList* bookList, Map* titleContentMap) {
  char* filenames_copy = strdup(filenames);  // Copia mutable de filenames
      char* filename = strtok(filenames_copy, " ");

      while (filename != NULL) {
          // Cargar el contenido del archivo y procesarlo
          FILE* file = fopen(filename, "r");
          if (file) {
              char word[MAX_WORD_LENGTH];
              char title[MAX_WORD_LENGTH];  // Variable para almacenar el título

              // Inicializar el título y contenido como cadenas vacías
              title[0] = '\0';
              char content[1000];
              content[0] = '\0';

              // Variables para contar palabras y caracteres
              int wordCount = 0;
              int charCount = 0;

              while (fgets(word, sizeof(word), file) != NULL) {
                  // Buscar el título en las primeras líneas del archivo
                  if (strncmp(word, "Title:", 6) == 0) {
                      // Si la línea comienza con "Title:", asignar el título
                      const char* titleStart = word + 6; // Saltar "Title:"
                      // Eliminar espacios en blanco al principio
                      while (isspace(*titleStart)) {
                          titleStart++;
                      }
                      // Eliminar espacios en blanco al final
                      int len = strlen(titleStart);
                      while (len > 0 && isspace(titleStart[len - 1])) {
                          len--;
                      }
                      // Copiar el título
                      strncpy(title, titleStart, len);
                      title[len] = '\0';
                  }
                  // Limpia el contenido para retener solo palabras válidas
                  cleanContent(content);

                  // Actualizar el contenido del libro
                  strncat(content, word, sizeof(content) - strlen(content) - 1);
                  charCount += strlen(word);

                  // Contar palabras en cada línea o palabra leída
                  int inWord = 0;
                  for (int i = 0; word[i] != '\0'; i++) {
                      if (isalpha(word[i])) {
                          if (!inWord) {
                              inWord = 1;
                              wordCount++;
                          }
                      } else {
                          inWord = 0;
                      }
                  }
              }

              // Insertar el título en la lista de libros
              strncpy(bookList->books[bookList->count].title, title, sizeof(bookList->books[bookList->count].title) - 1);
              bookList->books[bookList->count].title[sizeof(bookList->books[bookList->count].title) - 1] = '\0';

              // Actualizar el conteo total de palabras y caracteres
              bookList->books[bookList->count].wordCount = wordCount;
              bookList->books[bookList->count].charCount = charCount;

              // Guardar el título y contenido en el mapa
              insertWordMap(titleContentMap, title, content);

              bookList->count++;
              fclose(file);
              printf("El archivo %s se ha cargado con éxito.\n", filename);
          } else {
              printf("No se pudo abrir el archivo %s.\n", filename);
          }
          filename = strtok(NULL, " ");
      }

      free(filenames_copy);  // Liberar la memoria de la copia mutable
}

void showDocuments(BookList* bookList) {
    // Ordenar los libros por título (si no están ordenados previamente)
    qsort(bookList->books, bookList->count, sizeof(Book), comparisonOfWords);

    printf("Documentos ordenados por título:\n");
    printf("-----------------------------------\n");
    for (int i = 0; i < bookList->count; i++) {
        printf("ID: %d\n", i);
        printf("Título: %s\n", bookList->books[i].title);
        printf("Cantidad de palabras: %d\n", bookList->books[i].wordCount);
        printf("Cantidad de caracteres: %d\n", bookList->books[i].charCount);
        printf("\n");
    }
}

void searchBooksByKeywords(Book* books, int numBooks, const char* keywords) {
    // Dividir las palabras clave en un arreglo de palabras
    char* keywordsCopy = strdup(keywords);
    char* keyword = strtok(keywordsCopy, " ");

    int titleFound[numBooks];
    for (int i = 0; i < numBooks; i++) {
        titleFound[i] = 1; // Inicialmente, todos los títulos son considerados encontrados
    }

    while (keyword != NULL) {
        // Iterar a través de los títulos y buscar la palabra clave en cada uno
        for (int i = 0; i < numBooks; i++) {
            if (strstr(books[i].title, keyword) == NULL) {
                // La palabra clave no se encuentra en este título, marcarlo como no encontrado
                titleFound[i] = 0;
            }
        }
        keyword = strtok(NULL, " ");
    }

    // Imprimir los títulos de los libros que contienen todas las palabras clave
    int found = 0;
    for (int i = 0; i < numBooks; i++) {
        if (titleFound[i]) {
            printf("La palabra ingresada se asocia al libro:%s", books[i].title);
            found = 1;
        }
    }

    if (!found) {
        printf("No se encontraron libros que coincidan con las palabras clave.\n");
    }

    free(keywordsCopy);
}

void findTopWords(const char* bookTitle, Map* titleContentMap, WordFrequency* wordFrequencies, int numWords) {
    printf("Palabras con mayor frecuencia en el libro '%s':\n", bookTitle);
    printf("-----------------------------------\n");

    // Obtener el contenido del libro a partir del título
    char* content = getValueFromMap(titleContentMap, bookTitle);

    if (content == NULL) {
        printf("Libro no encontrado.\n");
        return;
    }

    // Copiar el contenido en una nueva cadena
    char* contentCopy = strdup(content);

    // Crear un mapa para contar las palabras y su frecuencia
    Map wordCountMap = createMap(numWords);

    // Tokenizar el contenido para contar las palabras y su frecuencia
    char* token = strtok(contentCopy, " ");
    while (token != NULL) {
        // Normalizar el token a minúsculas 
        char normalizedToken[MAX_WORD_LENGTH];
        strncpy(normalizedToken, token, sizeof(normalizedToken));
        for (int i = 0; normalizedToken[i]; i++) {
            normalizedToken[i] = tolower(normalizedToken[i]);
        }

        // Incrementar el conteo de palabras en el mapa
        incrementMap(&wordCountMap, normalizedToken);

        token = strtok(NULL, " ");
    }

    // Crear un iterador para recorrer el mapa
    MapIterator iter = createMapIterator(&wordCountMap);
    char* key, * value;

    // Recorrer y guardar las palabras y frecuencias en wordFrequencies
    int i = 0;
    while (hasNextMap(&iter) && i < numWords) {
        nextMap(&iter, &key, &value);
        strncpy(wordFrequencies[i].word, key, MAX_WORD_LENGTH);
        wordFrequencies[i].frequency = atoi(value);
        i++;
    }

    // Ordenar las palabras por frecuencia en orden descendente
    qsort(wordFrequencies, numWords, sizeof(WordFrequency), comparisonByFrequency);

    // Mostrar las 10 palabras con mayor frecuencia
    for (int i = 0; i < 10; i++) {
        printf("Palabra %s: y aparece %d veces\n", wordFrequencies[i].word, wordFrequencies[i].frequency);
    }

    // Liberar la memoria utilizada por el mapa y la copia del contenido
    freeMap(&wordCountMap);
    free(contentCopy);
}


void findTopRelevantWords(const char* bookTitle, BookList* bookList, Map* titleContentMap) {
    printf("Palabras más relevantes en el libro '%s':\n", bookTitle);

    // Busca el libro correspondiente
    int bookId = findBookIdByTitle(bookList, bookTitle);

    if (bookId == -1) {
        printf("Libro no encontrado.\n");
        return;
    }

    // Obtén el contenido del libro a partir del título
    char* content = getValueFromMap(titleContentMap, bookTitle);

    if (content == NULL) {
        printf("Libro no encontrado.\n");
        return;
    }

    char* contentCopy = strdup(content);

    // Crear un mapa para contar las palabras y su frecuencia
    Map wordCountMap = createMap(MAX_WORDS_PER_BOOK);

    // Tokenizar el contenido para contar las palabras y su frecuencia
    char* token = strtok(contentCopy, " ");
    while (token != NULL) {
        // Normalizar el token a minúsculas 
        char normalizedToken[MAX_WORD_LENGTH];
        strncpy(normalizedToken, token, sizeof(normalizedToken));
        for (int i = 0; normalizedToken[i]; i++) {
            normalizedToken[i] = tolower(normalizedToken[i]);
        }

        // Incrementar el conteo de palabras en el mapa
        incrementMap(&wordCountMap, normalizedToken);

        token = strtok(NULL, " ");
    }

    // Calcular la relevancia de cada palabra y almacenarla en una estructura
    WordRelevance relevantWords[wordCountMap.size];
    int wordCount = wordCountMap.size;

    for (int i = 0; i < wordCount; i++) {
        const char* word = wordCountMap.keys[i];

        int count = atoi(wordCountMap.values[i]);

        int numDocumentsWithWord = countDocumentsWithWord(&wordCountMap, bookList, word);

        printf("Si = %d\n", numDocumentsWithWord);

        // Calcula la relevancia utilizando la función calculateRelevance
        double relevance = calculateRelevance(count, bookList->books[bookId].wordCount, bookList->count, numDocumentsWithWord);

        strncpy(relevantWords[i].word, word, MAX_WORD_LENGTH);
        relevantWords[i].relevance = relevance;
    }

    // Ordenar las palabras por relevancia en orden descendente
    qsort(relevantWords, wordCount, sizeof(WordRelevance), comparisonByRelevance);

    // Mostrar las 10 palabras más relevantes
    for (int i = 0; i < wordCount && i < 10; i++) {
        printf("%s: %.6f de relevancia\n", relevantWords[i].word, relevantWords[i].relevance);
    }

    // Liberar la memoria utilizada por el mapa
    freeMap(&wordCountMap);
}

void searchWords(const char* searchWord, BookList* bookList, Map* titleContentMap) {
    printf("Búsqueda de la palabra: %s\n", searchWord);
    printf("-----------------------------------\n");

    // Crea una copia mutable del mapa titleContentMap
    Map copyTitleContentMap = copyMap(titleContentMap);

    // Obtén el número total de documentos (libros) y el número de documentos que contienen la palabra buscada
    int totalDocuments = bookList->count;
    int documentsWithWord = 0;

    // Recorre la lista de libros para contar cuántos contienen la palabra buscada
    for (int i = 0; i < totalDocuments; i++) {
        if (countWordInContent(bookList->books[i].content, searchWord)) {
            documentsWithWord++;
        }
    }

    // Definir una estructura para almacenar información de libros relevantes
    struct RelevantBook relevantBooks[MAX_MAP_ENTRIES];
    int relevantBookCount = 0;

    // Calcular la relevancia para cada libro
    for (int i = 0; i < totalDocuments; i++) {
        printf("Procesando libro: %s\n", bookList->books[i].title);

        const char* content = bookList->books[i].content;
        int wordCountInDocument = countWordInContent(content, searchWord);
        int totalWordsInDocument = bookList->books[i].wordCount;

        // Calcular la relevancia utilizando la fórmula tf-idf
        double relevance = calculateRelevance(wordCountInDocument, totalWordsInDocument, totalDocuments, documentsWithWord);

        // Almacenar la información del libro y su relevancia
        snprintf(relevantBooks[relevantBookCount].title, sizeof(relevantBooks[relevantBookCount].title), "%s", bookList->books[i].title);
        relevantBooks[relevantBookCount].relevance = relevance;
        relevantBookCount++;
    }

    // Ordenar los libros por relevancia en orden descendente
    qsort(relevantBooks, relevantBookCount, sizeof(struct RelevantBook), comparisonByRelevance);

    // Mostrar los libros ordenados por relevancia
    printf("Libros ordenados por relevancia para la palabra '%s':\n", searchWord);
    printf("-----------------------------------\n");
    for (int i = 0; i < relevantBookCount; i++) {
        printf("ID: %d\n", i);
        printf("Título: %s\n", relevantBooks[i].title);
        printf("-----------------------------------\n");
    }

    // Liberar la memoria utilizada por la copia del mapa
    freeMap(&copyTitleContentMap);
}

void searchWordInBooks(const BookList* bookList, const Map* titleContentMap) {
    char bookTitle[MAX_TITLE_LENGTH];
    char searchWord[MAX_WORD_LENGTH];

    printf("Ingrese el título del libro en el que desea buscar: ");
    scanf("%s", bookTitle);

    int bookId = findBookIdByTitle(bookList, bookTitle);

    if (bookId == -1) {
        printf("Libro no encontrado.\n");
        return;
    }

    printf("Ingrese la palabra que desea buscar: ");
    scanf("%s", searchWord);

    char* bookContent = getValueFromMap(titleContentMap, bookTitle);

    if (bookContent == NULL) {
        printf("Contenido del libro no encontrado.\n");
        return;
    }

    printf("Contexto de la palabra '%s' en el libro '%s':\n", searchWord, bookTitle);

    char* contentCopy = strdup(bookContent);
    char* contextStart = contentCopy;

    // Tokenizar el contenido para buscar la palabra y mostrar el contexto
    char* token = strtok(contentCopy, " ");
    int wordsBeforeAndAfter = 5; 

    while (token != NULL) {
        if (strcmp(token, searchWord) == 0) {
            printf("Contexto: ");

            int wordsShown = 0;

            // Mostrar palabras antes de la palabra buscada
            while (wordsShown < wordsBeforeAndAfter && contextStart != token) {
                printf("%s ", contextStart);
                contextStart = strtok(NULL, " ");
                wordsShown++;
            }

            // Mostrar la palabra buscada en negrita
            printf("<b>%s</b> ", token);

            // Mostrar palabras después de la palabra buscada
            wordsShown = 0;
            contextStart = strtok(NULL, " ");
            while (wordsShown < wordsBeforeAndAfter && contextStart != NULL) {
                printf("%s ", contextStart);
                contextStart = strtok(NULL, " ");
                wordsShown++;
            }

            printf("\n");
        }

        token = strtok(NULL, " ");
    }
}

int main() {
    TrieNode* root = createNode();
    int choice, bookId;
    char filenames[1000];
    char bookTitle[MAX_TITLE_LENGTH];
    char searchWord[MAX_WORD_LENGTH];
    char keywords[100];
    BookList bookList;
    bookList.count = 0;
    bookList.capacity = 100;
    bookList.books = (Book*)malloc(100 * sizeof(Book));
    Map titleContentMap = createMap(100);
    WordFrequency wordFrequencys[10];

    do {
        printf("-----------------------------------\n");
        printf("Menu:\n");
        printf("1.- Cargar documentos.\n");
        printf("2.- Mostrar documentos ordenados.\n");
        printf("3.- Buscar un libro por título.\n");
        printf("4.- Palabras con mayor frecuencia.\n");
        printf("5.- Palabras más relevantes.\n");
        printf("6.- Buscar por palabra.\n");
        printf("7.- Mostrar palabra en su contexto dentro del libro.\n");
        printf("8.- Salir.\n");
        printf("-----------------------------------\n");
        printf("Seleccione una opción: ");
        scanf("%d", &choice);
        printf("-----------------------------------\n");

        switch (choice) {
            case 1:
              printf("Ingrese los nombres de los archivos TXT separados por espacio: ");
              scanf(" %[^\n]s", filenames);
              loadDocuments(root, filenames, &bookList, &titleContentMap);
              break;
            case 2:
              if (bookList.count > 0) {
                showDocuments(&bookList);
              } else {
                printf("No se han cargado libros todavía.\n");
              }
              break;
            case 3:
              printf("Ingrese las palabras clave separadas por espacio: ");
              scanf(" %[^\n]s", keywords);
              searchBooksByKeywords(bookList.books, bookList.count, keywords);
              break;
            case 4:
                printf("Ingrese el ID del libro para encontrar las palabras con mayor frecuencia: ");
                scanf("%d", &bookId);

                if (bookId >= 0 && bookId < bookList.count) {
                    findTopWords(bookList.books[bookId].title, &titleContentMap, bookList.books[bookId].wordFrequencies, bookList.books[bookId].wordCount);
                } else {
                    printf("ID de libro no válido.\n");
                }
                break;
            case 5:
                printf("Ingrese el título del libro para encontrar las palabras más relevantes: ");
                scanf(" %[^\n]", bookTitle);

                findTopRelevantWords(bookTitle, &bookList, &titleContentMap);
                break;
            case 6:
              printf("Ingrese la palabra para encontrar libros ordenados por relevancia: ");
              char words[MAX_WORD_LENGTH];
              scanf("%s", words);

              searchWords(words, &bookList, &titleContentMap);
              break;
            case 7:
              searchWordInBooks(&bookList, &titleContentMap);
              break;
            case 8:
              // Liberar la memoria utilizada por el mapa
              freeMap(&titleContentMap);
              free(bookList.books);
              break;
            default:
                printf("Opción no válida. Intente de nuevo.\n");
        }
    } while (choice != 8);

    return 0;
}
