#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 100

struct DNASequence {
    char *sequence;
};

struct MinHeapNode {
    char data;
    unsigned freq;
    struct MinHeapNode *left, *right;
};

struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct MinHeapNode **array;
};

struct DNASequence *createDNASequence(char *sequence) {
    struct DNASequence *newSequence = (struct DNASequence *)malloc(sizeof(struct DNASequence));
    newSequence->sequence = strdup(sequence);
    return newSequence;
}

void destroyDNASequence(struct DNASequence *sequence) {
    free(sequence->sequence);
    free(sequence);
}

struct MinHeapNode *newNode(char data, unsigned freq) {
    struct MinHeapNode *temp = (struct MinHeapNode *)malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}

struct MinHeap *createMinHeap(unsigned capacity) {
    struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (struct MinHeapNode **)malloc(minHeap->capacity * sizeof(struct MinHeapNode *));
    return minHeap;
}

void swapMinHeapNode(struct MinHeapNode **a, struct MinHeapNode **b) {
    struct MinHeapNode *t = *a;
    *a = *b;
    *b = t;
}

void minHeapify(struct MinHeap *minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

int isSizeOne(struct MinHeap *minHeap) {
    return (minHeap->size == 1);
}

struct MinHeapNode *extractMin(struct MinHeap *minHeap) {
    struct MinHeapNode *temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

void insertMinHeap(struct MinHeap *minHeap, struct MinHeapNode *minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    minHeap->array[i] = minHeapNode;
}

void buildMinHeap(struct MinHeap *minHeap) {
    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

void printArr(int arr[], int n) {
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", arr[i]);
    printf("\n");
}

int isLeaf(struct MinHeapNode *root) {
    return !(root->left) && !(root->right);
}

struct MinHeap *createAndBuildMinHeap(struct DNASequence *sequences[], int size) {
    struct MinHeap *minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i) {
        minHeap->array[i] = newNode(' ', 0);
        minHeap->array[i]->freq = strlen(sequences[i]->sequence);
    }
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}

struct MinHeapNode *buildHuffmanTree(int freq[]) {
    struct MinHeapNode *left, *right, *top;
    struct MinHeap *minHeap = createMinHeap(256);

    for (int i = 0; i < 256; ++i) {
        if (freq[i] != 0) {
            minHeap->array[minHeap->size++] = newNode((char)i, freq[i]);
        }
    }

    buildMinHeap(minHeap);

    while (!isSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }

    return extractMin(minHeap);
}

void printCodesUtil(struct MinHeapNode *root, int arr[], int top, int alphabetPrinted[]) {
    if (root->left) {
        arr[top] = 0;
        printCodesUtil(root->left, arr, top + 1, alphabetPrinted);
    }

    if (root->right) {
        arr[top] = 1;
        printCodesUtil(root->right, arr, top + 1, alphabetPrinted);
    }

    if (isLeaf(root)) {
        printf("%c: ", root->data);
        printArr(arr, top);
        alphabetPrinted[root->data] = 1;
    }
}

void printCodes(struct MinHeapNode *root, int arr[], int top) {
    int alphabetPrinted[256] = {0};
    printCodesUtil(root, arr, top, alphabetPrinted);
}

void addDNASequence(struct DNASequence *sequences[], int *size) {
    char input[100];
    printf("Enter the new DNA sequence data (A, T, G, C): ");
    scanf("%s", input);

    if (*size < 100) {
        sequences[*size] = createDNASequence(input);
        (*size)++;
        printf("DNA sequence added successfully.\n");
    } else {
        printf("Maximum number of DNA sequences reached.\n");
    }
}

void viewDNASequences(struct DNASequence *sequences[], int size) {
    printf("List of DNA Sequences:\n");
    for (int i = 0; i < size; ++i) {
        printf("%d. %s\n", i + 1, sequences[i]->sequence);
    }
}

void compressDNASequences(struct DNASequence *sequences[], int size) {
    if (size == 0) {
        printf("No DNA sequences to compress.\n");
        return;
    }

    printf("List of DNA Sequences:\n");
    for (int i = 0; i < size; ++i) {
        printf("%d. %s\n", i + 1, sequences[i]->sequence);
    }

    int selectedSequence;
    printf("Enter the index of the DNA sequence to compress: ");
    scanf("%d", &selectedSequence);

    if (selectedSequence >= 1 && selectedSequence <= size) {
        int freq[256] = {0};

        char *sequence = sequences[selectedSequence - 1]->sequence;
        while (*sequence != '\0') {
            freq[(int)(*sequence)]++;
            sequence++;
        }

        struct MinHeapNode *root = buildHuffmanTree(freq);

        int arr[MAX_TREE_HT], top = 0;
        printf("Huffman Codes for the selected DNA Sequence:\n");
        printCodes(root, arr, top);
    } else {
        printf("Invalid index or no DNA sequences to compress.\n");
    }
}

void deleteDNASequence(struct DNASequence *sequences[], int *size) {
    int index;
    printf("Enter the index of the DNA sequence to delete: ");
    scanf("%d", &index);

    if (index >= 1 && index <= *size) {
        destroyDNASequence(sequences[index - 1]);
        for (int i = index - 1; i < *size - 1; ++i) {
            sequences[i] = sequences[i + 1];
        }
        (*size)--;
        printf("DNA sequence deleted successfully.\n");
    } else {
        printf("Invalid index or no DNA sequences to delete.\n");
    }
}

void handleMenu(struct DNASequence *sequences[], int *size) {
    int choice;
    do {
        printf("\n--- DNA Sequence Menu ---\n");
        printf("1. Add DNA Sequence\n");
        printf("2. View DNA Sequences\n");
        printf("3. Compress DNA Sequences\n");
        printf("4. Delete DNA Sequence\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            addDNASequence(sequences, size);
            break;
        case 2:
            viewDNASequences(sequences, *size);
            break;
        case 3:
            compressDNASequences(sequences, *size);
            break;
        case 4:
            deleteDNASequence(sequences, size);
            break;
        case 5:
            printf("Exiting the program.\n");
            break;
        default:
            printf("Invalid choice. Please enter a valid option.\n");
        }
    } while (choice != 5);
}

int main() {
    struct DNASequence *sequences[100];
    int size = 0;

    handleMenu(sequences, &size);

    for (int i = 0; i < size; ++i) {
        destroyDNASequence(sequences[i]);
    }

    return 0;
}
