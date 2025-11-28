#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>

// Include your headers
#include "string.h"
#include "../arena_memory/arena_memory.h"

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

bool test_string_memory_init()
{
    printf("  Step 1: Creating arena...\n");
    fflush(stdout);

    Arena* arena = arena_init(4096, 8, true);
    printf("  Step 2: Arena created at: %p\n", (void*)arena);
    fflush(stdout);

    if (!arena)
    {
        printf("  Arena creation failed!\n");
        return false;
    }

    printf("  Step 3: About to call string_memory_init...\n");
    fflush(stdout);

    StringMemory* sm = string_memory_init(arena);
    printf("  Step 4: string_memory_init returned: %p\n", (void*)sm);
    fflush(stdout);

    if (!sm)
    {
        printf("  StringMemory creation failed!\n");
        arena_destroy(arena);
        return false;
    }

    printf("  Step 5: Destroying string memory...\n");
    fflush(stdout);

    string_memory_destroy(sm);

    printf("  Step 6: Destroying arena...\n");
    fflush(stdout);

    arena_destroy(arena);

    printf("  Step 7: All done!\n");
    return true;
}

int main()
{
    printf("=== Isolated String Memory Test ===\n");
    TEST(string_memory_init);

    printf("Tests run: %d, passed: %d\n", tests_run, tests_passed);
    return 0;
}
