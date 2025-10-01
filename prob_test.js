const { test, describe } = require('node:test');
const assert = require('node:assert');

const {
    findLast_faulty,
    findLast_corrected,
    lastZero_faulty,
    lastZero_corrected,
    countPositive_faulty,
    countPositive_corrected,
    oddOrPos_faulty,
    oddOrPos_corrected
} = require('./prob.js');


// ============================================================================
// TEST SUITES
// ============================================================================

describe('Program 1: findLast', () => {
    test('findLast_faulty: Given failure case (y is at index 0)', () => {
        const result = findLast_faulty([2, 3, 5], 2);
        assert.strictEqual(result, -1); // Faulty version returns -1 instead of 0
    });

    test('findLast_corrected: Given failure case (y is at index 0)', () => {
        const result = findLast_corrected([2, 3, 5], 2);
        assert.strictEqual(result, 0);
    });

    test('findLast_faulty: Case that doesn\'t fail (y in middle)', () => {
        const result = findLast_faulty([2, 3, 5], 3);
        assert.strictEqual(result, 1);
    });

    test('findLast_corrected: Case that doesn\'t fail (y in middle)', () => {
        const result = findLast_corrected([2, 3, 5], 3);
        assert.strictEqual(result, 1);
    });
});

describe('Program 2: lastZero', () => {
    test('lastZero_faulty: Given failure case (multiple zeros)', () => {
        const result = lastZero_faulty([0, 1, 0]);
        assert.strictEqual(result, 0); // Faulty version returns 0 instead of 2
    });

    test('lastZero_corrected: Given failure case (multiple zeros)', () => {
        const result = lastZero_corrected([0, 1, 0]);
        assert.strictEqual(result, 2);
    });

    test('lastZero_faulty: Case that doesn\'t fail (only one zero)', () => {
        const result = lastZero_faulty([5, 1, 0, 8]);
        assert.strictEqual(result, 2);
    });

    test('lastZero_corrected: Case that doesn\'t fail (only one zero)', () => {
        const result = lastZero_corrected([5, 1, 0, 8]);
        assert.strictEqual(result, 2);
    });
});

describe('Program 3: countPositive', () => {
    test('countPositive_faulty: Given failure case (array contains 0)', () => {
        const result = countPositive_faulty([-4, 2, 0, 2]);
        assert.strictEqual(result, 3); // Faulty version returns 3 instead of 2
    });

    test('countPositive_corrected: Given failure case (array contains 0)', () => {
        const result = countPositive_corrected([-4, 2, 0, 2]);
        assert.strictEqual(result, 2);
    });

    test('countPositive_faulty: Case that doesn\'t fail (no zeros)', () => {
        const result = countPositive_faulty([-4, 2, 5]);
        assert.strictEqual(result, 2);
    });

    test('countPositive_corrected: Case that doesn\'t fail (no zeros)', () => {
        const result = countPositive_corrected([-4, 2, 5]);
        assert.strictEqual(result, 2);
    });
});

describe('Program 4: oddOrPos', () => {
    test('oddOrPos_faulty: Given failure case (contains negative odd)', () => {
        const result = oddOrPos_faulty([-3, -2, 0, 1, 4]);
        assert.strictEqual(result, 2); // Faulty version returns 2 instead of 3
    });

    test('oddOrPos_corrected: Given failure case (contains negative odd)', () => {
        const result = oddOrPos_corrected([-3, -2, 0, 1, 4]);
        assert.strictEqual(result, 3);
    });

    test('oddOrPos_faulty: Case that doesn\'t fail (no negative odds)', () => {
        const result = oddOrPos_faulty([-2, 0, 1, 4]);
        assert.strictEqual(result, 2);
    });

    test('oddOrPos_corrected: Case that doesn\'t fail (no negative odds)', () => {
        const result = oddOrPos_corrected([-2, 0, 1, 4]);
        assert.strictEqual(result, 2);
    });
});
