package nikitos.structures;

import java.util.EmptyStackException;

public class LinkedStack {
    private Node top;

    public LinkedStack(){
        top = null;
    }

    public void push(int el){
        top = new Node(el, top);
    }

    public int pop() throws EmptyStackException{
        if (isEmpty()){
            throw new EmptyStackException();
        }
        int val = top.val;
        Node next = top.next;
        top.next = null;
        top = next;
        return val;
    }

    public int peek() throws EmptyStackException{
        if (isEmpty()){
            throw new EmptyStackException();
        }
        return top.val;
    }

    public void clear(){
        while(!isEmpty()){
            pop();
        }
    }

    public boolean isEmpty(){
        return top == null;
    }

    private class Node{
        Node next;
        int val;

        Node(int val, Node next){
            this.val = val;
            this.next = next;
        }
    }
}
