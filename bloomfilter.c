#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define SIZE 100 // Size of the Bloom filter bit array
#define HASH_PRIME_1 31 // Prime number for hash function 1
#define HASH_PRIME_2 37 // Prime number for hash function 2

typedef struct {
    uint8_t* array; // Bit array to hold the presence or absence of elements
    size_t array_size; // Size of the bit array in bytes
} BloomFilter;

// Function prototypes
int hash1(char* str);
int hash2(char* str);
void add(BloomFilter* filter, char* str);
bool contains(BloomFilter* filter, char* str);

// Main function
int main() {
    // Initialize Bloom filter
    BloomFilter filter;
    size_t bit_array_size = (SIZE + 7) / 8; // Calculate the size of the bit array in bytes
    filter.array = (uint8_t*)calloc(bit_array_size, sizeof(uint8_t)); // Allocate memory for the bit array
    filter.array_size = bit_array_size;

    // Test the Bloom filter
    add(&filter, "hello");
    add(&filter, "world");

    printf("Contains 'hello': %d\n", contains(&filter, "hello"));
    printf("Contains 'world': %d\n", contains(&filter, "world"));
    printf("Contains 'ok': %d\n", contains(&filter, "ok"));

    // Free memory
    free(filter.array);

    return 0;
}

// Hash function 1
int hash1(char* str) {
    unsigned long hash = 0;
    int c;

    while ((c = *str++))
        hash = (hash * HASH_PRIME_1) + c;

    return (int)(hash % SIZE);
}

// Hash function 2
int hash2(char* str) {
    unsigned long hash = 0;
    int c;

    while ((c = *str++))
        hash = (hash * HASH_PRIME_2) + c;

    return (int)(hash % SIZE);
}

void set_bit(uint8_t* array, size_t index) {
    size_t byte_index = index / 8;
    size_t bit_index = index % 8;
    
    uint8_t bitmask = 1 << bit_index;
    
    array[byte_index] |= bitmask;
}

void add(BloomFilter* filter, char* str) {
    int index1 = hash1(str);
    int index2 = hash2(str);

    set_bit(filter->array, index1);
    set_bit(filter->array, index2);
}

// Check if an element is likely to be in the Bloom filter
bool contains(BloomFilter* filter, char* str) {
    int index1 = hash1(str);
    int index2 = hash2(str);

    // Check if corresponding bits are set for hash functions 1, 2, and 3
    return (filter->array[index1 / 8] & (1 << (index1 % 8))) &&
           (filter->array[index2 / 8] & (1 << (index2 % 8)));
}