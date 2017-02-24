from django import forms
from django.db import models

# Create your models here.
from lab4.settings import STATIC_URL


class Film(models.Model):
    tag = models.SlugField(max_length=128, unique=True)
    name = models.CharField('Название', max_length=128)
    year = models.CharField('Год', max_length=4)
    country = models.CharField('Страна', max_length=128)
    tagline = models.CharField('Слоган', max_length=128)
    producer = models.CharField('Режиссер', max_length=128)
    genre = models.CharField('Жанр', max_length=128)
    poster = models.ImageField('Постер', upload_to='images/posters/')
    short_descr = models.TextField('Короткое описание')
    full_descr = models.TextField('Длинное описание')
