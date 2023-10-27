#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

TrieNode* createNode() {
    TrieNode* newNode = (TrieNode*)malloc(sizeof(TrieNode));
    if (newNode) {
        newNode->isEndOfWord = 0;
        for (int i = 0; i < 62; i++) {
            newNode->children[i] = NULL;
        }
    }
    return newNode;
}

void insertWord(TrieNode* root, const char* word) {
    TrieNode* currentNode = root;
    int len = strlen(word);

    for (int i = 0; i < len; i++) {
        int index;

        // Verifica si el carácter es una letra minúscula del alfabeto inglés
        if (isalpha(word[i]) && islower(word[i])) {
            index = word[i] - 'a';
        } else {
            continue;
        }

        if (!currentNode->children[index]) {
            currentNode->children[index] = createNode();
        }
        currentNode = currentNode->children[index];
    }
    currentNode->isEndOfWord = 1;
}

int searchWord(TrieNode* root, const char* word) {
    TrieNode* currentNode = root;
    int len = strlen(word);
    for (int i = 0; i < len; i++) {
        int index = word[i] - 'a';
        if (!currentNode->children[index]) {
            return 0;
        }
        currentNode = currentNode->children[index];
    }
    return (currentNode != NULL && currentNode->isEndOfWord);
}

int comparisonOfWords(const void* a, const void* b) {
    const Book* bookA = (const Book*)a;
    const Book* bookB = (const Book*)b;
    return strcmp(bookA->title, bookB->title);
}

Map createMap(int initialCapacity) {
    Map map;
    map.keys = (char**)malloc(initialCapacity * sizeof(char*));
    map.values = (char**)malloc(initialCapacity * sizeof(char*));
    map.size = 0;
    map.capacity = initialCapacity;
    return map;
}

void insertWordMap(Map* map, const char* key, const char* value) {
    if (map->size >= map->capacity) {
        // Realocar memoria si se alcanza la capacidad máxima
        map->capacity *= 2;
        map->keys = (char**)realloc(map->keys, map->capacity * sizeof(char*)); 
        map->values = (char**)realloc(map->values, map->capacity * sizeof(char*));
    }

    map->keys[map->size] = strdup(key);
    map->values[map->size] = strdup(value);
    map->size++;
}

char* getValueFromMap(const Map* map, const char* key) {
    for (int i = 0; i < map->size; i++) {
        if (strcmp(map->keys[i], key) == 0) {
            return map->values[i];
        }
    }
    return NULL; // La clave no se encontró
}

void freeMap(Map* map) {
    for (int i = 0; i < map->size; i++) {
        free(map->keys[i]);
        free(map->values[i]);
    }
    free(map->keys);
    free(map->values);
}

void incrementMap(Map* map, const char* key) {
    for (int i = 0; i < map->size; i++) {
        if (strcmp(map->keys[i], key) == 0) {
            // La palabra ya existe en el mapa, incrementar su frecuencia
            int frequency = atoi(map->values[i]);
            frequency++;
            char frequencyStr[10];
            sprintf(frequencyStr, "%d", frequency);
            strncpy(map->values[i], frequencyStr, 10);
            return;
        }
    }

    // La palabra no existe en el mapa, agregarla con una frecuencia inicial de 1
    if (map->size < map->capacity) {
        map->keys[map->size] = strdup(key);
        map->values[map->size] = strdup("1"); // Frecuencia inicial
        map->size++;
    }
    // Si el mapa está lleno, puedes implementar una lógica para redimensionarlo.
}

// Función para limpiar el contenido y retener solo palabras válidas
void cleanContent(char* content) {
    char buffer[1000];  // Un buffer temporal para construir palabras
    char cleanedContent[1000] = "";  // Inicializar cleanedContent como una cadena vacía

    for (int i = 0; content[i] != '\0'; i++) {
        if (isalpha(content[i]) || isspace(content[i])) {
            strncat(cleanedContent, &content[i], 1);
        }
    }

    strcpy(content, cleanedContent);
}

