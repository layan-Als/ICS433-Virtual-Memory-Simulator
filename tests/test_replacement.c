
#include <stdio.h>
#include <assert.h>
#include "replacement.h"

/*
 * ICS 433 - Virtual Memory Simulator
 *
 * File: test_replacement.c
 *
 * Purpose:
 * Automatically test FIFO and LRU page-replacement
 * algorithms under different memory conditions.
 */

/*
 * Check that the algorithm produces the expected
 * number of hits, faults, and replacements.
 */
void check_results(Statistics result,
                   int hits,
                   int faults,
                   int replacements) {

    assert(result.hits == hits);
    assert(result.faults == faults);
    assert(result.replacements == replacements);
}

int main(void) {

    // Test 1: Compare FIFO and LRU.
    int pages1[] = {1, 2, 3, 1, 4, 1};

    Statistics fifo1 = run_fifo(pages1, 6, 3);
    Statistics lru1 = run_lru(pages1, 6, 3);

    check_results(fifo1, 1, 5, 2);
    check_results(lru1, 2, 4, 1);

    printf("Test 1 passed: FIFO vs LRU\n");

    // Test 2: All requests are for the same page.
    int pages2[] = {2, 2, 2, 2};

    Statistics fifo2 = run_fifo(pages2, 4, 3);
    Statistics lru2 = run_lru(pages2, 4, 3);

    check_results(fifo2, 3, 1, 0);
    check_results(lru2, 3, 1, 0);

    printf("Test 2 passed: Repeated page\n");

    // Test 3: Only one physical frame.
    int pages3[] = {1, 2, 1};

    Statistics fifo3 = run_fifo(pages3, 3, 1);
    Statistics lru3 = run_lru(pages3, 3, 1);

    check_results(fifo3, 0, 3, 2);
    check_results(lru3, 0, 3, 2);

    printf("Test 3 passed: Single frame\n");

    // Test 4: More frames than distinct pages.
    int pages4[] = {1, 2, 3, 1, 2, 3};

    Statistics fifo4 = run_fifo(pages4, 6, 5);
    Statistics lru4 = run_lru(pages4, 6, 5);

    check_results(fifo4, 3, 3, 0);
    check_results(lru4, 3, 3, 0);

    printf("Test 4 passed: Sufficient frames\n");

    // Test 5: Invalid frame count.
    int pages5[] = {1, 2, 3};

    Statistics fifo5 = run_fifo(pages5, 3, 0);
    Statistics lru5 = run_lru(pages5, 3, 0);

    check_results(fifo5, 0, 0, 0);
    check_results(lru5, 0, 0, 0);

    printf("Test 5 passed: Invalid frame count\n");

    
    // Test 6: Empty page-reference sequence.
    int pages6[] = {1, 2, 3};

    Statistics fifo6 = run_fifo(pages6, 0, 3);
    Statistics lru6 = run_lru(pages6, 0, 3);

    check_results(fifo6, 0, 0, 0);
    check_results(lru6, 0, 0, 0);

    printf("Test 6 passed: Empty trace\n");

    // Test 7: NULL page-reference sequence.
    Statistics fifo7 = run_fifo(NULL, 5, 3);
    Statistics lru7 = run_lru(NULL, 5, 3);

    check_results(fifo7, 0, 0, 0);
    check_results(lru7, 0, 0, 0);

    printf("Test 7 passed: NULL input\n");

    // Test 8: Negative number of frames.
    int pages8[] = {1, 2, 3};

    Statistics fifo8 = run_fifo(pages8, 3, -1);
    Statistics lru8 = run_lru(pages8, 3, -1);

    check_results(fifo8, 0, 0, 0);
    check_results(lru8, 0, 0, 0);

    printf("Test 8 passed: Negative frame count\n");


    // Test 9: Locality of reference.
    // Frequently accessed pages with sufficient frames.
    int pages9[] = {1, 2, 1, 2, 1, 2, 3, 1, 2};

    Statistics fifo9 = run_fifo(pages9, 9, 3);
    Statistics lru9 = run_lru(pages9, 9, 3);

    check_results(fifo9, 6, 3, 0);
    check_results(lru9, 6, 3, 0);

    printf("Test 9 passed: Locality of reference\n");

    // Test 10: Memory pressure.
    // Three different pages competing for two frames.
    int pages10[] = {1, 2, 3, 1, 2, 3};

    Statistics fifo10 = run_fifo(pages10, 6, 2);
    Statistics lru10 = run_lru(pages10, 6, 2);

    check_results(fifo10, 0, 6, 4);
    check_results(lru10, 0, 6, 4);

    printf("Test 10 passed: Memory pressure\n");

    // Test 11: All page faults.
    // Every requested page is different.
    int pages11[] = {1, 2, 3, 4, 5, 6};

    Statistics fifo11 = run_fifo(pages11, 6, 3);
    Statistics lru11 = run_lru(pages11, 6, 3);

    check_results(fifo11, 0, 6, 3);
    check_results(lru11, 0, 6, 3);

    printf("Test 11 passed: All page faults\n");


    // Test 12: Belady's Anomaly in FIFO.
    // Increasing the number of frames may increase page faults.
    int pages12[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};

    Statistics fifo12_3 = run_fifo(pages12, 12, 3);
    Statistics fifo12_4 = run_fifo(pages12, 12, 4);

    // Verify the expected statistics.
    check_results(fifo12_3, 3, 9, 6);
    check_results(fifo12_4, 2, 10, 6);

    // Confirm Belady's Anomaly.
    assert(fifo12_4.faults > fifo12_3.faults);

    printf("Test 12 passed: Belady's Anomaly\n");
    printf("  FIFO with 3 frames: %d faults\n", fifo12_3.faults);
    printf("  FIFO with 4 frames: %d faults\n", fifo12_4.faults);

    // Test 13: LRU with increasing frame counts.
    // Use the same reference sequence for a fair comparison.
    Statistics lru13_3 = run_lru(pages12, 12, 3);
    Statistics lru13_4 = run_lru(pages12, 12, 4);

    // Verify the expected statistics.
    check_results(lru13_3, 2, 10, 7);
    check_results(lru13_4, 4, 8, 4);

    // Verify that increasing frames does not increase faults.
    assert(lru13_4.faults <= lru13_3.faults);

    printf("Test 13 passed: LRU frame comparison\n");
    printf("  LRU with 3 frames: %d faults\n", lru13_3.faults);
    printf("  LRU with 4 frames: %d faults\n", lru13_4.faults);

    // All tests completed without assertion failures.
    printf("\nAll FIFO and LRU tests passed!\n");

    return 0;
}
