#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define HT_PRIME_1 (151)
#define HT_PRIME_2 (163)
//#define HT_INITIAL_BASE_SIZE (53)

#define HT_INITIAL_BASE_SIZE (9)
int last_index = 0;

typedef struct {
    unsigned int key;
    char *value;
} ht_item;

// Our hash table stores an array of pointers to items, 
// and some details about its size and how full it is:

typedef struct {
    int size;
    int count;
    ht_item **items;
} ht_hash_table;


ht_item HT_DELETED_ITEM = {-1, "-1"};

void delete_a_record(ht_hash_table *ht, char* value);
ht_hash_table *add_new_record(ht_hash_table *ht, char* value);
ht_hash_table *ht_insert(ht_hash_table *ht, char* value);
ht_item *ht_search(ht_hash_table *ht, char* value);
void ht_delete(ht_hash_table *ht, char* value);
int is_prime(const int x);
int next_prime(int x);
ht_item* ht_new_item(unsigned int k, char* v);
ht_hash_table *ht_new(void);
void ht_del_item(ht_item* i);
void ht_del_hash_table(ht_hash_table *mht);
int ht_hash(char* s, const int a, int m);
int ht_get_hash(char* s, int num_buckets, int attempt);
ht_hash_table* ht_new_sized(int base_size);
ht_hash_table *ht_resize(ht_hash_table *ht);
//void ht_resize_up(ht_hash_table* ht);
//void ht_resize_down(ht_hash_table* ht);
void ht_list_entries(ht_hash_table *ht);
//char* udf_strdup(char* input);
ht_hash_table* ht_new_fixed(int size);
unsigned int PJW_hash(char *word);

int main() 
{
    ht_hash_table *ht = ht_new();
    
    ht = add_new_record(ht, "undulate");  
    ht = add_new_record(ht, "Similar");
    ht = add_new_record(ht, "processors");
    ht = add_new_record(ht, "universality");
    ht = add_new_record(ht, "appropriately");	
    ht = add_new_record(ht, "compilation"); 
    //printf(" ht->count: %d\n",  ht->count); 
    ht = add_new_record(ht, "finished");
    ht = add_new_record(ht, "successfully");
    ht = add_new_record(ht, "directory");
    ht = add_new_record(ht, "appropriately");
    ht = add_new_record(ht, "in");
    ht = add_new_record(ht, "going");
    ht = add_new_record(ht, "and");
    ht = add_new_record(ht, "addition");
    //printf(" ht->count: %d\n",  ht->count); 
    ht_item *target = ht_search(ht, "processors");
    if (target != NULL)
    {
	printf("target->value, target->key: %s, %d\n", target->value, target->key);
    }
    target = ht_search(ht, "directory");
    if (target != NULL)
    {
	printf("target->value, target->key: %s, %d\n", target->value, target->key);
    }	
    delete_a_record(ht, "in");
    //printf(" ht->count: %d\n",  ht->count); 
	
    target = ht_search(ht, "in");
    if (target != NULL)
    {
	printf("target->value, target->key: %s, %d\n", target->value, target->key);
    }
    else
    {
	printf("target not found\n");
    }
    ht = add_new_record(ht, "example");
    ht = add_new_record(ht, "nap");
    ht = add_new_record(ht, "dynamic");
    ht = add_new_record(ht, "language");
    ht = add_new_record(ht, "dmanages");
    ht = add_new_record(ht, "space");
    delete_a_record(ht, "nap");
    ht = add_new_record(ht, "safety ");
    ht = add_new_record(ht, "example");
    ht = add_new_record(ht, "example");
    ht = add_new_record(ht, "starting");
    ht = add_new_record(ht, "implementations");
    ht_list_entries(ht);
    ht_del_hash_table(ht);	
    printf("End\n");
    //ht_del_hash_table(ht);	
}

ht_item* ht_new_item(unsigned int k, char* v)
{
    ht_item* i = malloc(sizeof(ht_item));
    i->key = k;		//strdup(k);
    i->value = v;		//strdup(v);
    return i;
}

