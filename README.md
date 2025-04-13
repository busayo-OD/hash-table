# Hash Table in C

A simple hash table implementation using linear probing for collision resolution and dynamic resizing based on load factor. The PJW hash function is used for generating keys.

## Features

- Insert and retrieve key-value pairs (strings)
- Collision resolution via linear probing
- Dynamic resizing when load factor exceeds 0.31

## Compile and Run

```bash
gcc main.c hash_table.c -o hash_table
./hash_table
