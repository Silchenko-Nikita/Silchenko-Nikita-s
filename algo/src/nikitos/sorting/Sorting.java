package nikitos.sorting;

import nikitos.structures.Heap;

import java.util.Arrays;

import static nikitos.stuff.MyUtils.swap;

public class Sorting {

    public static void shellSort(Comparable[] ar) {
        for (int gap = ar.length / 2; gap > 0; gap /= 2) {
            for (int i = gap; i < ar.length; i++) {
                Comparable tmp = ar[i];
                int j = i;
                for (; j >= gap && tmp.compareTo(ar[j - gap]) < 0; j -= gap) {
                    ar[j] = ar[j - gap];
                }
                ar[j] = tmp;
            }
        }
    }

    // hometask
    public static void bubbleTwoDirectSort(int[] arr) {
        int l = 0;
        int r = arr.length - 1;

        do{
            for (int i = 0; i < r; i++) {
                if(arr[i + 1] < arr[i]){
                    swap(arr, i, i + 1);
                }
            }
            r--;
            for (int i = r; i > l; i--) {
                if(arr[i] < arr[i -  1]){
                    swap(arr, i, i - 1);
                }
            }
            l++;
        }while(l < r);
    }

    // hometask
    private static int partition(int[] arr, int l, int h){
        int piv = arr[l];
        int i = l;
        int j = h + 1;

        while(true){
            do{
                i++;
            }while (i <= h && arr[i] < piv);

            do{
                j--;
            }while (j >= l && arr[j] > piv);

            if (i < j){
                swap(arr, i, j);
            }else{
                break;
            }
        }
        swap(arr, l, j);

        return j;
    }

    // hometask
    public static void quickSort(int[] arr, int l, int h) {
        if(l >= h) return;

        int q = partition(arr, l, h);
        quickSort(arr, l, q - 1);
        quickSort(arr, q + 1, h);
    }

    // hometask
    private static void sink(int[] arr, int i, int size){
        int l = i * 2 + 1;
        int r = i * 2 + 2;

        int largest = (l < size && arr[l] > arr[i]) ? l : i;
        if (r < size && arr[r] > arr[largest]){
            largest = r;
        }

        if  (largest != i) {
            swap(arr, i, largest);
            sink(arr, largest, size);
        }
    }

    // hometask
    private static void buildHeap(int[] arr){
        for (int i = (arr.length-1)/2; i >= 0; i--) {
            sink(arr, i, arr.length);
        }
    }

    // hometask
    public static void heapSort(int[] arr) {
        buildHeap(arr);

        for (int i = arr.length - 1; i > 0; i--) {
            swap(arr, 0, i);
            sink(arr, 0, i);
        }
    }

    // hometask
    public static void quickSort(int[] arr){
        quickSort(arr, 0, arr.length - 1);
    }

    public static ConvertableToInt[] countingSort(ConvertableToInt[] ar) {
        int minKey = ar[0].toInt(), maxKey = ar[0].toInt();
        for (int i = 0; i < ar.length; i++) {
            if(ar[i].toInt() < minKey) minKey = ar[i].toInt();
            if(ar[i].toInt() > maxKey) maxKey = ar[i].toInt();
        }

        int count[] = new int[maxKey - minKey + 1];
        ConvertableToInt[] sortedAr = new ConvertableToInt[ar.length];

        for(int i = 0; i < ar.length; i++){
            count[ar[i].toInt() - minKey]++;
        }

        for(int i = 1; i < count.length; i++){
            count[i] += count[i - 1];
        }

        for (int i = ar.length - 1; i >= 0; i--) {
            int key = ar[i].toInt() - minKey;

            count[key]--;
            sortedAr[count[key]] = ar[i];
        }

        return sortedAr;
    }
}
