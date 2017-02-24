import json

from django.core.files import File

from films.models import Film
import os.path

BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

# def run_script():
#     print(os.path.abspath(os.path.join(__file__, os.pardir)) + '/static/data/films.json')
#     films = json.load(open(os.path.abspath(os.path.join(__file__, os.pardir)+ '/static/data/films.json'), "r", encoding='utf-8'))
#     transl_dict = {
#         "tag": "tag",
#         "Название": "name",
#         "Год":	"year",
#         "Страна":	"country",
#         "Слоган":	"tagline",
#         "Режиссер":	"producer",
#         "Жанр": "genre",
#         "Постер": "poster",
#         "Короткое описание": "short_descr",
#         "Длинное описание": "full_descr"
#     }
#     for film in films:
#         db_film = Film()
#         for field in film:
#             str_repr = "".join(film[field]) if field == "Длинное описание" else film[field]
#             setattr(db_film, transl_dict[field], str_repr)
#             print(transl_dict[field], str_repr)
#         db_film.save()

def run_script():
    films = json.load(open(os.path.abspath(os.path.join(__file__, os.pardir) + '/static/data/films.json'), "r", encoding='utf-8'))
    for film in films:
        db_film = Film()
        for field in film:
            if field == "tag":
                db_film.tag = film[field]
            elif field == "Название":
                db_film.name = film[field]
            elif field == "Год":
                db_film.year = film[field]
            elif field == "Страна":
                db_film.country = film[field]
            elif field == "Слоган":
                db_film.tagline = film[field]
            elif field == "Режиссер":
                db_film.producer = film[field]
            elif field == "Жанр":
                db_film.genre = film[field]
            elif field == "Постер":
                pass
                # poster_file = open(os.path.abspath(os.path.join(__file__, os.pardir)) + "/static/media/images/posters/" + film[field], "rb")
                # django_file = File(poster_file)
                # db_film.poster.save(film[field], django_file, False)
            elif field == "Короткое описание":
                db_film.short_descr = film[field]
            elif field == "Длинное описание":
                db_film.full_descr = "".join(film[field])
        db_film.save()