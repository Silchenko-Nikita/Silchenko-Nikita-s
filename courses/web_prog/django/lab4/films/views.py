from django.core import exceptions
from django.http import HttpResponse
from django.http import HttpResponseNotFound
from django.shortcuts import render, redirect
from django.urls import reverse
from django.views.generic import ListView
from rest_framework import generics
from rest_framework import status
from rest_framework import viewsets
from rest_framework.decorators import api_view
from rest_framework.response import Response

from films.forms import FilmForm
from films.from_json_to_db import run_script
from films.models import Film
from films.serializers import FilmSerializer

from django.core.paginator import Paginator, EmptyPage, PageNotAnInteger


class FilmListView(ListView):
    model = Film
    paginate_by = 2
    context_object_name = 'films'
    template_name = 'films_list.html'

    def get_queryset(self):
        return Film.objects.filter(name__icontains=self.request.GET.get('name', ''))

    def get_context_data(self, **kwargs):
        context = super(FilmListView, self).get_context_data(**kwargs)
        name = self.request.GET.get('name', '')
        if name:
            if self.get_queryset():
                found = True
            else:
                found = False
        else:
            found = True
        context['found'] = found
        context['search_name'] = name
        context['form'] = FilmForm()
        return context

class FilmList(generics.ListCreateAPIView):

    model = Film
    queryset = Film.objects.all()
    serializer_class = FilmSerializer


class FilmDetail(generics.RetrieveUpdateDestroyAPIView):

    model = Film
    queryset = Film.objects.all()
    serializer_class = FilmSerializer


def films_table(request):
    # run_script()
    films = Film.objects.all()

    return render(request, 'films_list.html', {"films": films})


def open_page(request, name, page):
    return redirect('{0}?name={1}&page={2}'.format(reverse('films_list'), name, page) if name
                    else '{0}?page={1}'.format(reverse('films_list'), page))


def film_full_descr(request, film_tag):
    try:
        film = Film.objects.get(tag=film_tag)
    except:
        return HttpResponseNotFound('film with such tag not found')
    return render(request, 'full_descr.html', {'film': film})


def add_film(request):
    if request.method == "POST":
        form = FilmForm(request.POST, request.FILES)
        if form.is_valid():
            form.save()
    return redirect(reverse('films_list'))


def del_film(request, pk):
    if request.method == "POST":
        try:
            Film.objects.get(pk=pk).delete()
        except:
            pass
    return redirect(reverse('films_list'))

# def look_for_film(request):
#     try:
#         film = Film.objects.get(name=request.GET['name'])
#     except:
#         film = None
#     if film:
#         return render(request, 'full_descr.html', {'film': film, 'found': True})
#     else:
#         return render(request, 'films_list.html', {'found': False, 'objects_list': Film.objects.all()})