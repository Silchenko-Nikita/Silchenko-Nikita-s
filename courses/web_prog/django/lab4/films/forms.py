from django.forms import Form, ModelForm

from films.models import Film


class FilmForm(ModelForm):

    class Meta:
        model = Film
        fields = ('tag', 'name', 'year', 'country',
            'tagline', 'producer', 'genre', 'poster', 'short_descr', 'full_descr')

    def __init__(self, *args, **kwargs):
        super(FilmForm, self).__init__(*args, **kwargs)
        self.fields['short_descr'].widget.attrs['rows'] = 6