package nikitos;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.*;

public class Main {

    public static void main(String[] args) {
        SwingUtilities.invokeLater(SaperFrame::new);
    }
}

class SaperFrame extends JFrame implements Observer{
    AskPanel askPanel;
    SaperPanel saperPanel;

    SaperFrame(){
        setSize(220,220);
        setLocationRelativeTo(null);
        setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        askPanel = new AskPanel(this);
        add(askPanel);
        setVisible(true);
        toFront();
        setLayout(null);
    }

    @Override
    public void update(Observable o, Object arg) {
        askPanel.setVisible(false);
        saperPanel = new SaperPanel((int) arg, this);
        setSize(saperPanel.getWidth() + 6, saperPanel.getHeight() + 28);
        setResizable(false);
        setLocationRelativeTo(null);
        add(saperPanel);
        saperPanel.setVisible(true);
    }
}

class AskPanel extends JPanel{

    AskPanel(SaperFrame invokedSaperFrame){
        ObservableBySaperFrame obs = new ObservableBySaperFrame();
        obs.addObserver(invokedSaperFrame);

        setBounds(10,40,150,100);
        setLayout(null);
        JLabel question = new JLabel("How many blocks on a side?");
        question.setBounds(20, 10, 200, 30);

        Integer numbers[] = {9, 16, 25};
        JList<Integer> list = new JList<>(numbers);
        list.setBounds(20, 10 + question.getHeight(), 110, 60);
        list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);

        JButton button = new JButton("Continue");
        button.setBackground(Color.LIGHT_GRAY);
        button.setBounds(20, 25 + question.getHeight() + list.getHeight(), 110, 25);

        add(question);
        add(list);
        add(button);

        button.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                if (list.getSelectedValue() == null) return;
                obs.setChanged();
                obs.notifyObservers(list.getSelectedValue());
            }
        });
    }

    class ObservableBySaperFrame extends Observable {

        protected void setChanged(){
            super.setChanged();
        }
    }
}

class SaperPanel extends JPanel{
    Color buttonColor;
    SaperActionListener actionListen;
    int saperButtonSize = 20, numberOfBlocks;
    ArrayList<SaperButton> buttonsList;
    SaperButton[][] buttonArray;
    SaperFrame invokedSaperFrame;

    SaperPanel(int numberOfBlocks, SaperFrame invokedSaperFrame){
        super(true);
        this.invokedSaperFrame = invokedSaperFrame;
        this.numberOfBlocks = numberOfBlocks;
        setBounds(0, 0, saperButtonSize * numberOfBlocks, saperButtonSize * numberOfBlocks);
        setLayout(new GridLayout(numberOfBlocks, numberOfBlocks));
        actionListen = new SaperActionListener();

        int lng = numberOfBlocks*numberOfBlocks;
        buttonsList = new ArrayList<>(lng);
        int numberOfBombs = (int) (0.15 * lng);
        for (int i = 0; i < lng; i++) {
            SaperButton button = new SaperButton();
            button.bomb = i <= numberOfBombs;
            button.addActionListener(actionListen);
            button.addMouseListener(new MouseAdapter() {
                @Override
                public void mouseClicked(MouseEvent e) {
                    SaperButton clickedButton = (SaperButton) e.getSource();
                    if (e.getButton() == MouseEvent.BUTTON3){
                        if (clickedButton.getBackground().equals(buttonColor)){
                            clickedButton.setBackground(Color.LIGHT_GRAY);
                            clickedButton.setFlag();
                        } else {
                            clickedButton.setBackground(buttonColor);
                            clickedButton.removeAll();
                        }
                    }
                }
            });
            buttonsList.add(button);
        }
        buttonColor = buttonsList.get(0).getBackground();
        Collections.shuffle(buttonsList);
        buttonArray = new SaperButton[numberOfBlocks][numberOfBlocks];

        int k = 0;
        for (int i = 0; i < numberOfBlocks; i++) {
            for (int j = 0; j < numberOfBlocks; j++) {
                buttonArray[i][j] = buttonsList.get(k++);
            }
        }
        countBombsNear();

        for (int i = 0; i < numberOfBlocks; i++) {
            for (int j = 0; j < numberOfBlocks; j++) {
                ButtonPanel w = new ButtonPanel(buttonArray[i][j]);
                w.setBackground(Color.WHITE);
                w.setLayout(new GridLayout(1,1));
                w.add(buttonArray[i][j]);
                add(w);
            }
        }
    }

    void countBombsNear(){
        for (int i = 0; i < numberOfBlocks; i++) {
            for (int j = 0; j < numberOfBlocks; j++) {
                int b = 0;
                try {
                    if (buttonArray[i + 1][j].bomb) b++;
                }catch (ArrayIndexOutOfBoundsException ignored){}

                try {
                    if (buttonArray[i + 1][j + 1].bomb) b++;
                }catch (ArrayIndexOutOfBoundsException ignored){}

                try {
                    if (buttonArray[i + 1][j - 1].bomb) b++;
                }catch (ArrayIndexOutOfBoundsException ignored){}

                try {
                    if (buttonArray[i][j].bomb) b++;
                }catch (ArrayIndexOutOfBoundsException ignored){}

                try {
                    if (buttonArray[i][j + 1].bomb) b++;
                }catch (ArrayIndexOutOfBoundsException ignored){}

                try {
                    if (buttonArray[i][j - 1].bomb) b++;
                }catch (ArrayIndexOutOfBoundsException ignored){}

                try {
                    if (buttonArray[i - 1][j].bomb) b++;
                }catch (ArrayIndexOutOfBoundsException ignored){}

                try {
                    if (buttonArray[i - 1][j + 1].bomb) b++;
                }catch (ArrayIndexOutOfBoundsException ignored){}

                try {
                    if (buttonArray[i - 1][j - 1].bomb) b++;
                }catch (ArrayIndexOutOfBoundsException ignored){}

                buttonArray[i][j].numberOfBombsNear = b;
            }
        }
    }