// Función para crear un iterador para el mapa
MapIterator createMapIterator(Map* map) {
    MapIterator iterator;
    iterator.map = map;
    iterator.currentIndex = 0;
    return iterator;
}

// Función para verificar si hay más elementos en el mapa para iterar
int hasNextMap(MapIterator* iterator) {
    return iterator->currentIndex < iterator->map->size;
}

// Función para avanzar al siguiente elemento en el mapa y obtener su clave y valor
void nextMap(MapIterator* iterator, char** key, char** value) {
    if (hasNextMap(iterator)) {
        *key = iterator->map->keys[iterator->currentIndex];
        *value = iterator->map->values[iterator->currentIndex];
        iterator->currentIndex++;
    } else {
        *key = NULL;
        *value = NULL;
    }
}

int comparisonByFrequency(const void* a, const void* b) {
    const WordFrequency* wordA = (const WordFrequency*)a;
    const WordFrequency* wordB = (const WordFrequency*)b;

    // Compara por frecuencia en orden descendente
    return wordB->frequency - wordA->frequency;
}

int comparisonByRelevance(const void* a, const void* b) {
    const WordRelevance* wordA = (const WordRelevance*)a;
    const WordRelevance* wordB = (const WordRelevance*)b;
    if (wordA->relevance < wordB->relevance) return 1;
    if (wordA->relevance > wordB->relevance) return -1;
    return 0;
}

double calculateRelevance(int wordCountInDocument, int totalWordsInDocument, int totalDocuments, int documentsWithWord) {
    // Calcula la relevancia utilizando la fórmula tf-idf
    double tf = (double)wordCountInDocument / totalWordsInDocument;
    double idf = log((double)totalDocuments / documentsWithWord);

    // Reemplaza "wordCountInDocument" con la frecuencia de "word" en el documento actual
    // Reemplaza "totalWordsInDocument" con el total de palabras en el documento actual
    // Reemplaza "totalDocuments" con el número total de documentos
    // Reemplaza "documentsWithWord" con el número de documentos que contienen "word"

    return tf * idf;
}

int countWordInContent(const char* content, const char* searchWord) {
    int count = 0;
    char* token = strtok(content, " ");

    while (token != NULL) {
        // Normaliza el token a minúsculas antes de comparar
        char normalizedToken[MAX_WORD_LENGTH];
        strncpy(normalizedToken, token, sizeof(normalizedToken));
        for (int i = 0; normalizedToken[i]; i++) {
            normalizedToken[i] = tolower(normalizedToken[i]);
        }

        // Compara el token normalizado con la palabra buscada
        if (strcmp(normalizedToken, searchWord) == 0) {
            count++;
        }

        token = strtok(NULL, " ");
    }

    return count;
}


Map copyMap(const Map* originalMap) {
    Map copiedMap;

    // Inicializa la copia con la misma capacidad que el mapa original
    copiedMap.capacity = originalMap->capacity;
    copiedMap.size = originalMap->size;

    // Asigna memoria para las claves y valores de la copia
    copiedMap.keys = (char**)malloc(copiedMap.capacity * sizeof(char*));
    copiedMap.values = (char**)malloc(copiedMap.capacity * sizeof(char*));

    // Copia las claves y valores del mapa original a la copia
    for (int i = 0; i < originalMap->size; i++) {
        copiedMap.keys[i] = strdup(originalMap->keys[i]);
        copiedMap.values[i] = strdup(originalMap->values[i]);
    }

    return copiedMap;
}

int findBookIdByTitle(const BookList* bookList, const char* title) {
    for (int i = 0; i < bookList->count; i++) {
        if (strcmp(bookList->books[i].title, title) == 0) {
            return i;  // El título coincide, devuelve el ID del libro
        }
    }
    return -1;  // Si no se encuentra el libro, devuelve -1
}

int countDocumentsWithWord(const Map* wordCountMap, const BookList* bookList, const char* word) {
    int numBooks = bookList->count;

    int count = 0;

    for (int i = 0; i < numBooks; i++) {
        if (countWordInContent(bookList->books[i].content, word)) {
            count++;
        }
    }
    return count;
}



