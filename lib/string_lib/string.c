#include "string.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>


StringMemory* string_memory_init(Arena* arena)
{
    size_t size = 0;
    StringMemory* sm = arena_alloc(arena, sizeof(StringMemory) + sizeof(uint8_t*) * 50, &size);
    sm->freeMemory = (uint8_t**)(sm + 1);
    sm->count = 0;
    sm->maxCount = 50;
    sm->size = size;
    sm->arena = arena_init(ARENA_BLOCK_SIZE, STRING_CHUNK_SIZE, false);

    return sm;
}

void string_memory_destroy(StringMemory* sm)
{
    arena_destroy(sm->arena);
    sm->arena = NULL;
    sm->freeMemory = NULL;
    sm->maxCount = 0;
}


String* string_init(Arena* arena, StringMemory* sm)
{
    size_t size;
    String* string = arena_alloc(arena, sizeof(String) + sizeof(uint8_t*) * 5, &size);
    string->memory = (uint8_t**)(string + 1);

    if (sm->count > 0)
        string->memory[0] = sm->freeMemory[--sm->count];
    else
        string->memory[0] = arena_alloc(sm->arena, sizeof(char) * STRING_CHUNK_SIZE, NULL);

    string->memoryChunks = 1;
    string->maxChunks = 5;
    string->count = 0;
    string->maxCount = STRING_CHUNK_SIZE;
    string->size = size;

    return string;
}

/*void string_to_arena_free_list(Arena* arena, String* string)
{
    arena->freeList->memory[arena->freeList->count] = string;
    arena->freeList->sizes[arena->freeList->count++] = string->size;
}*/

String* string_realloc(Arena* arena, String* oldString)
{
    uint8_t const additionalChunks = 5;
    size_t size;
    size_t oldSize = sizeof(String) + sizeof(uint8_t*) * oldString->maxChunks;
    size_t newSize = sizeof(String) + sizeof(uint8_t*) * (oldString->maxChunks + additionalChunks);

    String* newString = arena_realloc(arena, oldString, oldSize, newSize, &size);
    if (newString)
    {
        newString->memory = (uint8_t**)(newString + 1);
        newString->maxChunks += additionalChunks;
        newString->size = size;
    }

    //printf("after realloc %zu\n", size);
    return newString;
}

void remove_last_char(String* string)
{
    if (string->count == 0)
        return;

    string->count--;

    if (string->count % STRING_CHUNK_SIZE == 0 && string->memoryChunks > 1)
    {
        string->memoryChunks--;
        string->maxCount -= STRING_CHUNK_SIZE;
    }
}

int string_c_append(Arena* arena, String** stringA, StringMemory* sm, const char* toAppend)
{
    int rValue = 0;
    String* string = *stringA;

    uint32_t appendCount = 0;

    while(toAppend[appendCount] != '\0')
        appendCount++;

    //printf("Appending %s of length %u\n", toAppend, appendCount);
    //printf("Current string length %u\n", string->count);

    if (string->count + appendCount <= string->maxCount)
    {
        uint32_t i = string->count - (STRING_CHUNK_SIZE *(string->memoryChunks-1));
        memcpy(string->memory[string->memoryChunks -1] + i, toAppend, appendCount);
        string->count += appendCount;
    }
    else
    {
        //use up rest of the memoryblock
        uint32_t remaining = string->maxCount - string->count;
        //printf("Remaining space in current chunk: %u\n", remaining);
        if (remaining > 0)
        {
            uint32_t i = string->count - (STRING_CHUNK_SIZE *(string->memoryChunks-1));
            memcpy(string->memory[string->memoryChunks -1] + i, toAppend, remaining);
            string->count += remaining;

            appendCount -= remaining;
        }

        while(appendCount > 0)
        {
            if (string->memoryChunks == string->maxChunks)
            {
                string = string_realloc(arena, string);
                if (!string)
                {
                    printf("ERROR - String Realloc failed!\n");
                    return -1;
                }
                rValue++;
            }

            if (sm->count > 0)
                string->memory[string->memoryChunks++] =  sm->freeMemory[--sm->count];
            else
                string->memory[string->memoryChunks++] = arena_alloc(arena, sizeof(char) * STRING_CHUNK_SIZE, NULL);

            if (appendCount > STRING_CHUNK_SIZE)
            {
                memcpy(string->memory[string->memoryChunks -1], toAppend + remaining, STRING_CHUNK_SIZE);
                string->count += STRING_CHUNK_SIZE;
                remaining += STRING_CHUNK_SIZE;
                appendCount -= STRING_CHUNK_SIZE;
            }
            else
            {
                memcpy(string->memory[string->memoryChunks -1], toAppend + remaining, appendCount);
                string->count += appendCount;
                appendCount = 0;

            }
            string->maxCount += STRING_CHUNK_SIZE;
        }
    }
    *stringA = string;
    return rValue;
}