    class SaperActionListener implements ActionListener {

        @Override
        public void actionPerformed(ActionEvent e) {
            SaperButton clickedButton = (SaperButton) e.getSource();
            if (!clickedButton.getBackground().equals(buttonColor)) return;
            SaperButton invoked = ((SaperButton) e.getSource());
            SaperButton butt = (SaperButton)e.getSource();
            if (butt.bomb){
                JLabel label = new JLabel();
                JPanel par = (JPanel) butt.getParent();
                par.setBackground(Color.RED);
                label.setText("   !");
                butt.changed = true;
                try {
                    par.removeAll();
                    par.add(label);
                    par.updateUI();
                } catch (NullPointerException ignored) {
                }

                for (int i = 0; i < numberOfBlocks; i++) {
                    for (int j = 0; j < numberOfBlocks; j++) {
                        updateButton(buttonArray[i][j], true, Color.YELLOW);
                    }
                }
                new GameOverDialog(false);
            }else {
                int bombs = invoked.numberOfBombsNear;
                if (bombs == 0){
                    t: for (int i = 0; i < numberOfBlocks; i++) {
                        for (int j = 0; j < numberOfBlocks; j++) {
                            if (buttonArray[i][j].equals(invoked)) {
                                zeroCase(i, j);
                                break t;
                            }
                        }
                    }
                }else updateButton(invoked, false, null);
            }
        }
    }

    void updateButton(SaperButton s, boolean gameOver, Color bombBackgroundColor){
        if (s.changed) return;
        ButtonPanel q = (ButtonPanel) s.getParent();
        JLabel l;
        if (s.bomb){
            l = new JLabel();
            q.setBackground(bombBackgroundColor);
            l.setText("   !");
        }else {
            l = new JLabel(s.numberOfBombsNear == 0 ? "" : "  " + s.numberOfBombsNear);
        }
        s.changed = true;
        try {
            q.removeAll();
            q.add(l);
            q.updateUI();
        } catch (NullPointerException ignored) {
        }
        q.repaint();

        if (gameOver) return;
        for (SaperButton e : buttonsList) {
            if (!e.changed && !e.bomb) return;
        }

        for (int i = 0; i < numberOfBlocks; i++) {
            for (int j = 0; j < numberOfBlocks; j++) {
                updateButton(buttonArray[i][j], true, Color.green);
            }
        }
        new GameOverDialog(true);
    }

    void zeroCase(int i, int j){
        SaperButton s;
        if (i < 0 || i >= numberOfBlocks || j < 0 || j >= numberOfBlocks || (s = buttonArray[i][j]).changed) return;
        updateButton(s, false, null);
        if (!(s.numberOfBombsNear == 0)) return;
        zeroCase(i + 1, j);
        zeroCase(i + 1, j + 1);
        zeroCase(i + 1, j - 1);
        zeroCase(i, j);
        zeroCase(i, j + 1);
        zeroCase(i, j - 1);
        zeroCase(i - 1, j);
        zeroCase(i - 1, j + 1);
        zeroCase(i - 1, j - 1);
    }

    class SaperButton extends JButton{
        boolean changed = false;
        boolean bomb;
        int numberOfBombsNear = 0;

        void setFlag(){
            JLabel l = new JLabel("F");
            l.setBounds(7 , 0, 16, 20);
            setLayout(null);
            add(l);
            updateUI();
        }
    }

    class ButtonPanel extends JPanel{
        SaperButton inButton;

        ButtonPanel(SaperButton inButton){
            this.inButton = inButton;
        }

        @Override
        public void paint(Graphics g) {
            super.paint(g);
            if (!inButton.changed) return;
            g.drawRect(0, 0, getWidth(), getHeight());
        }
    }

    class GameOverDialog extends JDialog{

        GameOverDialog(boolean won){
            setLayout(null);
            setSize(240, 100);
            setLocationRelativeTo(null);
            JLabel ll = new JLabel(won ? "You won!" : "You lose!");
            JButton button = new JButton("Restart");
            button.setBackground(Color.LIGHT_GRAY);
            JButton button1 = new JButton("Exit");
            button1.setBackground(Color.LIGHT_GRAY);

            button.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    invokedSaperFrame.dispose();
                    dispose();
                    Main.main(new String[0]);
                }
            });

            button1.addActionListener(new ActionListener() {
                @Override
                public void actionPerformed(ActionEvent e) {
                    System.exit(0);
                }
            });

            ll.setBounds(85, 0, 100, 30);
            button.setBounds(3, 30, 100, 30);
            button1.setBounds(120, 30, 100, 30);

            add(button);
            add(button1);
            add(ll);
            setVisible(true);
        }
    }
}

class BombsCountPanel extends JPanel{
    int numberOfBombs;

    BombsCountPanel(){

    }
}