/*
char* udf_strdup(char* input)
{
    // We need strlen(src) + 1, since we have to account for '\0'
    int len = strlen(input) + 1;
    char* output = (char*) malloc ((len + 1) * sizeof(char));
    if (output == NULL) return NULL;
    output = (char*) memcpy(output, input, len);
    return output;
}
*/

ht_hash_table *ht_new(void)
{
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));
    ht->size = 13;		//53;
    ht->count = 0;		
    ht->items = calloc((int)ht->size, sizeof(ht_item*));
    for (int i = 0; i < ht->size; i++) 
	{ 
        ht->items[i] = &HT_DELETED_ITEM;
    }
    return ht;
}

void ht_del_item(ht_item* i) 
{
    //free(i->key);
    //free(i->value);
    free(i);
}

void ht_del_hash_table(ht_hash_table *mht)
{
	//printf("ht_del_hash_table => mht->size: %d\n",  mht->size); 
	for (int i = 0; i < mht->size; i++) 
	{ 
        ht_item* item = mht->items[i];
        
        if (item != &HT_DELETED_ITEM)
        {
            ht_del_item(item);
        }
    }
    free(mht->items);
    free(mht);
    mht = NULL;
    //printf("ht_del_hash_table => (item != NULL), i: %d, %d\n",  i, (item != NULL)); 
}

void ht_list_entries(ht_hash_table* ht)
{
	printf("\n");
	printf("ht_list_entries => ht->size: %d\n", ht->size);
	for (int i = 0; i < ht->size; i++) 
	{ 
        ht_item* item = ht->items[i];
        if (item != &HT_DELETED_ITEM)
        {
            //ht_del_item(item);
            printf("%d, %s, %d\n",  i, item->value, item->key); 
        }
    }
}

int ht_hash(char* s, int a, int m)
{
    long hash = 0;
    const int len_s = strlen(s);
    for (int i = 0; i < len_s; i++) {
        hash += (long)pow(a, len_s - (i+1)) * s[i];
        hash = hash % m;
    }
    return (int)hash;
}

unsigned int PJW_hash(char *word)
{
    // Similar to the PJW Hash function, but tweaked for 32-bit processors.
    // It is a widley used hash function on UNIX based systems.
    unsigned int str_len = strlen(word);
    unsigned int hash = 0;
    unsigned int x    = 0;
    unsigned int i    = 0;

    for (i = 0; i < str_len; ++word, ++i)
    {
        hash = (hash << 4) + (*word);

        if ((x = hash & 0xF0000000L) != 0)
        {
            hash ^= (x >> 24);
        }

        hash &= ~x;
    }

    return hash;
}

int ht_get_hash(char* s, int num_buckets, int attempt)
{
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}

ht_hash_table *add_new_record(ht_hash_table *main_ht, char* value)
{
	const int load = main_ht->count * 100 / main_ht->size;
	//printf("main_ht->count, main_ht->size, load: %d, %d, %d\n", main_ht->count, main_ht->size, load);
	// For linear probing, as α approaches 1, the number of collisions increases
    if (load >= 31) 
    {	
		ht_hash_table *new_ht = ht_resize(main_ht); 
		
		main_ht->size = main_ht->size*2;
		main_ht->count = new_ht->count;
		main_ht->items = realloc(main_ht->items, main_ht->size * sizeof(ht_item*));
		
		for (int i = 0; i < main_ht->size; i++)
		{
			main_ht->items[i] = &HT_DELETED_ITEM;
		}
		
		for (int i = 0; i < new_ht->size; i++)
		{
			ht_item* temp = new_ht->items[i];
			if (temp != &HT_DELETED_ITEM)
			{
				main_ht = ht_insert(main_ht, temp->value);
			}			
		}
		
		ht_del_hash_table(new_ht);	
		
		//printf("(after delete) new_ht != NULL): %d\n", (new_ht != NULL));
    } 
    main_ht = ht_insert(main_ht, value);
    return main_ht;
}

