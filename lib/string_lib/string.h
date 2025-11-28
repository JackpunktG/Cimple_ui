#ifndef STRING_LIB
#define STRING_LIB
#include "../arena_memory/arena_memory.h"


#define STRINGMEMORY_SIZE 816
/* String memory controller - arena for memory and free list for reuse of dead memory */
// struct for pointers of dead string memory rdy to be reused.
typedef struct
{
    uint8_t** freeMemory;
    Arena* arena;
    uint32_t count;
    uint32_t maxCount;
    uint32_t size;
} StringMemory;

// Initialises string memory unit to hold pointers to free blocks of memory
StringMemory* string_memory_init(Arena* arena);
// destory StringMemory arena
void string_memory_destroy(StringMemory* sm);

#define STRING_CHUNK_SIZE 256
#define STRING_SIZE 64
/* String type used for short character strings so far up to a max of 1280 char */
typedef struct
{
    uint8_t** memory;  //initalised with 5 pointers - memory only allocated when needed. If more than 5 move to "text data"
    uint8_t memoryChunks;
    uint8_t maxChunks;
    uint32_t count;
    uint32_t maxCount;
    uint32_t size;
} String;


// Initialises a new String in the given memory arena with starting max size of 256 characters.
String* string_init(Arena* arena, StringMemory* sm);

// Appends the given C-style, '\0' byte terminating string to the String, resizing if necessary.
void string_c_append(Arena* arena, String** string, StringMemory* sm, const char* toAppend);
// Appends the given String to the String, resizing if necessary.
void string_append(Arena* arena, String** string, StringMemory* sm, String* toAppend);
// Appends string and auto flags toAppend string as dead
void string_append_flagdead(Arena* arena, String** string, StringMemory** sm, String* toAppend);
// sends back a C-style string copy of the String to dest - dest must be preallocated with count + 1 bytes for null terminator
void c_string_sendback(String* string, char* dest);
//flags string for reuse
void string_dead(Arena* arena, String* string, StringMemory** sm);

//function to print the string to standard output
void print(String* string);

void println(String* string);


/* Text used for long character strings - unlimited */
typedef struct
{
    uint8_t* memory;
    uint32_t count;
    uint32_t maxCount;
    uint32_t size;
    bool isAlive;
} Text;

// Initialises a new Text meant for longer text
Text* text_init(Arena* arena);


#endif
