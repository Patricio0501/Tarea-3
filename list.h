#ifndef LIST_H
#define LIST_H

#define MAX_WORDS_PER_BOOK 1000
#define MAX_WORD_LENGTH 50
#define MAX_MAP_ENTRIES 100
#define MAX_TITLE_LENGTH 100


// Estructura para representar una palabra y su frecuencia en un libro
typedef struct {
    char word[100];
    int frequency;
    float relativeFrequency;
} WordFrequency;

// Estructura de un nodo del árbol trie
typedef struct TrieNode {
    struct TrieNode* children[62];
    int isEndOfWord;
} TrieNode;

typedef struct Book {
    char title[100];
    char content[1000];  
    WordFrequency wordFrequencies[MAX_WORDS_PER_BOOK];
    int wordCount;
    int charCount;
} Book;

typedef struct BookList {
    Book* books; 
    int count;
    int capacity;
} BookList;

typedef struct {
    char** keys; 
    char** values; 
    int size;
    int capacity;
} Map;

typedef struct {
    Map* map;
    int currentIndex;
} MapIterator;

// Definición de la estructura para almacenar las palabras y su relevancia.
typedef struct {
    char word[MAX_WORD_LENGTH];
    double relevance;
} WordRelevance;

// Estructura para almacenar información de libros relevantes
struct RelevantBook {
    char id[10];
    char title[100];
    double relevance;
};

TrieNode* createNode();

void insertWord(TrieNode* root, const char* word);

int searchWord(TrieNode* root, const char* word);

int comparacionDePalabras(const void* a, const void* b);

Map createMap(int initialCapacity);

void insertWordMap(Map* map, const char* key, const char* value);

char* getValueFromMap(const Map* map, const char* key);

void freeMap(Map* map);

void incrementMap(Map* map, const char* key);

void cleanContent(char* content);

MapIterator createMapIterator(Map* map);

int hasNextMap(MapIterator* iterator);

void nextMap(MapIterator* iterator, char** key, char** value);

int comparisonByFrequency(const void* a, const void* b);

int comparisonByRelevance(const void* a, const void* b);

double calculateRelevance(int wordCountInDocument, int totalWordsInDocument, int totalDocuments, int documentsWithWord);

int countWordInContent(const char* word, const char* content);

Map copyMap(const Map* originalMap);

int findBookIdByTitle(const BookList* bookList, const char* title);

int countDocumentsWithWord(const Map* wordCountMap, const BookList* bookList, const char* word);

#endif /* LIST_H */