ht_hash_table *ht_insert(ht_hash_table *main_ht, char* value)
{
    unsigned int key = PJW_hash(value);
    ht_item* item = ht_new_item(key, value);
    int index = ht_get_hash(item->value, main_ht->size, 0);
    ht_item* cur_item = main_ht->items[index];
    int attempt = 1;
    while (cur_item != &HT_DELETED_ITEM)
    {
		//printf("(ht_insert 3) i, main_ht->size, (cur_item->key == key): %d, %d, %d\n", i, main_ht->size, (cur_item->key == key));
		if (cur_item->key == key) 
		{     
			//printf("(ht_insert 3) found =>  i, main_ht->size, (cur_item->key == key): %d, %d, %d\n", i, main_ht->size, (cur_item->key == key));           
			return main_ht;
        }
		index = ht_get_hash(cur_item->value, main_ht->size, attempt);
        cur_item = main_ht->items[index];
        attempt++;
        //printf("(ht_insert 3)\n");
    }    
    
    bool collision;
    do
    {
		collision = false;
		for (int k = 0; k < main_ht->size; k++)
		{
			ht_item* stored_item = main_ht->items[index];
			if ((stored_item != &HT_DELETED_ITEM) && (stored_item->key == item->key))
			{
				//printf("(ht_insert 5) stored_item->value, stored_item->key, k, index: %s, %d, %d, %d\n", stored_item->value, stored_item->key, k, index);           
				collision = true;
				break;
			}
		}
		if (collision)
		{
			index++;
			//collision = false;
		}
	}
	while (collision);
	
	main_ht->items[index] = item;
	main_ht->count++;
	last_index = index;
	printf("(ht_insert 2) value, key, index: %s, %d, %d\n", value, key, index);
	//printf("(ht_insert 2) value, key, index, count: %s, %d, %d, %d\n", value, key, index, main_ht->count);
		
    return main_ht;
}

ht_item *ht_search(ht_hash_table *main_ht, char* value)
{
    int index = ht_get_hash(value, main_ht->size, 0);
    ht_item *item = main_ht->items[index];
    int attempt = 1;
    unsigned int key = PJW_hash(value);
    //printf("(ht_search 0) value, index, (item != NULL): %s, %d, %d\n", value, index, (item != NULL));
    while (item != &HT_DELETED_ITEM)
    {
		//printf("(ht_search 2) value, index, (item->key == key): %s, %d, %d\n", value, index, (item->key == key));
        //if (strcmp(cur_item->key, key) == 0) {
        if (item->key == key)
        {
			//return item->value;
            //printf("(ht_search) value, item->key, item->value, index: %s, %d, %s, %d\n", value, item->key, item->value, index);
			return item;				
        }
        
        index = ht_get_hash(item->value, main_ht->size, attempt);
        item = main_ht->items[index];
        attempt++;
    } 
    return NULL;
}

void delete_a_record(ht_hash_table *main_ht, char* value)
{
	int load = main_ht->count * 100 / main_ht->size;
    if (load < 10)
    {
		ht_hash_table *new_ht = ht_resize(main_ht); 
		
		//printf("main_ht->size, new_ht->size: %d, %d\n", main_ht->size, new_ht->size);
		main_ht->size = main_ht->size/2;
		main_ht->items = realloc(main_ht->items, main_ht->size * sizeof(ht_item*));
		
		for (int i = 0; i < main_ht->size; i++)
		{
			main_ht->items[i] = &HT_DELETED_ITEM;
		}
		//printf("(add_new_record 9\n");
		for (int i = 0; i < new_ht->size; i++)
		{
			ht_item* temp = new_ht->items[i];
			if (temp != &HT_DELETED_ITEM)
			{
				main_ht = ht_insert(main_ht, temp->value);
			}			
		}
		//printf("main_ht->size: %d\n", main_ht->size);
		ht_del_hash_table(new_ht);	
		
		//printf("(after delete) new_ht != NULL): %d\n", (new_ht != NULL));
    } 
     
    ht_delete(main_ht, value);
    //return ht;
}


