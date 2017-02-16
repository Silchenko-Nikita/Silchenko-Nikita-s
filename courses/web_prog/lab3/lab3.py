import json, os
from flask import Flask, url_for, render_template, redirect
app = Flask(__name__)

APP_ROOT = os.path.dirname(os.path.abspath(__file__))
APP_STATIC = os.path.join(APP_ROOT, 'static')

f = open(os.path.join(APP_STATIC, 'data/films.json'), "r", encoding='utf-8')
filmsJSON = json.load(f)

@app.route('/')
def home():
    return redirect(url_for("index"))

@app.route('/index.html')
def index():
    return render_template("index.html")


@app.route('/about.html')
def about():
    return render_template("about.html")

@app.route('/films_table.html')
def films_table():
    return render_template('films_table.html', filmsJSON=filmsJSON)

@app.route('/films_descr/<tag>.html')
def film_full_descr(tag):
    filmJSON = next(filmJSON for filmJSON in filmsJSON if filmJSON['tag'] == tag)
    return render_template('full_descr.html', filmJSON=filmJSON)

if __name__ == '__main__':
    app.run()