from rest_framework import serializers

from films.models import Film


class FilmSerializer(serializers.ModelSerializer):

    class Meta:
        model = Film
        fields = ('pk', 'tag', 'name', 'year', 'country',
            'tagline', 'producer', 'genre', 'poster', 'short_descr', 'full_descr')
        extra_kwargs = {
            'url': {'lookup_field': 'tag'},
            'users': {'lookup_field': 'tag'}
        }