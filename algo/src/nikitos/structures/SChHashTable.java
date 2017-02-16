package nikitos.structures;

import java.lang.reflect.Array;
import java.util.LinkedList;

// hometask
// separate chaining
public class SChHashTable <K, V> {
    private int size = 0, capacity = 10;
    private double loadFactor = 0.75;
    private LinkedList<HashEntry> table[];

    public SChHashTable() {
        size = 0;
        table = (LinkedList<HashEntry>[]) Array.newInstance(LinkedList.class, capacity);
    }

    public SChHashTable(int capacity) {
        this.capacity = capacity;
        table = (LinkedList<HashEntry>[]) Array.newInstance(LinkedList.class, capacity);
    }

    public SChHashTable(int capacity, double loadFactor) {
        this(capacity);
        this.loadFactor = loadFactor;
    }


    public boolean put(K k, V v){
        int i = hash(k);
        if(table[i] == null){
            table[i] = new LinkedList<HashEntry>();
        }
        table[i].push(new HashEntry(k, v));
        size++;
        return true;
    }

    public V get(K k){
        int i = hash(k);
        if (table[i] == null) return null;
        for (HashEntry entry : table[i]) {
            if (entry.key.equals(k)){
                return entry.value;
            }
        }
        return null;
    }

    public V remove(K k){
        int i = hash(k);
        if (table[i] == null) return null;
        for (HashEntry entry : table[i]) {
            if (entry.key.equals(k)){
                V ret = entry.value;
                table[i].remove(entry);
                size--;
                return ret;
            }
        }
        return null;
    }

    public boolean contains(K k){
        int i = hash(k);
        if (table[i] == null) return false;
        for (HashEntry entry : table[i]) {
            if (entry.key.equals(k)){
                return true;
            }
        }
        return false;
    }

    public int size(){
        return size;
    }

    public boolean isEmpty(){
        return size == 0;
    }

    public void clear(){
        for (int i = 0; i < capacity; i++) {
            if(table[i] != null){
                table[i].clear();
            }
        }
        size = 0;
    }

    private int hash(K k) {
        Integer kInt = (Integer) k;
        return kInt % capacity;
    }

    private class HashEntry{
        K key = null;
        V value = null;
        boolean del = false;

        HashEntry(K key, V value) {
            this.key = key;
            this.value = value;
        }
    }
}