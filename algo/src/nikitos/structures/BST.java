package nikitos.structures;

import nikitos.stuff.Gender;
import nikitos.stuff.Student;

import java.util.LinkedList;
import java.util.Stack;


public class BST <K extends Comparable, V>{
    private TreeNode root;
    private int size;

    public BST() {
        root = null;
        size = 0;
    }

    public boolean containsKey(K key) {

        return getNode(key) != null;
    }

    public void traverse() {
        inorder(root);
    }

    private void inorder(TreeNode node){
        if(node == null) return;
        inorder(node.left);
        System.out.println("key: " + node.key + ", val: " + node.val);
        inorder(node.right);
    }

    public V insert(K key, V value) {
        size++;

        if(root == null){
            root = new TreeNode(key, value);
            return null;
        }

        TreeNode curNode = root;
        while (key.compareTo(curNode.key) != 0){
            if(key.compareTo(curNode.key) < 0){
                if(curNode.left == null){
                    curNode.left = new TreeNode(key, value, curNode);
                    return null;
                }

                curNode = curNode.left;
            }else{
                if(curNode.right == null){
                    curNode.right = new TreeNode(key, value, curNode);
                    return null;
                }

                curNode = curNode.right;
            }
        }

        V prevVal = curNode.val;
        curNode.val = value;

        return  prevVal;
    }

    public int size() {
        return size;
    }

    private TreeNode getNode(K key){
        TreeNode curNode = root;
        while (key.compareTo(curNode.key) != 0){
            if(key.compareTo(curNode.key) < 0){
                curNode = curNode.left;

            }else{
                curNode = curNode.right;
            }

            if (curNode == null) return null;
        }
        return curNode;
    }

    public V get(K key) {
        TreeNode node = getNode(key);
        if(node == null){
            return null;
        }else{
            return node.val;
        }
    }

    private TreeNode getMinNode() {
        TreeNode node = root;
        while(node.left != null){
            node = node.left;
        }
        return node;
    }

    public V getMin() {
        return getMinNode().val;
    }


    public V remove(K key) {
        TreeNode node = getNode(key);
        if(node == null) return null;
        V ret = node.val;
        remove(node);
        return ret;
    }

    private void remove(TreeNode toDel) {
        if(toDel == null) return;

        if(toDel.left != null && toDel.right != null){
            TreeNode successor = toDel.right;

            while(successor.left != null){
                successor = successor.left;
            }

            toDel.key = successor.key;
            toDel.val = successor.val;

            remove(successor);

        }else if(toDel == root){
            if(toDel.left != null){
                root = toDel.left;
                toDel.left.parent = null;
            }else{
                root = toDel.right;
                toDel.right.parent = null;
            }
            size--;
        }else if (toDel.parent.left == toDel){
            toDel.parent.setLeft((toDel.left != null) ? toDel.left : toDel.right);
            size--;
        }else if (toDel.parent.right == toDel) {
            toDel.parent.setRight((toDel.left != null) ? toDel.left : toDel.right);
            size--;
        }
    }

    private boolean toRemoveStudent(Student student, String ... removeCriteria){
        for (String crit : removeCriteria){
            if((crit.equals("male") && (student.getGender() == Gender.FEMALE)) ||
                    (crit.equals("female") && (student.getGender() == Gender.MALE)) ||
                    (crit.matches("grade: [1-6]") && (student.getGrade() != (crit.charAt(crit.lastIndexOf("grade: ") + "grade: ".length()) - '0'))) ||
                    (crit.equals("married") && !student.isMarried()) ||
                    (crit.equals("not married") && student.isMarried())){
                    return false;
            }
        }
        return true;
    }

    private int remove(TreeNode base, String ... removeCriteria){
        if(base == null) return 0;
        int removes = 0;

        removes += remove(base.left, removeCriteria);
        removes += remove(base.right, removeCriteria);

        if(toRemoveStudent((Student)base.val, removeCriteria)){
            remove(base);
            removes++;
        }

        return removes;
    }

    public int remove(String ... removeCriteria) {
        return remove(root, removeCriteria);
    }

    private int traverseSize(TreeNode node){
        if(node == null) return 0;
        int size = 0;
        size += traverseSize(node.left);
        size += traverseSize(node.right);
        return size;
    }

    public int traverseSize() {
        return traverseSize(root);
    }

    private class TreeNode {
        K key;
        V val;

        TreeNode right = null;
        TreeNode left = null;
        TreeNode parent = null;

        public TreeNode(K key, V val) {
            this.key = key;
            this.val = val;
        }

        public TreeNode(K key, V val, TreeNode parent) {
            this(key, val);
            this.parent = parent;
        }

        public void setLeft(TreeNode node){
            left = node;
            if(node != null) node.parent = this;
        }

        public void setRight(TreeNode node){
            right = node;
            if(node != null) node.parent = this;
        }
    }
}


