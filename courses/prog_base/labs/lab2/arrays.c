void fillRand3(int arr[], int size){
    int i;
    for (i = 0; i < size; i++){
        arr[i] = (rand()*2)/(RAND_MAX + 1);
    }
}

int checkRand3(int arr[], int size){
    int i, k = 1;
    for (i = 0; i < size; i++){
        if (!(arr[i] == 0 || arr[i] == 1)){
            k = 0;
            break;
        }
    }
    return k;
}

int maxValue(int arr[], int size){
    int i, max = arr[0];
    for (i = 1; i < size; i++){
        if (arr[i] > max) max = arr[i];
    }
    return max;
}

int maxIndex(int arr[], int size){
    int i, max = arr[0], maxInd = 0;
    for (i = 1; i < size; i++){
        if (arr[i] > max){
            max = arr[i];
            maxInd = i;
        }
    }
    return maxInd;
}

int maxOccurance(int arr[], int size){
    int i, j, maxOcVal = arr[0], maxOc = 1, currentCount;
    for (i = 0; i < size; i++){
        currentCount = 1;
        for (j = i + 1; j < size; j++){
            if (arr[j] == arr[i]){
                currentCount++;
            }
        }
        if ((currentCount > maxOc) || (currentCount == maxOc && arr[i] > maxOcVal)){
            maxOcVal = arr[i];
            maxOc = currentCount;
        }
    }
    return maxOc;
}

int diff(int arr1[], int arr2[], int res[], int size){
    int i, k = 1;
    for (i = 0; i < size; i++){
        res[i] = arr1[i] - arr2[i];
        if (res[i]) k = 0;
    }
    return k;
}

void sub(int arr1[], int arr2[], int res[], int size){
    int i;
    for (i = 0; i < size; i++){
        res[i] = arr1[i] - arr2[i];
    }
}

int eq(int arr1[], int arr2[], int size){
    int i, k = 1;
    for (i = 0; i < size; i++){
        if (arr1[i] != arr2[i]){
            k = 0;
            break;
        }
    }
    return k;
}

void lor(int arr1[], int arr2[], int res[], int size){
    int i;
    for (i = 0; i < size; i++){
        res[i] = arr1[i] || arr2[i];
    }
}
