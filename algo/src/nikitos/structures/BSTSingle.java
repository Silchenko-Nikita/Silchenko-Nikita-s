package nikitos.structures;

public class BSTSingle <K extends Comparable>{
    private TreeNode root;
    private int size;

    public BSTSingle() {
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
        System.out.println("key: " + node.key);
        inorder(node.right);
    }

    public K insert(K key) {
        size++;

        if(root == null){
            root = new TreeNode(key);
            return null;
        }

        TreeNode curNode = root;
        while (key.compareTo(curNode.key) != 0){
            if(key.compareTo(curNode.key) < 0){
                if(curNode.left == null){
                    curNode.left = new TreeNode(key, curNode);
                    return null;
                }

                curNode = curNode.left;
            }else{
                if(curNode.right == null){
                    curNode.right = new TreeNode(key, curNode);
                    return null;
                }

                curNode = curNode.right;
            }
        }

        K prevK = curNode.key;
        curNode.key = key;

        return  prevK;
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

    private TreeNode getMinNode() {
        if(root == null) return null;
        TreeNode node = root;
        while(node.left != null){
            node = node.left;
        }
        return node;
    }

    public K getMin() {
        TreeNode node = getMinNode();
        if(node == null) return null;
        return getMinNode().key;
    }


    public K remove(K key) {
        TreeNode node = getNode(key);
        if(node == null) return null;
        K ret = node.key;
        remove(node);
        return ret;
    }

    private void remove(TreeNode toDel) {
        if (toDel == null) return;

        if (toDel.left != null && toDel.right != null) {
            TreeNode successor = toDel.right;

            while (successor.left != null) {
                successor = successor.left;
            }

            toDel.key = successor.key;

            remove(successor);

        } else if (toDel == root) {
            if (toDel.left != null) {
                root = toDel.left;
                toDel.left.parent = null;
            } else {
                root = toDel.right;
                toDel.right.parent = null;
            }
            size--;
        } else if (toDel.parent.left == toDel) {
            toDel.parent.setLeft((toDel.left != null) ? toDel.left : toDel.right);
            size--;
        } else if (toDel.parent.right == toDel) {
            toDel.parent.setRight((toDel.left != null) ? toDel.left : toDel.right);
            size--;
        }
    }


    private int traverseSize(TreeNode node){
        if(node == null) return 0;
        int size = 0;
        size += traverseSize(node.left);
        size += traverseSize(node.right);
        return size;
    }

    public void clear(){
        root = null;
        size = 0;
    }

    public int traverseSize() {
        return traverseSize(root);
    }

    private class TreeNode {
        K key;

        TreeNode right = null;
        TreeNode left = null;
        TreeNode parent = null;

        public TreeNode(K key) {
            this.key = key;
        }

        public TreeNode(K key, TreeNode parent) {
            this(key);
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
