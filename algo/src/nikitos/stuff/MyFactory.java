package nikitos.stuff;

import java.util.Random;

// hometask
public class MyFactory{
    static private Random rnd = new Random();

    public static int[] getRandomIntArr(int length, int lBound, int rBound){
        int[] arr = new int[length];
        return getRandomIntArr(arr, length, lBound, rBound);
    }

    public static int[] getSortedIntArr(int length, int lBound, int rBound, boolean grOrder) {
        int[] arr = new int[length];
        return getSortedIntArr(arr, length, lBound, rBound, grOrder);
    }

    public static int[] getRandomIntArr(int[] arr, int length, int lBound, int rBound){
        for (int i = 0; i < length; i++) {
            arr[i] = rnd.nextInt(rBound) + lBound;
        }
        return arr;
    }

    public static int[] getSortedIntArr(int[] arr, int length, int lBound, int rBound, boolean grOrder) {
        for (int i = 0; i < length; i++) {
            arr[i] = grOrder ? (lBound + (rBound - lBound)* i / length) : ((rBound - 1) - (rBound - lBound)* i / length);
        }
        return arr;
    }
}
