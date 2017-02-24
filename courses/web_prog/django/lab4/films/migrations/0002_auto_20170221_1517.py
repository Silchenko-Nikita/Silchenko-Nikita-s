# -*- coding: utf-8 -*-
# Generated by Django 1.10.1 on 2017-02-21 13:17
from __future__ import unicode_literals

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('films', '0001_initial'),
    ]

    operations = [
        migrations.RenameField(
            model_name='film',
            old_name='Жанр',
            new_name='country',
        ),
        migrations.RenameField(
            model_name='film',
            old_name='Длинное_описание',
            new_name='full_descr',
        ),
        migrations.RenameField(
            model_name='film',
            old_name='Название',
            new_name='genre',
        ),
        migrations.RenameField(
            model_name='film',
            old_name='Режиссер',
            new_name='name',
        ),
        migrations.RenameField(
            model_name='film',
            old_name='Постер',
            new_name='poster',
        ),
        migrations.RenameField(
            model_name='film',
            old_name='Слоган',
            new_name='producer',
        ),
        migrations.RenameField(
            model_name='film',
            old_name='Короткое_описание',
            new_name='short_descr',
        ),
        migrations.RenameField(
            model_name='film',
            old_name='Страна',
            new_name='tagline',
        ),
        migrations.RenameField(
            model_name='film',
            old_name='Год',
            new_name='year',
        ),
    ]
