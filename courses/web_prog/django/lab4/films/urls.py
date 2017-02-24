from django.conf.urls import url, include
from rest_framework import routers
from rest_framework.urlpatterns import format_suffix_patterns

from films import views
from films.views import films_table, film_full_descr, FilmList, FilmDetail, FilmListView, open_page, add_film, del_film

urlpatterns = [
    url(r'^api/$', FilmList.as_view(), name='films-list'),
    url(r'^api/(?P<pk>\d+)/$', FilmDetail.as_view(), name='tag_films-detail'),

    url(r'add_film/$', add_film, name="add_film"),
    url(r'del_film/(?P<pk>\d+)/$', del_film, name="del_film"),

    url(r'(?P<film_tag>\w+)$', film_full_descr, name="film_full_descr"),
    url(r'go/(?P<name>\w*)/(?P<page>\d+)/$', open_page, name="open_page"),
    url(r'$', FilmListView.as_view(), name="films_list"),

]

urlpatterns = format_suffix_patterns(urlpatterns, allowed=['json', 'api'])
