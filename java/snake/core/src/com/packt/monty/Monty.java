package com.packt.monty;

import com.badlogic.gdx.ApplicationListener;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.*;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;

public class Monty implements ApplicationListener {
	private OrthographicCamera camera;
	private SpriteBatch batch;
	private float w, h;

	@Override
	public void create() {
		w = Gdx.graphics.getWidth();
		h = Gdx.graphics.getHeight();
		camera = new OrthographicCamera(w, h);
		batch = new SpriteBatch();
		camera.setToOrtho(false);

		batch = new SpriteBatch();
		GameManager.initialize(w, h);
	}


	@Override
	public void dispose () {
		batch.dispose();
		GameManager.dispose();
	}

	@Override
	public void pause () {

	}

	@Override
	public void resume () {
	}

	@Override
	public void render () {
		Gdx.gl.glClearColor(1, 1, 1, 1);
		Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT);

		batch.setProjectionMatrix(camera.combined);
		InputManager.handleInput(camera);

		batch.begin();
		GameManager.renderGame(batch);
		TextManager.displayMessage(batch);
		batch.end();
	}

	@Override
	public void resize (int width, int height) {

	}
}