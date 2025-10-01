// ============================================================================
// 1. FAULTY PROGRAMS (As provided in the homework)
// ============================================================================

/** Faulty: Loop condition `i > 0` misses the element at index 0. */
function findLast_faulty(x, y) {
    if (!Array.isArray(x)) { throw new TypeError('The first parameter must be an array'); }
    if (typeof y !== 'number') { throw new TypeError('The second parameter must be a number'); }
    for (let i = x.length - 1; i > 0; i--) {
        if (x[i] === y) {
            return i;
        }
    }
    return -1;
}

/** Faulty: Finds the *first* zero, not the last, because it iterates forward. */
function lastZero_faulty(x) {
    if (!Array.isArray(x)) { throw new TypeError('Not an array'); }
    for (let i = 0; i < x.length; i++) {
        if (x[i] === 0) {
            return i;
        }
    }
    return -1;
}

/** Faulty: Counts 0 as a positive number because of `>= 0`. */
function countPositive_faulty(x) {
    if (!Array.isArray(x)) { throw new TypeError('Not an array'); }
    let count = 0;
    for (let i = 0; i < x.length; i++) {
        if (x[i] >= 0) {
            count++;
        }
    }
    return count;
}

/** Faulty: `x % 2 === 1` fails for negative odd numbers (e.g., -3 % 2 is -1). */
function oddOrPos_faulty(x) {
    if (!Array.isArray(x)) { throw new TypeError('Not an array'); }
    let count = 0;
    for (let i = 0; i < x.length; i++) {
        if (x[i] % 2 === 1 || x[i] > 0) {
            count++;
        }
    }
    return count;
}


// ============================================================================
// 2. CORRECTED PROGRAMS
// ============================================================================

/** Corrected: Loop condition is `i >= 0`. */
function findLast_corrected(x, y) {
    if (!Array.isArray(x)) { throw new TypeError('The first parameter must be an array'); }
    if (typeof y !== 'number') { throw new TypeError('The second parameter must be a number'); }
    for (let i = x.length - 1; i >= 0; i--) { // FIX
        if (x[i] === y) {
            return i;
        }
    }
    return -1;
}

/** Corrected: Iterates backward to find the last zero. */
function lastZero_corrected(x) {
    if (!Array.isArray(x)) { throw new TypeError('Not an array'); }
    for (let i = x.length - 1; i >= 0; i--) { // FIX
        if (x[i] === 0) {
            return i;
        }
    }
    return -1;
}

/** Corrected: Uses `> 0` to only count strictly positive numbers. */
function countPositive_corrected(x) {
    if (!Array.isArray(x)) { throw new TypeError('Not an array'); }
    let count = 0;
    for (let i = 0; i < x.length; i++) {
        if (x[i] > 0) { // FIX
            count++;
        }
    }
    return count;
}

/** Corrected: `x % 2 !== 0` works for both positive and negative odd numbers. */
function oddOrPos_corrected(x) {
    if (!Array.isArray(x)) { throw new TypeError('Not an array'); }
    let count = 0;
    for (let i = 0; i < x.length; i++) {
        if (x[i] % 2 !== 0 || x[i] > 0) { // FIX
            count++;
        }
    }
    return count;
}

module.exports = {
    findLast_faulty,
    findLast_corrected,
    lastZero_faulty,
    lastZero_corrected,
    countPositive_faulty,
    countPositive_corrected,
    oddOrPos_faulty,
    oddOrPos_corrected
};
