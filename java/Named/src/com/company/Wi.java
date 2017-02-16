package com.company;

import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import java.util.*;


class Wi extends JFrame {
    Color field[][];
    Figure currentFigure, nextFigure;
    int width, height, blockSide, sleepingTime, bufferedSleepingTime, startSleepingTime, score, level;
    MyPane panel;
    MyPane1 panel1;
    MyPane2 panel2;
    Image image;
    boolean stopGame = false, alreadyPressed = false;
    Insets insets;
    JLabel scoreLabel, levelLabel;

    Wi() {
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        setResizable(false);
        setLayout(null);
        startSleepingTime = 300;
        bufferedSleepingTime = startSleepingTime;
        sleepingTime = startSleepingTime;
        score = 0;
        level = 0;
        width = 10;
        height = 20;
        blockSide = 15;
        field = new Color[width][height];
        for (int i = 0; i < width; i++) {
            Arrays.fill(field[i], Color.WHITE);
        }
        insets = getInsets();
        panel = new MyPane();
        panel1 = new MyPane1();
        add(panel);
        add(panel1);
        setSize(insets.left + insets.right + panel1.getWidth() + panel.getWidth(), 29 + panel.getHeight());
        setLocationRelativeTo(null);
        currentFigure = new Figure();
        for (int i = 0; i < 4; i++) {
            field[currentFigure.figure[i][0]][currentFigure.figure[i][1]] = currentFigure.figureColor;
        }
        addKeyListener(new MyKeyAdapter());
        nextFigure = new Figure();
        setVisible(true);
        Thread tr = new Thread() {
            @Override
            public void run() {
                try {
                    Thread.sleep(sleepingTime);
                    panel.repaint();
                } catch (InterruptedException ignored) {
                }

                while (true) {
                    try {
                        currentFigure.update("down");
                    } catch (Exception e) {
                        currentFigure = nextFigure;
                        for (int i = 0; i < 4; i++) {
                            if (!field[currentFigure.figure[i][0]][currentFigure.figure[i][1]].equals(Color.WHITE))
                                stopGame = true;
                        }

                        for (int i = 0; i < 4; i++) {
                            field[currentFigure.figure[i][0]][currentFigure.figure[i][1]] = currentFigure.figureColor;
                        }
                        nextFigure = new Figure();
                        panel2.repaint();
                        ifLine();
                        if (stopGame) break;
                    }
                    try {
                        Thread.sleep(sleepingTime);
                    } catch (InterruptedException ignored) {
                    }
                    panel.repaint();
                }
            }
        };
        tr.start();
    }


    class MyKeyAdapter extends KeyAdapter {

        @Override
        public void keyPressed(KeyEvent e) {
            if (alreadyPressed || stopGame) return;
            alreadyPressed = true;
            switch (e.getKeyCode()) {
                case KeyEvent.VK_UP:
                    currentFigure.nextState();
                    panel.repaint();
                    break;
                case KeyEvent.VK_LEFT:
                    try {
                        currentFigure.update("left");
                        panel.repaint();
                    } catch (Exception ignored) {
                    }
                    break;
                case KeyEvent.VK_RIGHT:
                    try {
                        currentFigure.update("right");
                        panel.repaint();
                    } catch (Exception ignored) {
                    }
                    break;
                case KeyEvent.VK_DOWN:
                    sleepingTime = 40;
                    panel.repaint();
            }
        }

        @Override
        public void keyReleased(KeyEvent e) {
            if (e.getKeyCode() == KeyEvent.VK_DOWN) sleepingTime = bufferedSleepingTime;
        }
    }

