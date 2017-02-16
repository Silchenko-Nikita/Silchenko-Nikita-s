import sqlite3, os, json, ast
from flask import Flask, g, redirect, url_for, render_template, request, abort

app = Flask(__name__)

APP_ROOT = app.root_path
APP_STATIC = os.path.join(APP_ROOT, 'static')
APP_DB = os.path.join(APP_STATIC, 'data/data.db')

#db
def connect_db():
    rv = sqlite3.connect(APP_DB)
    rv.row_factory = sqlite3.Row
    return rv

def get_db():
    if not hasattr(g, 'sqlite_db'):
        g.sqlite_db = connect_db()
    return g.sqlite_db

@app.teardown_appcontext
def close_db(error):
    if hasattr(g, 'sqlite_db'):
        g.sqlite_db.close()

def db_get_all_from_films():
    db = get_db()
    c = db.execute("SELECT * FROM films")
    return c.fetchall()


def db_add_film(film):
    db = get_db()
    try:
        c = db.execute("INSERT INTO films VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)",
                       (film['tag'], film['Название'], film['Год'], film['Страна'],
                        film['Слоган'], film['Режиссер'], film['Жанр'], film['Постер'],
                        film['Короткое описание'], "".join(film['Длинное описание']))
                       )
        c.fetchall()
        db.commit()
        return True
    except:
        return False


def db_get_from_films(tag=None, name=None):
    db = get_db()
    if name and tag:
        c = db.execute("SELECT * FROM films where Название=\"(?)\" and tag=(?)", (name, tag))
    elif tag:
        c = db.execute("SELECT * FROM films where tag=(?)", (tag,))
    else:
        c = db.execute('SELECT * FROM films where Название=(?)', (name,))
    return c.fetchone()


def db_delete_from_films(tag):
    db = get_db()
    c = db.execute("DELETE FROM films where tag=(?)", (tag,))
    c.fetchall()
    db.commit()


def db_update_film(tag, film_update: dict):
    film = dict(db_get_from_films(tag))
    res_film = {}
    for k, v in film.items():
        if isinstance(k, str):
            k = k.replace('o', 'о')
        res_film[k] = film_update[k] if film_update.get(k) else v
        film_update[k] = 'spanned'

    for k, v in film_update.items():
        if v is not 'spanned':
            return False

    db = get_db()

    try:
        c = db.execute("""UPDATE films SET "tag" = (?), "Название" = (?),
                "Год" = (?), "Страна" = (?),
                "Слоган" = (?), "Режиссер" = (?),
                "Жанр" = (?), "Постер" = (?),
                "Короткое описание" = (?), "Длинное описание" = (?)
                where tag=(?)""", (res_film['tag'], res_film['Название'], res_film['Год'], res_film['Страна'],
                                   res_film['Слоган'], res_film['Режиссер'], res_film['Жанр'], res_film['Постер'],
                                   res_film['Короткое описание'], "".join(res_film['Длинное описание']), tag))
    except:
        return False

    c.fetchall()
    db.commit()
    return True


def db_get_films_num():
    db = get_db()
    c = db.execute("SELECT COUNT(*) FROM films", ())
    return c.fetchone()
#//db

@app.route('/')
def home():
    return redirect(url_for("index"))

@app.route('/index.html')
def index():
    return render_template("index.html")

@app.route('/about.html')
def about():
    return render_template("about.html")

# REST API
@app.route('/films')
def REST_films():
    films_list = [dict(film) for film in db_get_all_from_films()]
    result_films = []
    for film in films_list:
        if ((request.args.get('yearlt') and film["Год"] < int(request.args.get('yearlt'))) or not request.args.get('yearlt')) and \
                ((request.args.get('yeargt') and film["Год"] > int(request.args.get('yeargt'))) or not request.args.get('yeargt')):
            result_films.append(film)
    return json.dumps(result_films, ensure_ascii=False)


@app.route('/add_film', methods=['POST'])
def REST_add_film():
    if isinstance(request.data, (bytearray, bytes)):
        json_str = request.data.decode("utf-8")
    else:
        json_str = request.data
    film = ast.literal_eval(json_str.strip('﻿'))

    if not db_add_film(film):
        returned = {
            "status": "notok",
            "error": "occupied tag"
        }
    else:
        returned = {
            "status": "ok",
            "inserted": dict(film)
        }

    return json.dumps(returned, ensure_ascii=False)