void ht_delete(ht_hash_table *main_ht, char* value)
{   
    int index = ht_get_hash(value, main_ht->size, 0);
    ht_item* item = main_ht->items[index];
    int attempt = 1;
    unsigned int key = PJW_hash(value);
    //printf("(ht_delete 0) (item != NULL), value, key, index: %d, %s, %d, %d\n", (item != NULL), value, key, index);
    while (item != &HT_DELETED_ITEM)
    {
		//if (strcmp(cur_item->key, key) == 0) {
        if (item->key == key)
        {
			//printf("(ht_delete 3) (item->key == key), value, key, index: %d, %s, %d, %d\n", (item->key == key), value, key, index);
            ht_del_item(item);
            main_ht->items[index] = &HT_DELETED_ITEM;
            main_ht->count--;
            break;
        }
        
        //printf("(ht_delete 4) (item != NULL), value, key, index: %d, %s, %d, %d\n", (item != NULL), value, key, index);
        index = ht_get_hash(item->value, main_ht->size, attempt);
        item = main_ht->items[index];
        attempt++;
    } 
    //ht->count--;
}

/*
 * Return whether x is prime or not
 *
 * Returns:
 *   1  - prime
 *   0  - not prime
 *   -1 - undefined (i.e. x < 2)
 */
int is_prime(int x)
{
    if (x < 2)
    { 
		return -1; 
	}
    if (x < 4)
    { 
		return 1; 
	}
    if ((x % 2) == 0)
    { 
		return 0; 
	}
    for (int i = 3; i <= floor(sqrt((double) x)); i += 2)
    {
        if ((x % i) == 0)
        {
            return 0;
        }
    }
    return 1;
}

/*
 * Return the next prime after x, or x if x is prime
 */
int next_prime(int x)
{
    while (is_prime(x) != 1)
    {
        x++;
    }
    return x;
}

ht_hash_table* ht_new_fixed(int size)
{
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));
    ht->size = size;

    //ht->size = next_prime(base_size);

    ht->count = 0;
    ht->items = calloc((int)size, sizeof(ht_item*));
    for (int i = 0; i < ht->size; i++) 
	{ 
        ht->items[i] = &HT_DELETED_ITEM;
    }
    return ht;
}

// hash_table.c
ht_hash_table* ht_new_sized(int base_size)
{
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));
    //ht->size = base_size;

    ht->size = next_prime(base_size);

    ht->count = 0;
    ht->items = calloc((int)ht->size, sizeof(ht_item*));
    return ht;
}

ht_hash_table *ht_resize(ht_hash_table *ht) 
{
	//int new_size = main_ht->size / 2;
	ht_hash_table *new_ht = ht_new_fixed(ht->size);
	//printf("(add_new_record 0\n");
    for (int i = 0; i < ht->size; i++)
	{
		//printf("(add_new_record 1\n");
		ht_item* item = ht->items[i];
		//printf("(add_new_record 2\n");
		if (item != &HT_DELETED_ITEM)
		{
			//printf("(add_new_record 3\n");
			unsigned int key_rz = item->key;		//PJW_hash(item->value);
			//printf("(add_new_record 03\n");
			ht_item* item_rz = ht_new_item(key_rz, item->value);
			//printf("(add_new_record 003\n");
			int index_rz = ht_get_hash(item_rz->value, new_ht->size, 0);
			//printf("(add_new_record 0003\n");
			ht_item* cur_item_rz = new_ht->items[index_rz];
			//printf("(add_new_record 00003\n");
			int attempt = 1;
			//printf("(add_new_record 4\n");
			while (cur_item_rz != &HT_DELETED_ITEM)
			{	
				//printf("(add_new_record 5\n");			      
				index_rz = ht_get_hash(cur_item_rz->value, new_ht->size, attempt);
				//printf("(add_new_record 6\n");
				cur_item_rz = new_ht->items[index_rz];
				attempt++;
			}
			//printf("(add_new_record 7\n");
			new_ht->items[index_rz] = item_rz;
			new_ht->count++;
			//last_index = index;
		}
	}
	return new_ht;
}
