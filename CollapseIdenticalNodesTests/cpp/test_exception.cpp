// Test 7: Exception Handling (Invoke/Landingpad)
// The pass likely assumes all terminators are 'br'. Invoke instructions have successors
// that are normal destination and exception destination. The pass may crash trying to
// process landingpad blocks or miss paths through catch blocks.
int test_exception_paths(int a) {
    int res;
    try {
        if (a == 42) {
            res = 100;
            throw 1;
        } else {
            res = 50 + 50;  // Also 100
        }
    } catch (...) {
        res = 100;  // Same value
    }
    return res;
}
