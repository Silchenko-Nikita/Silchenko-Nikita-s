package com.packt.monty;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.Sprite;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.math.Vector3;
import com.badlogic.gdx.utils.Array;
import com.badlogic.gdx.utils.IntArray;

public class GameManager {
    static Array<Door> doors;
    static Texture doorTexture;
    static Texture carTexture;
    static Texture goatTexture;
    static Vector3 temp = new Vector3();
    static IntArray goatIndices;
    static Level level;
    static boolean hasWon = false;
    static Sprite restartSprite;
    static Texture restartTexture;
    static Texture backtexture;
    static Sprite backSprite;

    static final float RESTART_RESIZE_FACTOR = 5500f;
    public static final float DOOR_RESIZE_FACTOR = 5000.0f;
    public static final float DOOR_VERT_POSITION_FACTOR = 3.0f;
    public static final float DOOR1_HORIZ_POSITION_FACTOR = 7.77f;
    public static final float DOOR2_HORIZ_POSITION_FACTOR = 2.57f;
    public static final float DOOR3_HORIZ_POSITION_FACTOR = 1.52f;
    static float width, height;

    public static void initialize(float width, float height){
        backtexture = new Texture(Gdx.files.internal("data/background.jpg"));
        backSprite = new Sprite(backtexture);
        backSprite.setSize(width, height);
        backSprite.setPosition(0,0f);

        restartTexture = new Texture(Gdx.files.internal("data/restart.png"));
        restartSprite = new Sprite(restartTexture);
        restartSprite.setSize(restartSprite.getWidth()*
                width/RESTART_RESIZE_FACTOR,restartSprite.getHeight()*
                width/RESTART_RESIZE_FACTOR);

        restartSprite.setPosition(0, 0);
        GameManager.width = width;
        GameManager.height = height;
        doorTexture = new Texture(Gdx.files.internal("data/door_close.png"));
        carTexture = new Texture(Gdx.files.internal("data/door_open_car.png"));
        goatTexture = new Texture(Gdx.files.internal("data/door_open_goat.png"));
        goatIndices = new IntArray();
        level = Level.START;
        initDoors();
        TextManager.initialize(width, height);
    }

    public static void renderGame(SpriteBatch batch){
        backSprite.draw(batch);
        restartSprite.draw(batch);
        for (Door door : doors){
            door.render(batch);
        }
    }

    public static void dispose(){
        doorTexture.dispose();
        carTexture.dispose();
        goatTexture.dispose();
        restartTexture.dispose();
        backtexture.dispose();
    }

    public static void initDoors(){
        doors = new Array<Door>();

        for (int i = 0; i < 3; i++) {
            doors.add(new Door());
        }

        doors.get(0).position.set(width/DOOR1_HORIZ_POSITION_FACTOR, height/DOOR_VERT_POSITION_FACTOR);
        doors.get(1).position.set(width/DOOR2_HORIZ_POSITION_FACTOR, height/DOOR_VERT_POSITION_FACTOR);
        doors.get(2).position.set(width/DOOR3_HORIZ_POSITION_FACTOR, height/DOOR_VERT_POSITION_FACTOR);

        for (Door door : doors){
            door.closeSprite = new Sprite(doorTexture);
            door.openSprite = new Sprite();
            door.width = door.closeSprite.getWidth()*(width/DOOR_RESIZE_FACTOR);
            door.height = door.closeSprite.getHeight()*(height/DOOR_RESIZE_FACTOR);
            door.closeSprite.setSize(door.width, door.height);
            door.closeSprite.setPosition(door.position.x, door.position.y);
            door.openSprite.setSize(door.width, door.height);
            door.openSprite.setPosition(door.position.x, door.position.y);
        }

        doors.get(0).openSprite.setRegion(goatTexture);
        doors.get(0).isGoat = true;
        doors.get(1).openSprite.setRegion(carTexture);
        doors.get(1).isGoat = false;
        doors.get(2).openSprite.setRegion(goatTexture);
        doors.get(2).isGoat = true;
    }

    public static IntArray getIndices(int selectedDoorIndex){
        goatIndices.clear();

        for(int i=0;i<doors.size;i++){
            if(i!=selectedDoorIndex && doors.get(i).isGoat) {
                goatIndices.add(i);
            }
        }
        return goatIndices;
    }

    enum Level{
        START,
        CONFIRM,
        END
    }

    public static void restartGame(){
        doors.shuffle();
        doors.get(0).position.set(width/DOOR1_HORIZ_POSITION_FACTOR, height/DOOR_VERT_POSITION_FACTOR);
        doors.get(1).position.set(width/DOOR2_HORIZ_POSITION_FACTOR, height/DOOR_VERT_POSITION_FACTOR);
        doors.get(2).position.set(width/DOOR3_HORIZ_POSITION_FACTOR, height/DOOR_VERT_POSITION_FACTOR);

        for(int i=0;i<GameManager.doors.size;i++){
            GameManager.doors.get(i).isOpen=false;
            GameManager.doors.get(i).closeSprite.setPosition
                    (GameManager.doors.get(i).position.x,
                            GameManager.doors.get(i).position.y);
            GameManager.doors.get(i).openSprite.setPosition
                    (GameManager.doors.get(i).position.x,
                            GameManager.doors.get(i).position.y);
        }
        GameManager.hasWon=false;
        level = Level.START;
        TextManager.confirm = new StringBuffer("You selected door no.Do you want to switch or stay?");
    }
}