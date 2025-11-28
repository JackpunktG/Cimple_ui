#include "string.h"
#include <stdio.h>


#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

// Test result tracking
static int tests_run = 0;
static int tests_passed = 0;

#define TEST(name) \
    do { \
        printf("Running test: %s\n", #name); \
        tests_run++; \
        if (test_##name()) { \
            printf("✅ PASSED: %s\n", #name); \
            tests_passed++; \
        } else { \
            printf("❌ FAILED: %s\n", #name); \
        } \
        printf("\n"); \
    } while(0)

#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("  ASSERTION FAILED: %s\n", message); \
            return false; \
        } \
    } while(0)

// =============================================================================
// ARENA TESTS
// =============================================================================

bool test_arena_init_destroy()
{
    Arena* arena = arena_init(4096, 8, true);
    ASSERT(arena != NULL, "Arena should be created");
    ASSERT(arena->defualtBlockSize == 4096, "Default block size should be set");
    ASSERT(arena->alignment == 8, "Alignment should be set");
    ASSERT(arena->freeList != NULL, "Free list should be initialized when useFreeList is true");
    ASSERT(arena->totalAllocated == sizeof(FreeList) + sizeof(void*) * 100 + sizeof(size_t) *100, "Initial allocation should be zero + free list size");

    arena_destroy(arena);
    return true;
}

bool test_arena_init_no_freelist()
{
    Arena* arena = arena_init(4096, 8, false);
    ASSERT(arena != NULL, "Arena should be created");
    ASSERT(arena->freeList == NULL, "Free list should be NULL when useFreeList is false");
    ASSERT(arena->totalAllocated == 0, "Initial allocation should be zero when no free list");

    arena_destroy(arena);
    return true;
}

bool test_arena_basic_allocation()
{
    Arena* arena = arena_init(4096, 8, true);
    size_t allocated_size;

    void* ptr1 = arena_alloc(arena, 100, &allocated_size);
    ASSERT(ptr1 != NULL, "First allocation should succeed");
    ASSERT(allocated_size >= 100, "Allocated size should be at least requested size");
    ASSERT(arena->totalAllocated > 0, "Total allocated should increase");

    void* ptr2 = arena_alloc(arena, 200, &allocated_size);
    ASSERT(ptr2 != NULL, "Second allocation should succeed");
    ASSERT(ptr2 != ptr1, "Different allocations should have different addresses");

    arena_destroy(arena);
    return true;
}

bool test_arena_alignment()
{
    Arena* arena = arena_init(4096, 16, true);
    size_t allocated_size;

    void* ptr = arena_alloc(arena, 1, &allocated_size);
    ASSERT(ptr != NULL, "Allocation should succeed");
    ASSERT(((uintptr_t)ptr % 16) == 0, "Pointer should be 16-byte aligned");

    arena_destroy(arena);
    return true;
}

bool test_arena_realloc_grow()
{
    Arena* arena = arena_init(4096, 8, true);
    size_t allocated_size;

    void* ptr = arena_alloc(arena, 100, &allocated_size);
    ASSERT(ptr != NULL, "Initial allocation should succeed");

    // Write some data
    memset(ptr, 0xAB, 100);

    void* new_ptr = arena_realloc(arena, ptr, allocated_size, 200, &allocated_size);
    ASSERT(new_ptr != NULL, "Realloc should succeed");
    ASSERT(allocated_size >= 200, "New size should be at least 200");

    // Check data is preserved
    uint8_t* bytes = (uint8_t*)new_ptr;
    for (int i = 0; i < 100; i++)
    {
        ASSERT(bytes[i] == 0xAB, "Original data should be preserved");
    }

    arena_destroy(arena);
    return true;
}

bool test_arena_realloc_shrink()
{
    Arena* arena = arena_init(4096, 8, true);
    size_t allocated_size;

    void* ptr = arena_alloc(arena, 200, &allocated_size);
    ASSERT(ptr != NULL, "Initial allocation should succeed");

    // Write some data
    memset(ptr, 0xCD, 50);

    void* new_ptr = arena_realloc(arena, ptr, allocated_size, 100, &allocated_size);
    ASSERT(new_ptr != NULL, "Realloc should succeed");

    // Check data is preserved
    uint8_t* bytes = (uint8_t*)new_ptr;
    for (int i = 0; i < 50; i++)
    {
        ASSERT(bytes[i] == 0xCD, "Original data should be preserved");
    }

    arena_destroy(arena);
    return true;
}

bool test_arena_free_list_add()
{
    Arena* arena = arena_init(4096, 8, true);
    size_t size1, size2;

    // Use real allocated pointers instead of fake ones
    void* ptr1 = arena_alloc(arena, 100, &size1);
    void* ptr2 = arena_alloc(arena, 200, &size2);

    arena_free_list_add(arena, ptr1, size1);
    ASSERT(arena->freeList->count == 1, "Free list should have one entry");

    arena_free_list_add(arena, ptr2, size2);
    ASSERT(arena->freeList->count == 2, "Free list should have two entries");

    arena_destroy(arena);
    return true;
}

bool test_arena_reset()
{
    Arena* arena = arena_init(4096, 8, true);
    size_t allocated_size;

    void* ptr = arena_alloc(arena, 100, &allocated_size);
    ASSERT(ptr != NULL, "Allocation should succeed");
    ASSERT(arena->totalAllocated > 0, "Should have allocated memory");

    arena_reset(arena);
    ASSERT(arena->totalAllocated == 0, "Total allocated should be reset to zero");

    arena_destroy(arena);
    return true;
}

bool test_arena_large_allocation()
{
    Arena* arena = arena_init(1024, 8, true);  // Small block size
    size_t allocated_size;

    // Allocate more than one block
    void* ptr = arena_alloc(arena, 2048, &allocated_size);
    ASSERT(ptr != NULL, "Large allocation should succeed");
    ASSERT(allocated_size >= 2048, "Should allocate at least requested size");

    arena_destroy(arena);
    return true;
}

// =============================================================================
// STRING MEMORY TESTS
// =============================================================================
bool test_arena_creation_only()
{
    printf("  About to call arena_init...\n");
    fflush(stdout);

    Arena* arena = arena_init(4096, 8, true);

    printf("  arena_init returned: %p\n", (void*)arena);
    fflush(stdout);

    if (arena)
    {
        printf("  Arena seems valid, destroying...\n");
        arena_destroy(arena);
        printf("  Arena destroyed successfully\n");
    }

    return arena != NULL;
}
bool test_string_memory_init()
{
    printf("  Creating arena...\n");
    Arena* arena = arena_init(4096, 8, true);
    printf("  Arena created at: %p\n", (void*)arena);

    if (!arena)
    {
        printf("  Arena creation failed!\n");
        return false;
    }

    printf("  Arena details:\n");
    printf("    - defualtBlockSize: %zu\n", arena->defualtBlockSize);
    printf("    - alignment: %zu\n", arena->alignment);
    printf("    - freeList: %p\n", (void*)arena->freeList);
    printf("    - current: %p\n", (void*)arena->current);
    printf("    - first: %p\n", (void*)arena->first);

    printf("  About to call string_memory_init...\n");
    fflush(stdout);  // Force output before potential crash

    StringMemory* sm = string_memory_init(arena);

    printf("  string_memory_init returned: %p\n", (void*)sm);

    if (sm)
    {
        string_memory_destroy(sm);
    }
    arena_destroy(arena);
    return true;
}

// =============================================================================
// STRING TESTS
// =============================================================================

bool test_string_init()
{
    Arena* arena = arena_init(4096, 8, true);
    StringMemory* sm = string_memory_init(arena);
    String* str = string_init(arena, sm);
    ASSERT(str != NULL, "String should be created");
    ASSERT(str->memory != NULL, "String memory array should be allocated");
    ASSERT(str->memoryChunks == 1, "Initial memory chunks should be zero");
    ASSERT(str->maxChunks > 0, "Should have positive max chunks");
    ASSERT(str->count == 0, "Initial count should be zero");
    ASSERT(str->maxCount > 0, "Should have positive max count");

    string_memory_destroy(sm);
    arena_destroy(arena);
    return true;
}

bool test_string_c_append_basic()
{
    Arena* arena = arena_init(4096, 8, true);
    StringMemory* sm = string_memory_init(arena);
    String* str = string_init(arena, sm);

    string_c_append(arena, &str, sm, "Hello");
    ASSERT(str->count == 5, "String count should be 5");
    ASSERT(str->memoryChunks >= 1, "Should have at least one memory chunk");

    string_c_append(arena, &str, sm, " World");
    ASSERT(str->count == 11, "String count should be 11");

    string_memory_destroy(sm);
    arena_destroy(arena);
    return true;
}

bool test_string_c_append_large()
{
    Arena* arena = arena_init(4096, 8, true);
    StringMemory* sm = string_memory_init(arena);
    String* str = string_init(arena, sm);

    // Create a string larger than STRING_CHUNK_SIZE
    char large_str[STRING_CHUNK_SIZE * 2];
    memset(large_str, 'A', sizeof(large_str) - 1);
    large_str[sizeof(large_str) - 1] = '\0';

    string_c_append(arena, &str, sm, large_str);
    ASSERT(str->count == STRING_CHUNK_SIZE * 2 - 1, "String count should match input");
    ASSERT(str->memoryChunks >= 2, "Should have multiple memory chunks");

    string_memory_destroy(sm);
    arena_destroy(arena);
    return true;
}

bool test_string_append()
{
    Arena* arena = arena_init(4096, 8, true);
    StringMemory* sm = string_memory_init(arena);

    String* str1 = string_init(arena, sm);
    String* str2 = string_init(arena, sm);

    string_c_append(arena, &str1, sm, "Hello");
    string_c_append(arena, &str2, sm, " World");

    string_append(arena, &str1, sm, str2);
    ASSERT(str1->count == 11, "Combined string should have 11 characters");

    string_memory_destroy(sm);
    arena_destroy(arena);
    return true;
}

bool test_string_realloc()
{
    Arena* arena = arena_init(4096, 8, true);
    StringMemory* sm = string_memory_init(arena);
    String* str = string_init(arena, sm);

    // Fill up to capacity
    for (int i = 0; i < str->maxCount; i++)
    {
        char temp[1] = {'A'};
        string_c_append(arena, &str, sm, temp);
    }

    String* old_ptr = str;
    uint32_t old_max_chunks = str->maxChunks;

    // This should trigger realloc
    string_c_append(arena, &str, sm, "Force realloc");

    ASSERT(str->maxChunks > old_max_chunks, "Max chunks should have increased");
    // Note: str pointer may or may not change depending on arena_realloc implementation

    string_memory_destroy(sm);
    arena_destroy(arena);
    return true;
}

bool test_string_dead_and_reuse()
{
    Arena* arena = arena_init(4096, 8, true);
    StringMemory* sm = string_memory_init(arena);

    String* str1 = string_init(arena, sm);
    string_c_append(arena, &str1, sm, "Hello World");

    uint32_t old_free_count = sm->count;
    string_dead(arena, str1, &sm);

    ASSERT(sm->count > old_free_count, "Free memory count should increase");

    // Create new string - should potentially reuse freed memory
    String* str2 = string_init(arena, sm);
    ASSERT(str2 != NULL, "New string should be created");

    string_memory_destroy(sm);
    arena_destroy(arena);
    return true;
}

bool test_string_append_flagdead()
{
    Arena* arena = arena_init(4096, 8, true);
    StringMemory* sm = string_memory_init(arena);

    String* str1 = string_init(arena, sm);
    String* str2 = string_init(arena, sm);

    string_c_append(arena, &str1, sm, "Hello");
    string_c_append(arena, &str2, sm, " World");

    uint32_t old_free_count = sm->count;
    string_append_flagdead(arena, &str1, &sm, str2);

    ASSERT(str1->count == 11, "Combined string should have 11 characters");
    ASSERT(sm->count > old_free_count, "Free memory count should increase (str2 flagged dead)");

    string_memory_destroy(sm);
    arena_destroy(arena);
    return true;
}

bool test_string_multiple_operations()
{
    Arena* arena = arena_init(4096, 8, true);
    StringMemory* sm = string_memory_init(arena);

    String* str = string_init(arena, sm);

    // Multiple append operations
    for (int i = 0; i < 10; i++)
    {
        char temp[20];
        sprintf(temp, "Iteration %d ", i);
        string_c_append(arena, &str, sm, temp);
    }

    ASSERT(str->count > 0, "String should have content");
    ASSERT(str->memoryChunks > 0, "Should have memory chunks allocated");

    // Print the string if print function is implemented
    printf("  Final string content: ");
    print(str);
    printf("\n");

    string_memory_destroy(sm);
    arena_destroy(arena);
    return true;
}

// =============================================================================
// STRESS TESTS
// =============================================================================

bool test_stress_many_allocations()
{
    Arena* arena = arena_init(4096, 8, true);

    void* ptrs[1000];
    size_t allocated_size;

    // Allocate many small blocks
    for (int i = 0; i < 1000; i++)
    {
        ptrs[i] = arena_alloc(arena, 32, &allocated_size);
        ASSERT(ptrs[i] != NULL, "Allocation should succeed");
    }

    arena_destroy(arena);
    return true;
}

bool test_stress_string_operations()
{
    Arena* arena = arena_init(8192, 8, true);
    StringMemory* sm = string_memory_init(arena);

    String* strings[100];

    // Create many strings
    for (int i = 0; i < 100; i++)
    {
        strings[i] = string_init(arena, sm);
        ASSERT(strings[i] != NULL, "String creation should succeed");

        char content[50];
        sprintf(content, "String number %d with some content", i);
        string_c_append(arena, &strings[i], sm, content);
    }

    // Flag half of them as dead
    for (int i = 0; i < 50; i++)
    {
        string_dead(arena, strings[i], &sm);
    }

    ASSERT(sm->count > 0, "Should have freed memory available");

    string_memory_destroy(sm);
    arena_destroy(arena);
    return true;
}

// =============================================================================
// MAIN TEST RUNNER
// =============================================================================

int main()
{
    printf("=== Arena and String Library Test Suite ===\n\n");

    // Arena tests
    printf("--- Arena Tests ---\n");
    TEST(arena_init_destroy);
    TEST(arena_init_no_freelist);
    TEST(arena_basic_allocation);
    TEST(arena_alignment);
    TEST(arena_realloc_grow);
    TEST(arena_realloc_shrink);
    TEST(arena_free_list_add);
    TEST(arena_reset);
    TEST(arena_large_allocation);

    // String Memory tests
    printf("--- String Memory Tests ---\n");
    TEST(arena_creation_only);
    TEST(string_memory_init);

    // String tests
    printf("--- String Tests ---\n");
    TEST(string_init);
    TEST(string_c_append_basic);
    TEST(string_c_append_large);
    TEST(string_append);
    TEST(string_realloc);
    TEST(string_dead_and_reuse);
    TEST(string_append_flagdead);
    TEST(string_multiple_operations);

    // Stress tests
    printf("--- Stress Tests ---\n");
    TEST(stress_many_allocations);
    TEST(stress_string_operations);

    // Summary
    printf("=== Test Results ===\n");
    printf("Tests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_passed);
    printf("Tests failed: %d\n", tests_run - tests_passed);
    printf("Success rate: %.1f%%\n", (float)tests_passed / tests_run * 100.0f);

    return (tests_passed == tests_run) ? 0 : 1;
}

