package nikitos.structures;

public class ArrayList {
    private static final int defaultCapacity = 8;

    private String[] array;
    private int size;
    private int capacity;

    public ArrayList(){
        size = 0;
        capacity = defaultCapacity;
        array = new String[capacity];
    }

    public void add(int index, String el) throws IllegalArgumentException, IndexOutOfBoundsException{
        if (!elIsValid(el)){
            throw new IllegalArgumentException();
        }
        if (index < 0 || index > size){
            throw new IndexOutOfBoundsException();
        }
        if(size < capacity){
            System.arraycopy(array, index, array, index + 1, size - index);
        }else {
            String[] oldArray = array;
            capacity *= 2;
            array = new String[capacity];

            System.arraycopy(oldArray, 0, array, 0, index);
            System.arraycopy(oldArray, 0, array, index + 1, size - index);
        }
        array[index] = el;
        size++;
    }

    public String get(int index) throws IndexOutOfBoundsException{
        if (index < 0 || index >= size){
            throw new IndexOutOfBoundsException();
        }
        return array[index];
    }

    public void set(int index, String el) throws IllegalArgumentException, IndexOutOfBoundsException{
        if (!elIsValid(el)){
            throw new IllegalArgumentException();
        }
        if (index < 0 || index >= size){
            throw new IndexOutOfBoundsException();
        }
        array[index] = el;
    }

    public String remove (int index) throws IndexOutOfBoundsException{
        if (index < 0 || index >= size){
            throw new IndexOutOfBoundsException();
        }
        String el = array[index];
        System.arraycopy(array, index + 1, array, index, size - index);
        size--;
        return el;
    }

    public int size(){
        return size;
    }

    static private boolean elIsValid(String el){
        return el.matches("[1-9A-Fa-f][0-9A-Fa-f]*");
    }
}