int string_append(Arena* arena, String** stringA, StringMemory* sm, String* toAppend)
{
    int rValue = 0;

    String* string = *stringA;

    uint32_t appendCount = toAppend->count;

    if (string->count + appendCount <= string->maxCount)
    {
        uint32_t i = string->count - (STRING_CHUNK_SIZE *(string->memoryChunks-1));
        memcpy(string->memory[string->memoryChunks -1] + i, toAppend->memory[toAppend->memoryChunks -1], appendCount);
        string->count += appendCount;
    }
    else
    {
        char *tmp = malloc(sizeof(char) * (toAppend->count +1));
        uint8_t index = 0;
        uint8_t j = 0;
        uint32_t i = 0;
        while(i < toAppend->count)
        {
            if (i % STRING_CHUNK_SIZE == 0 && i != 0)
            {
                index++;
                j = 0;
            }
            tmp[i++] = toAppend->memory[index][j++];
        }
        tmp[i] = '\0';

        rValue = string_c_append(arena, &string, sm, tmp);

        free(tmp);
    }
    *stringA = string;
    return rValue;
}


void resize_string_memory(Arena* arena, StringMemory** stringMemory)
{
    StringMemory* sm = *stringMemory;
    uint8_t const additionalPointers = 50;

    if (sm->count == sm->maxCount) //expand
    {
        size_t size;
        size_t newSize = sizeof(StringMemory) + (sm->maxCount + additionalPointers) * sizeof(uint8_t*);
        sm = arena_realloc(arena, sm, sm->size, newSize, &size);
        if (!sm)
        {
            printf("ERROR in memoryString realloc");
            return;
        }
        else
        {
            sm->freeMemory = (uint8_t**)(sm + 1);
            sm->maxCount += additionalPointers;
            sm->size = size;
        }
    }
    else if(sm->maxCount > additionalPointers && sm->count < (sm->maxCount - additionalPointers))  //shrink
    {
        size_t size;
        size_t newSize = sizeof(StringMemory) + (sm->maxCount - additionalPointers) * sizeof(uint8_t*);
        sm = arena_realloc(arena, sm, sm->size, newSize, &size);
        if (!sm)
        {
            printf("ERROR in memoryString realloc");
            return;
        }
        else
        {
            sm->freeMemory = (uint8_t**)(sm + 1);
            sm->maxCount -= additionalPointers;
            sm->size = size;
        }
    }
    *stringMemory = sm;
}



int string_append_flagdead(Arena* arena, String** stringA, StringMemory** stringMemory, String* toAppend)
{
    int rValue = 0;
    String* string = *stringA;
    rValue = string_append(arena, &string, *stringMemory, toAppend);
    string_dead(arena, toAppend, stringMemory);
    *stringA = string;
    return rValue;
}

void string_dead(Arena* arena, String* string, StringMemory** stringMemory)
{
    StringMemory* sm = *stringMemory;
    int count = 0;
    while (string->memoryChunks > 0)
    {

        sm->freeMemory[sm->count++] = string->memory[string->memoryChunks-- -1];
        if (sm->count == sm->maxCount)
            resize_string_memory(arena, &sm);
        //printf("Count: %d | %u\n", ++count, sm->count);
    }
    arena_free_list_add(arena, string, string->size);

    //check to see if shrink
    resize_string_memory(arena, &sm);
    *stringMemory = sm;
}

void c_string_sendback(String* string, char* dest)
{
    uint8_t index = 0;
    uint8_t j = 0;
    uint32_t i = 0;
    while(i < string->count)
    {
        if (i % STRING_CHUNK_SIZE == 0 && i != 0)
        {
            index++;
            j = 0;
        }
        dest[i++] = string->memory[index][j++];
        assert(dest[i-1] != '\0');
    }
    dest[i] = '\0';
}


void print(String* string)
{
    uint8_t index = 0;
    uint8_t j = 0;
    for (uint32_t i = 0; i < string->count; ++i)
    {
        if (i % STRING_CHUNK_SIZE == 0 && i != 0)
        {
            index++;
            j = 0;
            //printf("New Chunk\n");
        }
        assert(string->memory[index][j] != '\0');
        putchar(string->memory[index][j++]);
    }
}


void println(String* string)
{
    if (string == NULL)
    {
        putchar('\n');
        return;
    }

    uint8_t index = 0;
    uint8_t j = 0;
    for (uint32_t i = 0; i < string->count; ++i)
    {
        if (i % STRING_CHUNK_SIZE == 0 && i != 0)
        {
            index++;
            j = 0;
            //printf("New Chunk\n");
        }
        assert(string->memory[index][j] != '\0');
        putchar(string->memory[index][j++]);
    }
    putchar('\n');
}
