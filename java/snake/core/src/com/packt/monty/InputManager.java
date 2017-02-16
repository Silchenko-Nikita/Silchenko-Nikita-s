package com.packt.monty;

import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.OrthographicCamera;

public class InputManager {

    public static void handleInput(OrthographicCamera camera){
        if (Gdx.input.justTouched()){
            GameManager.temp.set(Gdx.input.getX(), Gdx.input.getY(), 0);
            camera.unproject(GameManager.temp);

            float touchX = GameManager.temp.x;
            float touchY = GameManager.temp.y;

            for (int i = 0; i < GameManager.doors.size; i++) {
                if (!GameManager.doors.get(i).isOpen){

                    if(handleDoor(GameManager.doors.get(i), touchX, touchY, i))
                        break;
                }
            }
            handleRestart(touchX,touchY);
        }
    }

    public static void handleRestart(float touchX,float touchY){

        if((touchX>=GameManager.restartSprite.getX()) &&
                touchX<=(GameManager.restartSprite.getX()+GameManager
                        .restartSprite.getWidth()) && (touchY>=GameManager
                .restartSprite.getY()) && touchY<=(GameManager
                .restartSprite.getY()+GameManager
                .restartSprite.getHeight()) ){
            GameManager.restartGame();
        }
    }

    public static boolean handleDoor(Door door, float touchX, float touchY, int doorIndex){
        if(touchX >= door.position.x && touchX <= door.position.x + door.width &&
                touchY >= door.position.y && touchY <= door.position.y + door.height) {
            switch (GameManager.level) {
                case START:
                    GameManager.doors.get(GameManager.getIndices(doorIndex).random()).isOpen = true;
                    GameManager.level = GameManager.Level.CONFIRM;
                    TextManager.setSelectedDoor(doorIndex);
                    break;
                case CONFIRM:
                    door.isOpen = true;
                    GameManager.level = GameManager.Level.END;
                    if(!door.isGoat){
                        GameManager.hasWon=true;
                    }
                    break;
                case END:
                    break;
            }

            return true;
        }
        return false;
    }
}
