/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package gui;

import java.awt.Color;
import java.awt.Component;
import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.InputEvent;
import java.awt.event.KeyEvent;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.JLabel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextArea;
import javax.swing.KeyStroke;
import javax.swing.SwingUtilities;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;

/**
 *
 * @author siegrift
 */
public class Frame extends javax.swing.JFrame implements Runnable {

    /**
     * Creates new form Frame
     */
    private int SLEEP_TIME = 500;
    private int MIN_SLEEP_TIME = 20;
    private int MAX_SLEEP_TIME = 3000;
    private int SLEEP_UNIT = 20;
    public static final Font RESULTS_FONT = new Font("Noto Sans", Font.PLAIN, 20);
    private String DELIMETER_LINE = "+++++";

    public Frame() {
        initAll();
    }

    public Frame(String file) {
        filename = file;
        initAll();
    }

    /**
     * This method is called from within the constructor to initialize the form.
     * WARNING: Do NOT modify this code. The content of this method is always
     * regenerated by the Form Editor.
     */
    @SuppressWarnings("unchecked")
    // <editor-fold defaultstate="collapsed" desc="Generated Code">//GEN-BEGIN:initComponents
    private void initComponents() {

        observerPanel = new gui.Observer();
        tabbedPane = new javax.swing.JTabbedPane();
        jScrollPane3 = new javax.swing.JScrollPane();
        resultsTable = new javax.swing.JTable();

        setDefaultCloseOperation(javax.swing.WindowConstants.EXIT_ON_CLOSE);
        setTitle("Observer");

        javax.swing.GroupLayout observerPanelLayout = new javax.swing.GroupLayout(observerPanel);
        observerPanel.setLayout(observerPanelLayout);
        observerPanelLayout.setHorizontalGroup(
            observerPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 0, Short.MAX_VALUE)
        );
        observerPanelLayout.setVerticalGroup(
            observerPanelLayout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGap(0, 272, Short.MAX_VALUE)
        );

        tabbedPane.setFont(RESULTS_FONT);

        resultsTable.setModel(new javax.swing.table.DefaultTableModel(
            new Object [][] {

            },
            new String [] {
                "Meno", "Body"
            }
        ));
        resultsTable.setEnabled(false);
        jScrollPane3.setViewportView(resultsTable);

