/* For both rolling averages, do not make the window bigger than the array of vals passed in */
float rollingAvgFloat(float *vals, int windowSize) {
    int i = 0;
    float total = 0;
    for (i = 0; i < windowSize; i++) 
        total += vals[i];
    return total / (float) windowSize;
}

/* Debatable whether this is worth keeping, all it really does is truncates */
int rollingAvgInt(int *vals, int windowSize) {
    int i = 0, total = 0;
    for (i = 0; i < windowSize; i++)
        total += vals[i];
    return total / windowSize;
}


/* Weight must be < 1 for both filters */
float expFilterFloat(float currVal, float prevVal, float weight) {
    float term1 = weight * currVal;
    float term2 = (1.0 - weight) * prevVal;
    
    return (term1 + term2);
}

int expFilterInt(int currVal, int prevVal, float weight) {
    int term1 = weight * currVal;
    int term2 = (1.0 - weight) * prevVal;
    
    return (term1 + term2);
}
