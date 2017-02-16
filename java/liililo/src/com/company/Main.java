package com.company;

import java.io.*;
import java.util.*;

public class Main{
    static String[][] desk = new String[5][5];
    static ArrayList<String> figures = new ArrayList<>();
    static ArrayList<int[]> listDesk = new ArrayList<>();
    static BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
    static boolean squares;

    public static void main(String[] argv) throws IOException {
        figures.add("0000");
        figures.add("0001");
        figures.add("0010");
        figures.add("0011");
        figures.add("0100");
        figures.add("0101");
        figures.add("0110");
        figures.add("0111");
        figures.add("1000");
        figures.add("1001");
        figures.add("1010");
        figures.add("1011");
        figures.add("1100");
        figures.add("1101");
        figures.add("1110");
        figures.add("1111");

        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                if (i == 0) desk[i][j] = ((j == 0) ? "Y\\X" : j) + "    ";
                else if (j == 0) desk[i][j] = i + "    ";
                else {
                    desk[i][j] = "Empty";
                    listDesk.add(new int[]{i, j});
                }
            }
        }
        System.out.println("Enter 1 to play with squares. Enter 0 to play without them");
        q: while (true) {
            String s = reader.readLine();
            switch (s){
                case "1":
                    squares = true;
                    break q;
                case "0":
                    squares = false;
                    break q;
                default:
                    System.out.println("Please, enter acceptable value");
            }
        }
        notification();
    }

    static void showDesk(){
        System.out.println(figures.size() == 16 ? "\n\n\nThe desk:\n" : "\n\n\n\n\n\nNew situation. The desk:\n");
        for (int i = 0; i < 5; i++) {
            for (int j = 0; j < 5; j++) {
                System.out.print(desk[i][j] + ((j == 0) ? "" : "    "));
            }
            System.out.println();
        }
    }

    static void notification(String...d) throws IOException {
        int a;
        showDesk();

        if (d.length == 0) {
            System.out.println("\nYou to give a figure to the partner. Chose one of following figures:\n");
            for (int i = 1; i < figures.size() + 1; i++) {
                System.out.print(((i == 1) ? "P" : "p") + "ress " + i + " to chose " + figures.get(i - 1) + ((i < figures.size()) ? ", " : "") + ((i % 4 == 0) ? "\n" : ""));
            }
            System.out.println((((figures.size() % 4 == 0)) ? "" : "\n") + "\nEnter the number");
            while (true) {
                try {
                    String s = reader.readLine();
                    a = Integer.parseInt(s) - 1;
                    if (!((a >= 0) && (a < figures.size()))) throw new Exception();
                    break;
                } catch (Exception e) {
                    System.out.println("Please, enter acceptable value");
                }
            }
            analise(a);
        } else {
            figures.remove(d[0]);
            System.out.println("\nYou has been given the figure " + d[0] + ". You to set the given figure on the desk. Enter coordinates\n");
            System.out.print("Residual figures: ");
            for (int i = 0; i < figures.size(); i++) {
                System.out.print(figures.get(i) + ((i < figures.size() - 1) ? ", " : ""));
            }
            System.out.println("\n\nEnter X and Y");
            while (true) {
                try {
                    int k2 = Integer.parseInt(reader.readLine());
                    int k1 = Integer.parseInt(reader.readLine());
                    if(!desk[k1][k2].equals("Empty")) throw new Exception();
                    else {
                        desk[k1][k2] = d[0] + " ";
                        if (k1 == k2)
                            t: for (int k = 0; k < 4; k++) {
                                for (int l = 1; l < 5; l++) {
                                    if (desk[l][l].equals("Empty")) break t;
                                    if (desk[l][l].charAt(k) != desk[k1][k2].charAt(k)) break;
                                    if (l == 4) {
                                        showDesk();
                                        System.out.println("\n\nYOU WIN!!!");
                                        return;
                                    }
                                }
                            }
                        if (k1 + k2 == 5)
                            t: for (int k = 0; k < 4; k++) {
                                for (int l = 1; l < 5; l++) {
                                    if (desk[l][5 - l].equals("Empty")) break t;
                                    if (desk[l][5 - l].charAt(k) != desk[k1][k2].charAt(k)) break;
                                    if (l == 4) {
                                        showDesk();
                                        System.out.println("\n\nYOU WIN!!!");
                                        return;
                                    }
                                }
                            }
                        if (squares) {
                            if (k1 < 4 && k2 < 4 && square(k1, k2)) {
                                showDesk();
                                System.out.println("\n\nYOU WIN!!!");
                                return;
                            }
                            if (k1 < 4 && k2 > 1 && square(k1, k2 - 1)) {
                                showDesk();
                                System.out.println("\n\nYOU WIN!!!");
                                return;
                            }
                            if (k1 > 1 && k2 < 4 && square(k1 - 1, k2)) {
                                showDesk();
                                System.out.println("\n\nYOU WIN!!!");
                                return;
                            }
                            if (k1 > 1 && k2 > 1 && square(k1 - 1, k2 - 1)) {
                                showDesk();
                                System.out.println("\n\nYOU WIN!!!");
                                return;
                            }
                        }

                        t: for (int k = 0; k < 4; k++) {
                            for (int l = 1; l < 5; l++) {
                                if (desk[k1][l].equals("Empty")) break t;
                                if (desk[k1][l].charAt(k) != desk[k1][k2].charAt(k)) break;
                                if (l == 4) {
                                    showDesk();
                                    System.out.println("\n\nYOU WIN!!!");
                                    return;
                                }
                            }
                        }
                        t: for (int k = 0; k < 4; k++) {
                            for (int l = 1; l < 5; l++) {
                                if (desk[l][k2].equals("Empty")) break t;
                                if (desk[l][k2].charAt(k) != desk[k1][k2].charAt(k)) break;
                                if (l == 4) {
                                    showDesk();
                                    System.out.println("\n\nYOU WIN!!!");
                                    return;
                                }
                            }
                        }
                    }
                    break;
                } catch (Exception e) {
                    System.out.println("Please, enter acceptable value");
                }
            }
            notification();
            }
        }

    static boolean square(int p1, int p2){
        for (int k = 0; k < 4; k++) {
            if (desk[p1][p2 + 1].equals("Empty")) return false;
            if (desk[p1][p2 + 1].charAt(k) != desk[p1][p2].charAt(k)) continue;
            if (desk[p1 + 1][p2].equals("Empty")) return false;
            if (desk[p1 + 1][p2].charAt(k) != desk[p1][p2].charAt(k)) continue;
            if (desk[p1 + 1][p2 + 1].equals("Empty")) return false;
            if (desk[p1 + 1][p2 + 1].charAt(k) != desk[p1][p2].charAt(k)) continue;
            return true;
            }
        return false;
        }

    static  void analise(int a) throws IOException {
        Collections.shuffle(listDesk);
        for (int[] aListDesk : listDesk) {
            if (!desk[aListDesk[0]][aListDesk[1]].equals("Empty")) continue;
            desk[aListDesk[0]][aListDesk[1]] = figures.get(a) + " ";
            if (aListDesk[0] == aListDesk[1])
                t: for (int k = 0; k < 4; k++) {
                    for (int l = 1; l < 5; l++) {
                        if (desk[l][l].equals("Empty")) break t;
                        if (desk[l][l].charAt(k) != desk[aListDesk[0]][aListDesk[1]].charAt(k)) break;
                        if (l == 4) {
                            showDesk();
                            System.out.println("\n\nYOU LOSE!!!");
                            return;
                        }
                    }
                }
            if (aListDesk[0] + aListDesk[1] == 5)
                t: for (int k = 0; k < 4; k++) {
                    for (int l = 1; l < 5; l++) {
                        if (desk[l][5 - l].equals("Empty")) break t;
                        if (desk[l][5 - l].charAt(k) != desk[aListDesk[0]][aListDesk[1]].charAt(k)) break;
                        if (l == 4) {
                            showDesk();
                            System.out.println("\n\nYOU LOSE!!!");
                            return;
                        }
                    }
                }
            if (squares) {
                if (aListDesk[0] < 4 && aListDesk[1] < 4 && square(aListDesk[0], aListDesk[1])) {
                    showDesk();
                    System.out.println("\n\nYOU LOSE!!!");
                    return;
                }
                if (aListDesk[0] < 4 && aListDesk[1] > 1 && square(aListDesk[0], aListDesk[1] - 1)) {
                    showDesk();
                    System.out.println("\n\nYOU LOSE!!!");
                    return;
                }
                if (aListDesk[0] > 1 && aListDesk[1] < 4 && square(aListDesk[0] - 1, aListDesk[1])) {
                    showDesk();
                    System.out.println("\n\nYOU LOSE!!!");
                    return;
                }
                if (aListDesk[0] > 1 && aListDesk[1] > 1 && square(aListDesk[0] - 1, aListDesk[1] - 1)) {
                    showDesk();
                    System.out.println("\n\nYOU LOSE!!!");
                    return;
                }
            }

            t: for (int k = 0; k < 4; k++) {
                for (int l = 1; l < 5; l++) {
                    if (desk[aListDesk[0]][l].equals("Empty")) break t;
                    if (desk[aListDesk[0]][l].charAt(k) != desk[aListDesk[0]][aListDesk[1]].charAt(k)) break;
                    if (l == 4) {
                        showDesk();
                        System.out.println("\n\nYOU LOSE!!!");
                        return;
                    }
                }
            }
            t: for (int k = 0; k < 4; k++) {
                for (int l = 1; l < 5; l++) {
                    if (desk[l][aListDesk[1]].equals("Empty")) break t;
                    if (desk[l][aListDesk[1]].charAt(k) != desk[aListDesk[0]][aListDesk[1]].charAt(k)) break;
                    if (l == 4) {
                        showDesk();
                        System.out.println("\n\nYOU LOSE!!!");
                        return;
                    }
                }
            }
            desk[aListDesk[0]][aListDesk[1]] = "Empty";
        }
        for (int[] aListDesk : listDesk) {
            if (!desk[aListDesk[0]][aListDesk[1]].equals("Empty")) continue;
            desk[aListDesk[0]][aListDesk[1]] = figures.get(a) + " ";
            break;
        }
            figures.remove(a);

        if (figures.isEmpty()){
            showDesk();
            System.out.println("\n\nDRAW!!!");
            return;
        }
        ArrayList<String> figures1 = new ArrayList<>(figures);
        Collections.shuffle(figures1);
        h:
        for (String aFigures1 : figures1) {
            for (int i = 1; i < 5; i++) {
                for (int j = 1; j < 5; j++) {
                    if (desk[i][j].equals("Empty")) {
                        desk[i][j] = aFigures1;
                        if (i == j)
                            t: for (int k = 0; k < 4; k++) {
                                for (int l = 1; l < 5; l++) {
                                    if (desk[l][l].equals("Empty")) break t;
                                    if (desk[l][l].charAt(k) != aFigures1.charAt(k)) break;
                                    if (l == 4) {
                                        desk[i][j] = "Empty";
                                        continue h;
                                    }
                                }
                            }
                        if (i + j == 5)
                            t: for (int k = 0; k < 4; k++) {
                                for (int l = 1; l < 5; l++) {
                                    if (desk[l][5 - l].equals("Empty")) break t;
                                    if (desk[l][5 - l].charAt(k) != aFigures1.charAt(k)) break;
                                    if (l == 4) {
                                        desk[i][j] = "Empty";
                                        continue h;
                                    }
                                }
                            }
                        if (squares) {
                            if (i < 4 && j < 4 && square(i, j)) {
                                desk[i][j] = "Empty";
                                continue h;
                            }
                            if (i < 4 && j > 1 && square(i, j - 1)) {
                                desk[i][j] = "Empty";
                                continue h;
                            }
                            if (i > 1 && j < 4 && square(i - 1, j)) {
                                desk[i][j] = "Empty";
                                continue h;
                            }
                            if (i > 1 && j > 1 && square(i - 1, j - 1)) {
                                desk[i][j] = "Empty";
                                continue h;
                            }
                        }
                        t: for (int k = 0; k < 4; k++) {
                            for (int l = 1; l < 5; l++) {
                                if (desk[i][l].equals("Empty")) break t;
                                if (desk[i][l].charAt(k) != aFigures1.charAt(k)) break;
                                if (l == 4) {
                                    desk[i][j] = "Empty";
                                    continue h;
                                }
                            }
                        }
                        t: for (int k = 0; k < 4; k++) {
                            for (int l = 1; l < 5; l++) {
                                if (desk[l][j].equals("Empty")) break t;
                                if (desk[l][j].charAt(k) != aFigures1.charAt(k)) break;
                                if (l == 4){
                                    desk[i][j] = "Empty";
                                    continue h;
                                }
                            }
                        }
                        desk[i][j] = "Empty";
                    }
                }
            }
            notification(aFigures1);
            return;
        }
        notification(figures1.get(0));
    }
}