@app.route('/film/<int:id>', methods=['GET', 'DELETE'])
def REST_film(id):
    if db_get_films_num()[0] < id or id < 1:
        returned = {
            "status": "notok",
            "error": "invalid id"
        }
        return json.dumps(returned, ensure_ascii=False)

    films_list = [dict(film) for film in db_get_all_from_films()]
    if request.method == 'GET':
        return json.dumps(films_list[id - 1], ensure_ascii=False)
    elif request.method == 'DELETE':
        db_delete_from_films(films_list[id - 1]['tag'])

        returned = {
            "status" : "ok",
            "deleted" : dict(films_list[id - 1])
        }
        return json.dumps(returned, ensure_ascii=False)


@app.route('/film/<tag>', methods=['GET', 'DELETE'])
def REST_film_by_tag(tag):
    film = db_get_from_films(tag.strip())
    if not film:
        returned = {
            "status": "notok",
            "error": "invalid tag"
        }
        return json.dumps(returned, ensure_ascii=False)

    if request.method == 'GET':
        return json.dumps(dict(film), ensure_ascii=False)
    elif request.method == 'DELETE':
        db_delete_from_films(tag)

        returned = {
            "status": "ok",
            "deleted": dict(film)
        }
        return json.dumps(returned, ensure_ascii=False)


@app.route('/update_film/<int:id>', methods=['POST'])
def REST_update_film(id):
    if db_get_films_num()[0] < id or id < 1:
        returned = {
            "status": "notok",
            "error": "invalid id"
        }

    if isinstance(request.data, (bytearray, bytes)):
        json_str = request.data.decode("utf-8")
    else:
        json_str = request.data
    film_update = ast.literal_eval(json_str.strip('﻿'))

    films_list = [dict(film) for film in db_get_all_from_films()]
    film = films_list[id - 1]

    if film_update.get('tag') and (dict(db_get_from_films(film_update['tag'])) != film):
        returned = {
            "status": "notok",
            "error": "occupied tag"
        }
        return json.dumps(returned, ensure_ascii=False)

    if not db_update_film(film['tag'], film_update):
        returned = {
            "status": "notok",
            "error": "invalid keyvals "
        }
    else:
        returned = {
            "status": "ok",
            "updated": dict(film)
        }

    return json.dumps(returned, ensure_ascii=False)


@app.route('/update_film/<tag>', methods=['POST'])
def REST_update_film_by_tag(tag):
    if isinstance(request.data, (bytearray, bytes)):
        json_str = request.data.decode("utf-8")
    else:
        json_str = request.data
    film_update = ast.literal_eval(json_str.strip('﻿'))

    film = db_get_from_films(tag)
    if film_update.get('tag') and (db_get_from_films(film_update['tag']) != film):
        returned = {
            "status": "notok",
            "error": "occupied tag"
        }
        return json.dumps(returned, ensure_ascii=False)

    if not db_update_film(tag, film_update):
        returned = {
            "status": "notok",
            "error": "invalid keyvals"
        }
    else:
        returned = {
            "status": "ok",
            "updated": dict(film)
        }

    return json.dumps(returned, ensure_ascii=False)
# //REST API


# errorhandlers
@app.errorhandler(410)
def no_such_film(e):
    return "No such film", 410


@app.errorhandler(411)
def several_films_with_such_names(e):
    return "There are several films with such names", 411

# // errorhandlers


@app.route('/add_film_within_form', methods=['POST'])
def add_film():
    film = {k : "".join(v) for k, v in dict(request.form).items()}
    film["Постер"] = cStringIO.StringIO(film['Постер'])
    print(film)
    print(db_add_film(film))

    return redirect(url_for("films_table"))


@app.route('/delete_film_within_form/<tag>', methods=['POST'])
def delete_film_within_form(tag):
    db_delete_from_films(tag)

    return redirect(url_for("films_table"))


@app.route('/films_table.html')
def films_table():
    films = db_get_all_from_films()

    return render_template('films_table.html', films=films)


@app.route('/films_descr/<tag>.html')
def film_full_descr(tag):
    film = db_get_from_films(tag)

    return render_template('full_descr.html', film=film)


@app.route('/films_descr')
def film_full_descr_arg():
    name = request.args.get("name").strip()
    if name:
        got_films = [x for x in list(db_get_all_from_films()) if dict(x).get("Название") == name]
        if len(got_films) > 1:
            abort(411)
        elif not len(got_films):
            abort(410)

        film = dict(got_films[0])
        return render_template('full_descr.html', film=film)
    else:
        abort(410)


if __name__ == '__main__':
    app.run()