        javax.swing.GroupLayout layout = new javax.swing.GroupLayout(getContentPane());
        getContentPane().setLayout(layout);
        layout.setHorizontalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(javax.swing.GroupLayout.Alignment.TRAILING, layout.createSequentialGroup()
                .addContainerGap()
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(observerPanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                    .addGroup(layout.createSequentialGroup()
                        .addComponent(jScrollPane3, javax.swing.GroupLayout.PREFERRED_SIZE, 157, javax.swing.GroupLayout.PREFERRED_SIZE)
                        .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                        .addComponent(tabbedPane, javax.swing.GroupLayout.DEFAULT_SIZE, 448, Short.MAX_VALUE)))
                .addContainerGap())
        );
        layout.setVerticalGroup(
            layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
            .addGroup(layout.createSequentialGroup()
                .addContainerGap()
                .addComponent(observerPanel, javax.swing.GroupLayout.DEFAULT_SIZE, javax.swing.GroupLayout.DEFAULT_SIZE, Short.MAX_VALUE)
                .addPreferredGap(javax.swing.LayoutStyle.ComponentPlacement.RELATED)
                .addGroup(layout.createParallelGroup(javax.swing.GroupLayout.Alignment.LEADING)
                    .addComponent(jScrollPane3, javax.swing.GroupLayout.PREFERRED_SIZE, 198, javax.swing.GroupLayout.PREFERRED_SIZE)
                    .addComponent(tabbedPane, javax.swing.GroupLayout.PREFERRED_SIZE, 198, javax.swing.GroupLayout.PREFERRED_SIZE))
                .addContainerGap())
        );

        pack();
    }// </editor-fold>//GEN-END:initComponents

    int N, M;
    int numBots;
    boolean paused = false;
    ArrayList<String> botNames;
    ArrayList<Color> botColors;
    ArrayList<JTextArea> botLogs;
    ArrayList<ArrayList<Integer>> playerPoints;
    ArrayList<BufferedReader> botLogReaders;
    String filename = null;
    Scanner in;

    // Variables declaration - do not modify//GEN-BEGIN:variables
    private javax.swing.JScrollPane jScrollPane3;
    private gui.Observer observerPanel;
    private javax.swing.JTable resultsTable;
    private javax.swing.JTabbedPane tabbedPane;
    // End of variables declaration//GEN-END:variables

    private void bindKeys() {
        tabbedPane.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_SPACE, 0), "pause");
        tabbedPane.getActionMap().put("pause", pauseObserver());

        tabbedPane.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_ADD, InputEvent.CTRL_MASK), "faster");
        tabbedPane.getActionMap().put("faster", fasterObserver());

        tabbedPane.getInputMap().put(KeyStroke.getKeyStroke(KeyEvent.VK_SUBTRACT, InputEvent.CTRL_MASK), "slower");
        tabbedPane.getActionMap().put("slower", slowerObserver());

    }

    private Action pauseObserver() {
        Frame instance = this;
        Action a;
        a = new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                synchronized (instance) {
                    paused = !paused;
                    if (paused == false) {
                        instance.notifyAll();
                    }
                }
            }
        };
        return a;
    }

    private Action fasterObserver() {
        Action a = new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                SLEEP_TIME = Math.max(SLEEP_TIME - SLEEP_UNIT, MIN_SLEEP_TIME);
                System.out.println("FAST: " + SLEEP_TIME);
            }
        };
        return a;
    }

    private Action slowerObserver() {
        Action a = new AbstractAction() {
            @Override
            public void actionPerformed(ActionEvent e) {
                SLEEP_TIME = Math.min(SLEEP_TIME + SLEEP_UNIT, MAX_SLEEP_TIME);
                System.out.println(SLEEP_TIME);
            }
        };
        return a;
    }

    private void createBotLogs() {
        botLogs = new ArrayList<>();
        playerPoints = new ArrayList<>();

        for (int i = 0; i < numBots; i++) {
            JTextArea a = new JTextArea();
            if (filename == null) {
                a.setText("Debugovacie logy sú dostupné iba v lokálnom móde!");
            }
            a.setFont(RESULTS_FONT);
            a.setEditable(false);
            a.setFocusable(false);
            JScrollPane pane = new JScrollPane();
            pane.setViewportView(a);
            botLogs.add(a);
            tabbedPane.add(pane, botNames.get(i));
        }

        //open log files
        if (filename != null) {
            botLogReaders = new ArrayList<>(numBots);
            for (int i = 0; i < numBots; i++) {
                try {
                    BufferedReader read = new BufferedReader(new FileReader(new File(botNames.get(i) + ".log")));
                    botLogReaders.add(read);
                    while (true) {
                        String line = read.readLine();
                        if (line.equals(DELIMETER_LINE)) {
                            break;
                        }
                        botLogs.get(i).setText(botLogs.get(i).getText() + line+"\n");
                    }
                } catch (FileNotFoundException ex) {
                    Logger.getLogger(Frame.class.getName()).log(Level.SEVERE, null, ex);
                } catch (IOException ex) {
                    Logger.getLogger(Frame.class.getName()).log(Level.SEVERE, null, ex);
                }
            }
        }
    }

    private void createTable() {
        resultsTable.setFont(RESULTS_FONT);
        resultsTable.getTableHeader().setFont(RESULTS_FONT);
        resultsTable.setRowHeight(getGraphics().getFontMetrics(RESULTS_FONT).getHeight());

        DefaultTableModel model = (DefaultTableModel) resultsTable.getModel();
        for (int i = 0; i < numBots; i++) {
            model.addRow(new Object[]{botNames.get(i), 0});
        }

        for (int i = 0; i < 2; i++) {
            resultsTable.getColumnModel().getColumn(i).setCellRenderer(new MyTableCellRenderer(botColors));
        }
    }

    private void initObserver() {
        if (filename != null) {
            try {
                in = new Scanner(new File(filename));
            } catch (FileNotFoundException ex) {
                Logger.getLogger(Frame.class.getName()).log(Level.SEVERE, null, ex);
            }
        } else {
            in = new Scanner(System.in);
        }

        //read header
        numBots = in.nextInt();
        botColors = new ArrayList<>();
        botNames = new ArrayList<>();

        for (int i = 0; i < numBots; i++) {
            botNames.add(in.next());
            int r = in.nextInt(), g = in.nextInt(), b = in.nextInt();
            botColors.add(new Color(r, g, b));
        }
        N = in.nextInt();
        M = in.nextInt();

        observerPanel.initData(botColors, N, M);
    }

    @Override
    public void run() {
        int whoseArea[][] = new int[N][M];
        int whoseSnake[][] = new int[N][M];

        while (true) {
            long time = System.currentTimeMillis();
            //read general
            for (int i = 0; i < numBots; i++) {
                ArrayList<Integer> tmpList = new ArrayList<>();
                int xpos = in.nextInt();
                int ypos = in.nextInt();
                int score = in.nextInt();

                tmpList.add(xpos);
                tmpList.add(ypos);
                playerPoints.add(tmpList);

                resultsTable.getModel().setValueAt(score, i, 1);
            }

            //read map
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < M; j++) {
                    whoseArea[i][j] = in.nextInt();
                    whoseSnake[i][j] = in.nextInt();
                }
            }

            //read logs
            for (int i = 0; i < numBots; i++) {
                try {
                    String s;
                    while (!(s = botLogReaders.get(i).readLine()).equals(DELIMETER_LINE)) {
                        botLogs.get(i).setText(botLogs.get(i).getText() + s+"\n");
                    }
                } catch (IOException ex) {
                    Logger.getLogger(Frame.class.getName()).log(Level.SEVERE, null, ex);
                }
            }

            SwingUtilities.invokeLater(() -> {
                observerPanel.setData(whoseArea, whoseSnake);
                observerPanel.repaint();
            });

            long totTime = (System.currentTimeMillis() - time);
            //System.out.println("TOT time: " + totTime);
            try {
                Thread.sleep(Math.max(SLEEP_TIME - totTime, (long) MIN_SLEEP_TIME));
                synchronized (this) {
                    while (paused) {
                        this.wait();
                    }
                }
            } catch (InterruptedException ex) {
                Logger.getLogger(Frame.class.getName()).log(Level.SEVERE, null, ex);
                System.err.println(ex);
            }
        }
    }

    private void startThread() {
        new Thread(this).start();
    }

    public void setFocus() {
        addWindowListener(new WindowAdapter() {
            @Override
            public void windowOpened(WindowEvent e) {
                super.windowOpened(e); //To change body of generated methods, choose Tools | Templates.
                tabbedPane.requestFocus();
            }
        });
    }

    private void initAll() {
        initComponents();
        initObserver();
        createBotLogs();
        createTable();
        bindKeys();

        startThread();
    }

    private static class MyTableCellRenderer extends DefaultTableCellRenderer {

        ArrayList<Color> colors;

        public MyTableCellRenderer(ArrayList<Color> c) {
            colors = c;
        }

        @Override
        public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
            JLabel l = (JLabel) super.getTableCellRendererComponent(table, value, isSelected, hasFocus, row, column);
            l.setForeground(colors.get(row));
            return l;
        }
    }
}
