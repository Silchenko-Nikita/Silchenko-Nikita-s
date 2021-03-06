# -*- coding: utf-8 -*-
# Generated by Django 1.10.1 on 2017-02-21 11:42
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Film',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('tag', models.CharField(max_length=128)),
                ('Название', models.CharField(max_length=128)),
                ('Год', models.CharField(max_length=4)),
                ('Страна', models.CharField(max_length=128)),
                ('Слоган', models.CharField(max_length=128)),
                ('Режиссер', models.CharField(max_length=128)),
                ('Жанр', models.CharField(max_length=128)),
                ('Постер', models.ImageField(upload_to='')),
                ('Короткое_описание', models.TextField()),
                ('Длинное_описание', models.TextField()),
            ],
        ),
    ]
