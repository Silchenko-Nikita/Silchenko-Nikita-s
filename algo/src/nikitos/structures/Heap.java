package nikitos.structures;

import java.util.Arrays;

import static nikitos.stuff.MyUtils.swap;

// hometask - ints
public class Heap {
    /*private int arr[];
    private int size;

    public Heap(int arr[]){
        this.arr = new int[arr.length + 1];
        System.arraycopy(arr, 0, this.arr, 1, arr.length);

        size = arr.length;

        for (int i = (size - 1)/2; i > 0; i--) {
            sink(i);
        }

        for (int i = 1; i < this.arr.length; i++) {
            System.out.print(this.arr[i] + ", ");
        }
        System.out.println();
    }

    private void sink(int i){
        int l = i * 2;
        int r = l * 2 + 1;

        int largest = (l <= size && arr[l] > arr[i]) ? l : i;
        if (r <= size && arr[r] > arr[largest]){
            largest = r;
        }

        if  (largest != i) {
            swap(arr, i, largest);
            sink(largest);
        }
    }

    public void sort(){
        for (int i = size; i > 0; i--) {
            swap(arr, i, 1);
            size--;
            sink(1);
        }

        for (int i = 1; i < this.arr.length; i++) {
            System.out.print(this.arr[i] + ", ");
        }
        System.out.println();
    }

    public int[] toArray(int arr[]){
        System.arraycopy(this.arr, 1, arr, 0, size);
        return arr;
    }

    public int[] toArray(){
        return toArray(new int[size]);
    }
    */ // unlucky attempt
}
