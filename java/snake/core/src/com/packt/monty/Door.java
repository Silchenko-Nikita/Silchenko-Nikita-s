package com.packt.monty;

import com.badlogic.gdx.graphics.g2d.Sprite;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;
import com.badlogic.gdx.math.Vector2;

public class Door {
    public Sprite openSprite;
    public Sprite closeSprite;
    public boolean isGoat = false;
    public boolean isOpen = false;
    public Vector2 position = new Vector2();

    public float width, height;

    public void render(SpriteBatch batch){
        if (isOpen){
            openSprite.draw(batch);
        }else{
            closeSprite.draw(batch);
        }
    }
}
