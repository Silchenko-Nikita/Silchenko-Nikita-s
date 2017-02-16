package nikitos.structures;

import java.lang.reflect.Array;

// hometask
// open addressing
public class OAHashTable <K, V> {
    private int size = 0, capacity = 10;
    private double loadFactor = 0.75;
    private HashEntry table[];

    public OAHashTable() {
        size = 0;
        table = (HashEntry[]) Array.newInstance(HashEntry.class, capacity);
        //Arrays.fill(table, null);
    }

    public OAHashTable(int capacity) {
        this.capacity = capacity;
        table = (HashEntry[]) Array.newInstance(HashEntry.class, capacity);
    }

    public OAHashTable(int capacity, double loadFactor) {
        this(capacity);
        this.loadFactor = loadFactor;
    }

    private void rehash() {
        HashEntry oldTable[] = table;
        capacity *= 2;
        table = (HashEntry[]) Array.newInstance(HashEntry.class, capacity);
        for (HashEntry oldEntry : oldTable){
            if(oldEntry != null){
                put(oldEntry.key, oldEntry.value);
            }
        }
    }

    public boolean put(K k, V v){
        if (size > loadFactor*capacity) rehash();

        for (int i = hash(k), j = 0; j < capacity; i++, j++) {
            if(i == capacity) i = 0;

            if(table[i] == null || k.equals(table[i].key)) {
                table[i] = new HashEntry(k, v);

                size++;
                return true;
            }else if(table[i].del){
                table[i].key = k;
                table[i].value = v;
                table[i].del = true;

                size++;
                return true;
            }
        }
        return false;
    }

    public V get(K k){
        for (int i = hash(k), j = 0; j < capacity; i++, j++) {
            if(i == capacity) i = 0;
            if (table[i] == null) return null;
            if(table[i].del) continue;
            if(k.equals(table[i].key)){
                return table[i].value;
            }
        }
        return null;
    }

    public V remove(K k){
        for (int i = hash(k), j = 0; j < capacity; i++, j++) {
            if (i == capacity) i = 0;
            if (table[i] == null) return null;
            if(table[i].del) continue;
            if (k.equals(table[i].key)) {
                V ret = table[i].value;
                table[i].del = true;

                size--;
                return ret;
            }
        }
        return null;
    }

    public boolean contains(K k){
        for (int i = hash(k), j = 0; j < capacity; i++, j++) {
            if(i == capacity) i = 0;
            if (table[i] == null) return false;
            if(table[i].del) continue;
            if(k.equals(table[i].key)){
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
            table[i] = null;
        }
        size = 0;
    }

    private int hash(K k) {
        Integer kInt = (Integer) k;
        return kInt % capacity;
    }

    public void print(){
        for (int i = 0; i < table.length; i++) {
            if(table[i] != null && !table[i].del)
                System.out.println(i + ") key = " + table[i].key + ", value = " + table[i].value);
            else
                System.out.println(i + ") null");
        }
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