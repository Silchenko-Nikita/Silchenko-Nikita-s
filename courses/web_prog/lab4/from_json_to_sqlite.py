import sqlite3
import json

films = json.load(open('static/data/films.json', "r", encoding='utf-8'))
db = sqlite3.connect('static/data/data.db')

for film in films:
    query = "INSERT INTO films VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"
    c = db.cursor()
    c.execute(query, (film['tag'], film['Название'], film['Год'], film['Страна'],
              film['Слоган'], film['Режиссер'], film['Жанр'], film['Постер'],
              film['Короткое описание'], "".join(film['Длинное описание'])))
    c.close()
    db.commit()
#
# query = "DELETE from films"
# c = db.cursor()
# c.execute(query)
# db.commit()



# query = "SELECT * FROM films"
# c = db.cursor()
# c.execute(query)
# data = c.fetchall()
#
# for d in data:
#     print(d)
#
# c.close()