# -*- coding: utf-8 -*-
# Generated by Django 1.10.1 on 2016-12-04 08:37
from __future__ import unicode_literals

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('users', '0001_initial'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='userprofile',
            name='created_at',
        ),
        migrations.RemoveField(
            model_name='userprofile',
            name='height',
        ),
        migrations.RemoveField(
            model_name='userprofile',
            name='updated_at',
        ),
        migrations.RemoveField(
            model_name='userprofile',
            name='weight',
        ),
        migrations.AlterModelTable(
            name='userprofile',
            table='user_profile',
        ),
    ]
