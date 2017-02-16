package com.packt.monty;

import com.badlogic.gdx.graphics.Color;
import com.badlogic.gdx.graphics.g2d.BitmapFont;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;

public class TextManager {
    static BitmapFont font;
    static String start = "Select a door";
    static StringBuffer confirm;
    static String win = "You Win!";
    static String lose = "You Lose!";
    static float width,height;

    public static void initialize(float width,float height){
        TextManager.width = width;
        TextManager.height= height;
        font = new BitmapFont();
        font.setColor(Color.CYAN);
        confirm = new StringBuffer("You selected door no. Do you want to switch or stay?");
    }

    public static void displayMessage(SpriteBatch batch){
        switch(GameManager.level){
            case START:
                font.draw(batch, start, width/2 - font.getSpaceWidth()*start.length(), GameManager.doors
                        .first().closeSprite.getY()/2);
                break;
            case CONFIRM:
                font.draw(batch, confirm, width/2 - font.getSpaceWidth()*confirm.length(), GameManager.doors
                        .first().closeSprite.getY()/2);
                break;
            case END:
                if(GameManager.hasWon)
                    font.draw(batch, win,width/2 - font.getSpaceWidth()*win.length(), GameManager.doors
                            .first().closeSprite.getY()/2);
                else
                    font.draw(batch,lose,width/2 - font.getSpaceWidth()*lose.length(), GameManager.doors
                            .first().closeSprite.getY()/2);
                break;
        }
    }

    public static void setSelectedDoor(int doorIndex){
        confirm.insert(confirm.indexOf("door no")+"door no".length(), " "+(doorIndex+1));
    }
}