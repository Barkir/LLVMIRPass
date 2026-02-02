// Test 6: Switch Statement (Non-branch terminator)
// The pass checks for 'br' and 'select' terminators. It likely ignores or crashes on switch.
// If it treats switch as unconditional fall-through, it will merge paths incorrectly.
int test_switch_terminator(int a) {
    int res;
    switch (a) {
        case 100:
            res = 1000;
            break;
        case 200:
            res = 1000;
            break;
        default:
            res = (a == 100) ? 1000 : 1000;  // Complex computation matching above
    }
    return res;
}
