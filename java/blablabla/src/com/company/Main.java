package com.company;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class Main {

    public static void main(String[] args){
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                new Calculator();
            }
        });
    }
}

class Calculator extends JFrame implements ActionListener {
    Number calcLine = new Number();
    double buffer = 0;
    String operation = "";
    JTextField result;

    Calculator() {
        pack();
        setSize(300, 350);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        setLayout(null);
        result = new JTextField();
        result.setText(calcLine.toString());
        Insets ins = getInsets();
        result.setBounds(0, 0, getWidth() - ins.left - ins.right, 30);
        add(result);

        JPanel panel1 = new JPanel(new GridLayout(3, 4));
        JPanel panel2 = new JPanel(new GridLayout(2, 3));

        JButton zero = new JButton("0");
        JButton one = new JButton("1");
        JButton two = new JButton("2");
        JButton three = new JButton("3");
        JButton four = new JButton("4");
        JButton five = new JButton("5");
        JButton six = new JButton("6");
        JButton seven = new JButton("7");
        JButton eight = new JButton("8");
        JButton nine = new JButton("9");
        JButton plus = new JButton("+");
        JButton minus = new JButton("-");
        JButton multiply = new JButton("*");
        JButton division = new JButton("/");
        JButton equals = new JButton("=");
        JButton clear = new JButton("C");
        JButton point = new JButton(".");
        JButton change = new JButton("+/-");

        panel1.add(one);
        panel1.add(two);
        panel1.add(three);
        panel1.add(plus);
        panel1.add(four);
        panel1.add(five);
        panel1.add(six);
        panel1.add(minus);
        panel1.add(seven);
        panel1.add(eight);
        panel1.add(nine);
        panel1.add(multiply);
        panel1.add(zero);

        panel2.add(zero);
        panel2.add(division);
        panel2.add(point);
        panel2.add(change);
        panel2.add(equals);
        panel2.add(clear);

        panel1.setBounds(0, 30, getWidth() - ins.left - ins.right, getHeight() - 28 - ins.top - ins.bottom - 120);
        panel2.setBounds(0, getHeight() - 157, getWidth() - ins.left - ins.right, 120);
        getContentPane().add(panel1);
        getContentPane().add(panel2);

        zero.addActionListener(this);
        one.addActionListener(this);
        two.addActionListener(this);
        three.addActionListener(this);
        four.addActionListener(this);
        five.addActionListener(this);
        six.addActionListener(this);
        seven.addActionListener(this);
        eight.addActionListener(this);
        nine.addActionListener(this);
        plus.addActionListener(this);
        multiply.addActionListener(this);
        division.addActionListener(this);
        equals.addActionListener(this);
        clear.addActionListener(this);
        minus.addActionListener(this);
        point.addActionListener(this);
        change.addActionListener(this);

        setVisible(true);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        String value = e.getActionCommand();
        try {
            Integer.parseInt(value);
            calcLine.append(value);
        } catch (NumberFormatException ex) {
            if (value.equals(".")) calcLine.append(value);
            else {
                switch (value) {
                    case "C":
                        calcLine.setValue("0");
                        buffer = 0;
                        calcLine.ifPoint = false;
                        break;
                    case "+/-":
                        double line = Double.parseDouble(calcLine.toString());
                        if (line < 0) calcLine.setValue(calcLine.toString().substring(1));
                        else if (line == 0) break;
                        else calcLine.setValue("-" + calcLine.toString());
                        calcLine.process = false;
                        break;
                    case "=":
                        operate();
                        operation = "";
                        calcLine.ifPoint = false;
                        calcLine.process = true;
                        buffer = Double.parseDouble(calcLine.toString());
                        break;
                    case "+":
                    case "-":
                    case "*":
                    case "/":
                        if (calcLine.ifChanged)
                            operate();
                        operation = value;
                        calcLine.process = true;
                        calcLine.ifPoint = false;
                        buffer = Double.parseDouble(calcLine.toString());
                }
            }
        }
        result.setText(calcLine.toString());
    }

    void operate()
    {
        double line = Double.parseDouble(calcLine.toString());
        double rez;
        switch (operation) {
            case "+":
                rez = buffer + line;
                break;
            case "-":
                rez = buffer - line;
                break;
            case "*":
                rez = buffer * line;
                break;
            case "/":
                if (line != 0) rez = buffer / line;
                else rez = 0;
                break;
            default:
                rez = line;
        }
        calcLine.ifChanged = false;
        String sRez = String.valueOf(rez);
        if ((sRez.length() < 11 && !calcLine.ifPoint) || (sRez.length() < 12 && calcLine.ifPoint)) {
            if (rez == (int) rez) {
                calcLine.setValue(String.valueOf((int) rez));
            } else {
                calcLine.setValue(String.valueOf(rez));
            }
        }
    }
}

class Number {
    boolean ifPoint = false;
    boolean ifChanged = false;
    boolean process = false;
    private String value = "0";

    void append(String symbol){
        if ((ifPoint && value.length() == 11) || (!ifPoint && value.length() == 10)) return;
        if ((value.equals("0") && !symbol.equals("."))){
            process = false;
            if (!ifPoint && symbol.equals("0")) return;
            if (symbol.equals(".")) value = "0";
            else value = "";
        }

        if (process) {
            if (symbol.equals(".")) value = "0";
            else value = "";
            process = false;
        }

        if (symbol.equals(".")){
            if (ifPoint) return;
            else ifPoint = true;
        }else if (value.equals("0")) return;

        ifChanged = true;
        value = value + symbol;
    }

    void setValue(String value){
        this.value = value;
    }

    public String toString(){
        return value;
    }
}
