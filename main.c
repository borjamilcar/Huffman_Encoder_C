#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_TREE_HT 100
#define FILENAME "DadosGPS26_07_2018_1.txt"
int MAX_LEN;

/**/
struct MinHeapNode{
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

/**/
struct MinHeap{
    unsigned size;
    unsigned capacity;
    struct MinHeapNode **array;
};

/**/
struct MinHeapNode* newNode(char data, unsigned freq){

    struct MinHeapNode* temp =(struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));

    temp->data = data;
    temp->freq = freq;
    temp->left = temp->right = NULL;

    return temp;
}

/**/
struct MinHeap* createMinHeap(unsigned capacity){

    struct MinHeap* minHeap =(struct MinHeap*) malloc(sizeof(struct MinHeap));

    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =(struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));

    return minHeap;
}

/**/
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b){

    struct MinHeapNode* t = *a;

    *a = *b;
    *b = t;
}

/**/
void minHeapify(struct MinHeap* minHeap, unsigned idx){

    unsigned smallest = idx;
    unsigned left = 2 * idx + 1;
    unsigned right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
      smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
      smallest = right;

    if (smallest != idx){
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

/**/
bool isSizeOne(struct MinHeap* minHeap){
    return (minHeap->size == 1);
}

/**/
struct MinHeapNode* extractMin(struct MinHeap* minHeap){

    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);

    return temp;
}

/**/
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode){

    ++minHeap->size;
    unsigned i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1)/2]->freq){
        minHeap->array[i] = minHeap->array[(i - 1)/2];
        i = (i - 1)/2;
    }

    minHeap->array[i] = minHeapNode;
}

/**/
void buildMinHeap(struct MinHeap* minHeap){

    unsigned n = minHeap->size - 1;

    for (int i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

/**/
void printArr(unsigned arr[], unsigned n){

    for (unsigned i = 0; i < n; ++i){
        printf("%d", arr[i]);
    }

    printf("\n");
}

/**/
bool isLeaf(struct MinHeapNode* root){
    return !(root->left) && !(root->right) ;
}

/**/
struct MinHeap* createAndBuildMinHeap(char data[], unsigned freq[], unsigned size){

    struct MinHeap* minHeap = createMinHeap(size);

    for (unsigned i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);

    minHeap->size = size;

    buildMinHeap(minHeap);

    return minHeap;
}

/**/
struct MinHeapNode* buildHuffmanTree(char data[], unsigned freq[], unsigned size){

    struct MinHeapNode *left, *right, *top;

    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);

    while (!isSizeOne(minHeap)){

        left = extractMin(minHeap);
        right = extractMin(minHeap);

        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }

    return extractMin(minHeap);
}

/**/
void printCodes(struct MinHeapNode* root, unsigned arr[], unsigned top){

    if (root->left){
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }

    if (root->right){
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }

    if (isLeaf(root)){
        printf("%c: ", root->data);
        printArr(arr, top);
    }
}

/**/
void huffmanEncoder(char data[], unsigned freq[], unsigned size){

   struct MinHeapNode* root = buildHuffmanTree(data, freq, size);

   unsigned arr[MAX_TREE_HT], top = 0;

   printCodes(root, arr, top);
}

/**/
bool isEmpty(char *str) {
    return str == NULL;
}

/**/
bool isDataAlreadySet(char c, char arr[]) {

    unsigned i = 0;

    while (arr[i] != '\0') {
        if (c == arr[i++])
            return true;
    }

    return false;
}

/**/
char *readFile(char *file_name) {
    FILE *file_ptr;
    char c;
    char *str_ptr;
    size_t index = 0;

    if ((file_ptr = fopen(file_name, "r")) == NULL)
        return NULL;

    fseek(file_ptr, 0, SEEK_END);
    long file_size = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);
    MAX_LEN = file_size - 1;

    str_ptr = malloc(file_size);

    while ((c = fgetc(file_ptr)) != EOF)
        str_ptr[index++] = c;

    str_ptr[index] = '\0';

    return str_ptr;
}

/**/
unsigned calcDaraFreq(char c, char *fileContent) {

    unsigned cont = 0;

    for (unsigned i = 0; i < strlen(fileContent) + 1; i++) {
        if (c == fileContent[i])
            cont++;
    }

    return cont;
}

/**/
 void createArrayDataFreq(char *fileContent) {
    char dataArr[MAX_LEN];
    unsigned freqArr[MAX_LEN];
    unsigned index = 0;

    for (int i = 0; i < strlen(fileContent) + 1; i++) {
        if (i == 0) {
            dataArr[index] = fileContent[i];
            freqArr[index++] = calcDaraFreq(fileContent[i], fileContent);
        } else {
            if (!isDataAlreadySet(fileContent[i], dataArr)) {
                dataArr[index] = fileContent[i];
                freqArr[index++] = calcDaraFreq(fileContent[i], fileContent);
            }
        }
    }

    huffmanEncoder(dataArr, freqArr, index);
 }

/**/
int main(){

    char *fileContent = readFile(FILENAME);

    if (isEmpty(fileContent)) {
        printf("Ficheiro nao encontrado!\n");
    } else {

        for (unsigned i = 0; i < strlen(fileContent); i++){
            printf("%c", fileContent[i]);
        }
        createArrayDataFreq(fileContent);
    }

    return 0;
}