    void ifLine() {
        q:
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (field[j][i].equals(Color.WHITE)) continue q;
            }
            for (int w = 0; w < 4; w++) {
                field[currentFigure.figure[w][0]][currentFigure.figure[w][1]] = Color.WHITE;
            }
            for (int j = i; j > 0; j--) {
                for (int k = 0; k < width; k++) {
                    field[k][j] = field[k][j - 1];
                }
            }
            for (int w = 0; w < 4; w++) {
                field[currentFigure.figure[w][0]][currentFigure.figure[w][1]] = currentFigure.figureColor;
            }
            if (++score == 10) {
                score = 0;
                level++;
                bufferedSleepingTime = (int) (bufferedSleepingTime * 0.9);
                sleepingTime = bufferedSleepingTime;
            }
            scoreLabel.setText(String.valueOf(score));
            levelLabel.setText(String.valueOf(level));
        }
    }

    class MyPane extends JPanel {

        MyPane() {
            setBounds(insets.left, insets.top, blockSide * width, blockSide * height);
        }

        @Override
        public void paintComponent(Graphics g) {
            super.paintComponent(g);
            image = createImage(getWidth(), getHeight());
            Graphics g1 = image.getGraphics();
            for (int i = 0; i < field.length; i++) {
                for (int j = 0; j < field[i].length; j++) {
                    g1.setColor(field[i][j]);
                    g1.fillRect(i * blockSide + 1, j * blockSide + 1, blockSide - 1, blockSide - 1);
                }
            }
            g.drawImage(image, 0, 0, null);
            alreadyPressed = false;
        }
    }

    class MyPane1 extends JPanel {
        MyPane1() {
            setBackground(Color.LIGHT_GRAY);
            setLayout(null);
            setBounds(insets.left + panel.getWidth(), insets.top, 100, panel.getHeight());

            String sScore = String.valueOf(score);
            scoreLabel = new JLabel(sScore);
            JLabel scoreLabelAnnounce = new JLabel("Lines:");
            FontMetrics fM = scoreLabel.getFontMetrics(scoreLabel.getFont());
            scoreLabelAnnounce.setBounds((getWidth() - fM.stringWidth("Lines:")) / 2, 50 - fM.getHeight(), fM.stringWidth("Lines:"), fM.getHeight());
            scoreLabel.setBounds((getWidth() - fM.stringWidth(sScore)) / 2, 50, fM.stringWidth(sScore), fM.getHeight());
            add(scoreLabelAnnounce);
            add(scoreLabel);

            String sLevel = String.valueOf(level);
            levelLabel = new JLabel(sLevel);
            JLabel levelLabelAnnounce = new JLabel("Level:");
            levelLabelAnnounce.setBounds((getWidth() - fM.stringWidth("Level:")) / 2, 200 - fM.getHeight(), fM.stringWidth("Level:"), fM.getHeight());
            levelLabel.setBounds((getWidth() - fM.stringWidth(sLevel)) / 2, 200, fM.stringWidth(sScore), fM.getHeight());
            add(levelLabelAnnounce);
            add(levelLabel);

            JLabel nextFigureLabel = new JLabel("NextFigure:");
            nextFigureLabel.setBounds((getWidth() - fM.stringWidth("NextFigure:")) / 2, 75, fM.stringWidth("NextFigure:"), fM.getHeight());
            panel2 = new MyPane2();
            panel2.setBounds(getWidth() / 2 - 2 * blockSide, 75 + fM.getHeight(), 4 * blockSide, 4 * blockSide);
            add(panel2);
            add(nextFigureLabel);
        }
    }

    class MyPane2 extends JPanel {
        Image image;

        @Override
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);
            image = createImage(getWidth(), getHeight());
            Graphics g1 = image.getGraphics();
            for (int i = 0; i < 4; i++) {
                g1.setColor(Color.WHITE);
                for (int j = 0; j < 4; j++) {
                    g1.fillRect(i * blockSide + 1, j * blockSide + 1, blockSide - 1, blockSide - 1);
                }
            }
            for (int i = 0; i < 4; i++) {
                g1.setColor(nextFigure.figureColor);
                g1.fillRect((nextFigure.figure[i][0] - (width / 2 - 2)) * blockSide + 1, (nextFigure.figure[i][1] + 1) * blockSide + 1, blockSide - 1, blockSide - 1);
            }
            g.drawImage(image, 0, 0, null);
        }
    }

    class Figure {
        Color figureColor;
        ArrayList<int[][]> state = new ArrayList<>();
        int[][] figure = new int[4][2];
        ListIterator<int[][]> iterator;

        void update(String s) throws Exception {
            switch (s) {
                case "down":
                    k:
                    for (int i = 0; i < 4; i++) {
                        int[] l = {figure[i][0], figure[i][1] + 1};
                        for (int j = 0; j < 4; j++) {
                            if (figure[j][0] == l[0] && figure[j][1] == l[1]) continue k;
                        }
                        if (!field[l[0]][l[1]].equals(Color.WHITE)) throw new Exception();
                    }

                    for (int i = 0; i < 4; i++) {
                        field[figure[i][0]][figure[i][1]] = Color.WHITE;
                    }
                    for (int[][] stateA : state) {
                        for (int i = 0; i < stateA.length; i++) {
                            stateA[i][1]++;
                        }
                    }
                    for (int i = 0; i < 4; i++) {
                        field[figure[i][0]][figure[i][1]] = figureColor;
                    }
                    break;
                case "left":
                    k:
                    for (int r = 0; r < 4; r++) {
                        int[] l = {figure[r][0] - 1, figure[r][1]};
                        for (int j = 0; j < 4; j++) {
                            if (figure[j][0] == l[0] && figure[j][1] == l[1]) continue k;
                        }
                        if (!field[l[0]][l[1]].equals(Color.WHITE)) throw new Exception();
                    }
                    for (int i = 0; i < 4; i++) {
                        field[figure[i][0]][figure[i][1]] = Color.WHITE;
                    }
                    for (int[][] stateA : state) {
                        for (int i = 0; i < stateA.length; i++) {
                            stateA[i][0]--;
                        }
                    }
                    for (int i = 0; i < 4; i++) {
                        field[figure[i][0]][figure[i][1]] = figureColor;
                    }
                    break;
                case "right":
                    k:
                    for (int r = 0; r < 4; r++) {
                        int[] l = {figure[r][0] + 1, figure[r][1]};
                        for (int j = 0; j < 4; j++) {
                            if (figure[j][0] == l[0] && figure[j][1] == l[1]) continue k;
                        }
                        if (!field[l[0]][l[1]].equals(Color.WHITE)) throw new Exception();
                    }
                    for (int i = 0; i < 4; i++) {
                        field[figure[i][0]][figure[i][1]] = Color.WHITE;
                    }
                    for (int[][] stateA : state) {
                        for (int i = 0; i < stateA.length; i++) {
                            stateA[i][0]++;
                        }
                    }
                    for (int i = 0; i < 4; i++) {
                        field[figure[i][0]][figure[i][1]] = figureColor;
                    }
            }
        }

        void nextState() {
            if (iterator.hasNext()) {
                s();
            } else {
                iterator = state.listIterator();
                s();
            }
        }

        void s() {
            try {
                int[][] q = iterator.next();
                k:
                for (int r = 0; r < 4; r++) {
                    for (int j = 0; j < 4; j++) {
                        if (q[r][0] == figure[j][0] && q[r][1] == figure[j][1]) continue k;
                    }
                    if (!field[q[r][0]][q[r][1]].equals(Color.WHITE)) throw new Exception();
                }
                for (int i = 0; i < 4; i++) {
                    field[figure[i][0]][figure[i][1]] = Color.WHITE;
                }
                figure = q;
                for (int i = 0; i < 4; i++) {
                    field[figure[i][0]][figure[i][1]] = figureColor;
                }
            } catch (Exception e) {
                iterator.previous();
            }
        }

        Figure() {
            switch ((int) (Math.random() * 7)) {
                case 0:
                    figure[0][0] = width / 2 - 2;
                    figure[0][1] = 0;
                    figure[1][0] = width / 2 - 1;
                    figure[1][1] = 0;
                    figure[2][0] = width / 2;
                    figure[2][1] = 0;
                    figure[3][0] = width / 2 + 1;
                    figure[3][1] = 0;
                    state.add(figure);

                    int[][] currentState = new int[4][2];
                    currentState[0][0] = width / 2 - 1;
                    currentState[0][1] = -3;
                    currentState[1][0] = width / 2 - 1;
                    currentState[1][1] = -2;
                    currentState[2][0] = width / 2 - 1;
                    currentState[2][1] = -1;
                    currentState[3][0] = width / 2 - 1;
                    currentState[3][1] = 0;
                    state.add(currentState);

                    figureColor = Color.CYAN;
                    break;
                case 1:
                    figure[0][0] = width / 2 - 2;
                    figure[0][1] = 1;
                    figure[1][0] = width / 2 - 1;
                    figure[1][1] = 1;
                    figure[2][0] = width / 2 - 1;
                    figure[2][1] = 0;
                    figure[3][0] = width / 2;
                    figure[3][1] = 0;
                    state.add(figure);

                    currentState = new int[4][2];
                    currentState[0][0] = width / 2 - 2;
                    currentState[0][1] = -1;
                    currentState[1][0] = width / 2 - 2;
                    currentState[1][1] = 0;
                    currentState[2][0] = width / 2 - 1;
                    currentState[2][1] = 0;
                    currentState[3][0] = width / 2 - 1;
                    currentState[3][1] = 1;
                    state.add(currentState);

                    figureColor = Color.GREEN;
                    break;
                case 2:
                    figure[0][0] = width / 2 - 2;
                    figure[0][1] = 0;
                    figure[1][0] = width / 2 - 1;
                    figure[1][1] = 0;
                    figure[2][0] = width / 2 - 1;
                    figure[2][1] = 1;
                    figure[3][0] = width / 2;
                    figure[3][1] = 1;
                    state.add(figure);

                    currentState = new int[4][2];
                    currentState[0][0] = width / 2;
                    currentState[0][1] = -1;
                    currentState[1][0] = width / 2;
                    currentState[1][1] = 0;
                    currentState[2][0] = width / 2 - 1;
                    currentState[2][1] = 0;
                    currentState[3][0] = width / 2 - 1;
                    currentState[3][1] = 1;
                    state.add(currentState);

                    figureColor = Color.RED;
                    break;
                case 3:
                    figure[0][0] = width / 2 - 2;
                    figure[0][1] = 0;
                    figure[1][0] = width / 2 - 2;
                    figure[1][1] = 1;
                    figure[2][0] = width / 2 - 1;
                    figure[2][1] = 1;
                    figure[3][0] = width / 2;
                    figure[3][1] = 1;
                    state.add(figure);

                    currentState = new int[4][2];
                    currentState[0][0] = width / 2 - 1;
                    currentState[0][1] = -1;
                    currentState[1][0] = width / 2 - 2;
                    currentState[1][1] = -1;
                    currentState[2][0] = width / 2 - 2;
                    currentState[2][1] = 0;
                    currentState[3][0] = width / 2 - 2;
                    currentState[3][1] = 1;
                    state.add(currentState);

                    currentState = new int[4][2];
                    currentState[0][0] = width / 2 - 2;
                    currentState[0][1] = 0;
                    currentState[1][0] = width / 2 - 1;
                    currentState[1][1] = 0;
                    currentState[2][0] = width / 2;
                    currentState[2][1] = 0;
                    currentState[3][0] = width / 2;
                    currentState[3][1] = 1;
                    state.add(currentState);

                    currentState = new int[4][2];
                    currentState[0][0] = width / 2;
                    currentState[0][1] = -1;
                    currentState[1][0] = width / 2;
                    currentState[1][1] = 0;
                    currentState[2][0] = width / 2 - 1;
                    currentState[2][1] = 1;
                    currentState[3][0] = width / 2;
                    currentState[3][1] = 1;
                    state.add(currentState);

                    figureColor = Color.BLUE;
                    break;
                case 4:
                    figure[0][0] = width / 2;
                    figure[0][1] = 0;
                    figure[1][0] = width / 2 - 2;
                    figure[1][1] = 1;
                    figure[2][0] = width / 2 - 1;
                    figure[2][1] = 1;
                    figure[3][0] = width / 2;
                    figure[3][1] = 1;
                    state.add(figure);

                    currentState = new int[4][2];
                    currentState[0][0] = width / 2 - 1;
                    currentState[0][1] = -1;
                    currentState[1][0] = width / 2;
                    currentState[1][1] = -1;
                    currentState[2][0] = width / 2;
                    currentState[2][1] = 0;
                    currentState[3][0] = width / 2;
                    currentState[3][1] = 1;
                    state.add(currentState);

                    currentState = new int[4][2];
                    currentState[0][0] = width / 2 - 2;
                    currentState[0][1] = 0;
                    currentState[1][0] = width / 2 - 1;
                    currentState[1][1] = 0;
                    currentState[2][0] = width / 2;
                    currentState[2][1] = 0;
                    currentState[3][0] = width / 2 - 2;
                    currentState[3][1] = 1;
                    state.add(currentState);

                    currentState = new int[4][2];
                    currentState[0][0] = width / 2 - 2;
                    currentState[0][1] = -1;
                    currentState[1][0] = width / 2 - 2;
                    currentState[1][1] = 0;
                    currentState[2][0] = width / 2 - 1;
                    currentState[2][1] = 1;
                    currentState[3][0] = width / 2 - 2;
                    currentState[3][1] = 1;
                    state.add(currentState);

                    figureColor = Color.ORANGE;
                    break;
                case 5:
                    figure[0][0] = width / 2 - 1;
                    figure[0][1] = 0;
                    figure[1][0] = width / 2;
                    figure[1][1] = 0;
                    figure[2][0] = width / 2 - 1;
                    figure[2][1] = 1;
                    figure[3][0] = width / 2;
                    figure[3][1] = 1;
                    state.add(figure);

                    figureColor = Color.YELLOW;
                    break;
                case 6:
                    figure[0][0] = width / 2 - 1;
                    figure[0][1] = 0;
                    figure[1][0] = width / 2 - 2;
                    figure[1][1] = 1;
                    figure[2][0] = width / 2 - 1;
                    figure[2][1] = 1;
                    figure[3][0] = width / 2;
                    figure[3][1] = 1;
                    state.add(figure);

                    currentState = new int[4][2];
                    currentState[0][0] = width / 2 - 2;
                    currentState[0][1] = -1;
                    currentState[1][0] = width / 2 - 2;
                    currentState[1][1] = 0;
                    currentState[2][0] = width / 2 - 1;
                    currentState[2][1] = 0;
                    currentState[3][0] = width / 2 - 2;
                    currentState[3][1] = 1;
                    state.add(currentState);

                    currentState = new int[4][2];
                    currentState[0][0] = width / 2 - 2;
                    currentState[0][1] = -1;
                    currentState[1][0] = width / 2 - 1;
                    currentState[1][1] = -1;
                    currentState[2][0] = width / 2;
                    currentState[2][1] = -1;
                    currentState[3][0] = width / 2 - 1;
                    currentState[3][1] = 0;
                    state.add(currentState);

                    currentState = new int[4][2];
                    currentState[0][0] = width / 2;
                    currentState[0][1] = -1;
                    currentState[1][0] = width / 2 - 1;
                    currentState[1][1] = 0;
                    currentState[2][0] = width / 2;
                    currentState[2][1] = 0;
                    currentState[3][0] = width / 2;
                    currentState[3][1] = 1;
                    state.add(currentState);


                    figureColor = Color.MAGENTA;
            }
            iterator = state.listIterator();
            if (iterator.hasNext()) iterator.next();
        }
    }
}

