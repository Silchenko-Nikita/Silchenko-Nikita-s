package nikitos;

import nikitos.sorting.Sorting;
import nikitos.structures.*;
import nikitos.stuff.MyFactory;

import java.util.Arrays;
import java.util.Random;

public class Main {

    public static void main(String[] args) {
        /*BST<Integer, Integer> bst = new BST<>();
        bst.insert(3, 2);
        bst.insert(2, 2);
        bst.insert(1, 3);
        bst.insert(5, 4);
        bst.insert(6, 2);
        System.out.println(bst.getMin());*/


        BSTSingle<Integer> bst = new BSTSingle<>();
        bst.insert(3);
        bst.insert(2);
        bst.insert(1);
        bst.insert(5);
        bst.insert(6);
        System.out.println(bst.getMin());

        bst.clear();

        bst.insert(3);
        bst.insert(1);
        bst.insert(2);
        bst.insert(5);
        bst.insert(6);
        System.out.println(bst.getMin());

        bst.clear();

        bst.insert(3);
        bst.insert(5);
        bst.insert(4);
        bst.insert(6);
        System.out.println(bst.getMin());

        bst.clear();

        bst.insert(4);
        bst.insert(5);
        bst.insert(6);
        bst.insert(8);
        System.out.println(bst.getMin());

        bst.clear();

        bst.insert(5);
        bst.insert(4);
        bst.insert(2);
        bst.insert(3);
        System.out.println(bst.getMin());


        bst.clear();

        bst.insert(4);
        bst.insert(5);
        bst.insert(6);
        bst.insert(8);
        bst.insert(1);
        System.out.println(bst.getMin());

        bst.clear();

        System.out.println(bst.getMin());
    }
}






















        /*System.out.println("___Bubble sorting___\n");
        System.out.println("_Random_\n");
        for (int i = 1; i < 11; i++) {
            int arrLen = i*10_000;
            int arr[] = MyFactory.getRandomIntArr(arrLen, 0, arrLen / 2);

            long time1 = System.currentTimeMillis();
            Sorting.bubbleTwoDirectSort(arr);
            long time2 = System.currentTimeMillis();

            System.out.printf("Array of %d els:\n Sorting time: %d mls\n\n", arrLen, time2 - time1);
        }

        System.out.println("_Best case(sorted grOrder)_\n");
        for (int i = 1; i < 11; i++) {
            int arrLen = i*10_000;
            int arr[] = MyFactory.getSortedIntArr(arrLen, 0, arrLen, true);

            long time1 = System.currentTimeMillis();
            Sorting.bubbleTwoDirectSort(arr);
            long time2 = System.currentTimeMillis();

            System.out.printf("Array of %d els:\n Sorting time: %d mls\n\n", arrLen, time2 - time1);
        }

        System.out.println("_Worst case(sorted lsOrder)_\n");
        for (int i = 1; i < 11; i++) {
            int arrLen = i*10_000;
            int arr[] = MyFactory.getSortedIntArr(arrLen, 0, arrLen, false);

            long time1 = System.currentTimeMillis();
            Sorting.bubbleTwoDirectSort(arr);
            long time2 = System.currentTimeMillis();

            System.out.printf("Array of %d els:\n Sorting time: %d mls\n\n", arrLen, time2 - time1);
        }

        int arrLen = 100;
        int arr[] = MyFactory.getRandomIntArr(arrLen, 0, arrLen / 2);
        int arr1[] = Arrays.copyOf(arr, arrLen);

        for (int i = 0; i < arrLen; i++) {
            System.out.print(arr[i] + ", ");
        }

        System.out.println();
        Sorting.heapSort(arr);

        for (int i = 0; i < arrLen; i++) {
            System.out.print(arr[i] + ", ");
        }

        System.out.println("\n");

        for (int i = 0; i < arrLen; i++) {
            System.out.print(arr1[i] + ", ");
        }

        System.out.println();
        Sorting.quickSort(arr1);

        for (int i = 0; i < arrLen; i++) {
            System.out.print(arr1[i] + ", ");
        }

        System.out.println("___Heap sorting, Quick sorting___\n");

        System.out.println("_Random_\n");
        for (int i = 1; i < 2; i++) {
            int arrLen = i * 5000;
            int arr[] = MyFactory.getRandomIntArr(arrLen, 0, arrLen / 2);
            int arr1[] = Arrays.copyOf(arr, arrLen);

            long time1 = System.nanoTime();
            Sorting.heapSort(arr);
            long time2 = System.nanoTime();

            System.out.printf("Heap Sort (Array of %d els):\n Sorting time: %d mcs\n\n", arrLen, (time2 - time1) / 1000);

            time1 = System.nanoTime();
            Sorting.quickSort(arr1);
            time2 = System.nanoTime();

            System.out.printf("Quick Sort (Array of %d els):\n Sorting time: %d mcs\n\n", arrLen, (time2 - time1) / 1000);
        }

        System.out.println("_Best case(sorted grOrder)_\n");
        for (int i = 1; i < 8; i++) {
            int arrLen = i*5000;
            int arr[] = MyFactory.getSortedIntArr(arrLen, 0, arrLen / 2, true);
            int arr1[] = Arrays.copyOf(arr, arrLen);

            long time1 = System.nanoTime();
            Sorting.heapSort(arr);
            long time2 = System.nanoTime();

            System.out.printf("Heap Sort (Array of %d els):\n Sorting time: %d mcs\n\n", arrLen, (time2 - time1) / 1000);

            time1 = System.nanoTime();
            Sorting.quickSort(arr1);
            time2 = System.nanoTime();

            System.out.printf("Quick Sort (Array of %d els):\n Sorting time: %d mcs\n\n", arrLen, (time2 - time1) / 1000);
        }

        System.out.println("_Worst case(sorted lsOrder)_\n");
        for (int i = 1; i < 8; i++) {
            int arrLen = i*5000;
            int arr[] = MyFactory.getSortedIntArr(arrLen, 0, arrLen / 2, false);
            int arr1[] = Arrays.copyOf(arr, arrLen);

            long time1 = System.nanoTime();
            Sorting.heapSort(arr);
            long time2 = System.nanoTime();

            System.out.printf("Heap Sort (Array of %d els):\n Sorting time: %d mcs\n\n", arrLen, (time2 - time1) / 1000);

            time1 = System.nanoTime();
            Sorting.quickSort(arr1);
            time2 = System.nanoTime();

            System.out.printf("Quick Sort (Array of %d els):\n Sorting time: %d mcs\n\n", arrLen, (time2 - time1) / 1000);
        }

        System.out.println("___Removing from element separate chaining and open addressing hashtables___\n");
        Random rnd = new Random();
        OAHashTable<Integer, Integer> oaHashTable = new OAHashTable<>(10000);
        SChHashTable<Integer, Integer> sChHashTable = new SChHashTable<>(10000);
        for (int i = 1; i < 11; i++) {
            int htSize = i*10_000;
            for (int j = 0; j < htSize; j++) {
                int k = rnd.nextInt(htSize);
                int v = rnd.nextInt(htSize);
                oaHashTable.put(k, v);
                sChHashTable.put(k, v);
            }

            for (int j = 0; j < 1; j++) {
                int k = rnd.nextInt(htSize);

                long time1 = System.nanoTime();
                oaHashTable.remove(k);
                long time2 = System.nanoTime();

                System.out.printf("Open addressing hashTable of %d els(attempt %d):\n Getting time: %d nanos\n\n", htSize, j, time2 - time1);

                time1 = System.nanoTime();
                sChHashTable.remove(k);
                time2 = System.nanoTime();

                System.out.printf("Separate chaining hashTable of %d els(attempt %d):\n Getting time: %d nanos\n\n", htSize, j, time2 - time1);
            }

            System.out.println("__");

            oaHashTable.clear();
            sChHashTable.clear();

        }
    }
}*/
