/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package proboj;

import gui.Frame;
import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.JTextArea;

/**
 *
 * @author siegrift
 */
public class BotLogReader implements Runnable {

    ArrayList<BufferedReader> botLogReaders;
    ArrayList<JTextArea> botLogs;
    int n;
    int roundNumber;
    private String NEXT_TURN_LOG = "TAH ";

    public BotLogReader(ArrayList<BufferedReader> botReaders, ArrayList<JTextArea> botLogsJTF) {
        botLogReaders = botReaders;
        n = botLogReaders.size();
        botLogs = botLogsJTF;
    }

    @Override
    public void run() {
        //init
        for (int i = 0; i < n; i++) {
            try {
                BufferedReader read = botLogReaders.get(i);
                while (true) {
                    String line = read.readLine();
                    if (line == null) {
                        throw new IOException("Koniec bot logu!");
                    }
                    if (line.startsWith(NEXT_TURN_LOG)) {
                        roundNumber = parseRound(line);
                        break;
                    }
                    botLogs.get(i).setText(botLogs.get(i).getText() + String.format("[ INIT ] %s\n", line));
                }
            } catch (IOException ex) {
                botLogs.get(i).setText(botLogs.get(i).getText() + String.format("--KONIEC BOT LOGU!--\n"));
                botLogReaders.set(i, null);
            }
        }

        //read logs
        for (int i = 0; i < n; i++) {
            if (botLogReaders.get(i) == null) {
                continue;
            }
            try {
                String s = botLogReaders.get(i).readLine();
                while (s != null && s.startsWith(NEXT_TURN_LOG) == false) {
                    botLogs.get(i).setText(botLogs.get(i).getText() + String.format("[Tah %d] %s\n", roundNumber, s));
                    s = botLogReaders.get(i).readLine();
                }

                if (s == null) {
                    throw new IOException("Koniec bot logu!");
                } else {
                    roundNumber = parseRound(s);
                }
            } catch (IOException ex) {
                botLogReaders.set(i, null);
                botLogs.get(i).setText(botLogs.get(i).getText() + String.format("--KONIEC BOT LOGU!--\n"));
                Logger.getLogger(Frame.class.getName()).log(Level.SEVERE, null, ex);
            }
        }
    }

    private int parseRound(String s) {
        return Integer.parseInt(s.split(" ")[1]);
    }

}
