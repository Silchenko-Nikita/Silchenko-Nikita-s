from django import forms
from django.contrib.auth.decorators import login_required
from django.contrib.auth.models import User
from django.core.checks import messages
from django.db.models import Q
from django.forms import model_to_dict
from django.http import Http404
from django.shortcuts import render, redirect
from django.template.defaulttags import register
from django.urls import reverse
from django.utils.decorators import method_decorator
from django.views.generic import ListView
from django.views.generic.edit import FormView
from django.contrib.auth.forms import UserCreationForm, PasswordResetForm, PasswordChangeForm
from django.contrib.auth.forms import AuthenticationForm
from django.contrib.auth import login, update_session_auth_hash
from django.http import HttpResponseRedirect, request
from django.views.generic.base import View
from django.contrib.auth import logout

# from users.forms import PictureForm
from registration.forms import RegistrationForm
from rest_framework import generics
from rest_framework.decorators import api_view
from rest_framework.response import Response

from CCN.settings import MEDIA_URL
from users.forms import UserProfileAvatarForm, UserProfileInfoForm, RegistrationFormWithName
from users.models import UserSerializer

anonymous_avatar_src = "../.." + MEDIA_URL + "users/anonymous.png"

@api_view(['GET'])
def api_root(request, format=None):
    return Response({
        'users': reverse('user-list'),
    })


class UserList(generics.ListCreateAPIView):
    model = User
    queryset = User.objects.all()
    serializer_class = UserSerializer


class UserDeatail(generics.RetrieveUpdateDestroyAPIView):
    model = User
    queryset = User.objects.all()
    serializer_class = UserSerializer


class RegisterFormView(FormView):
    form_class = RegistrationFormWithName
    template_name = "logreg.html"
    success_url = '/'

    def get_context_data(self, **kwargs):
        context = super(RegisterFormView, self).get_context_data(**kwargs)
        context['submit_val'] = 'Зареєструватися'
        return context

    def form_valid(self, form):
        form.save()

        return super(RegisterFormView, self).form_valid(form)


class LoginFormView(FormView):
    form_class = AuthenticationForm
    template_name = "logreg.html"
    success_url = '/users/home'

    def form_valid(self, form):
        self.user = form.get_user()

        login(self.request, self.user)
        return super(LoginFormView, self).form_valid(form)

    def get_context_data(self, **kwargs):
        context = super(LoginFormView, self).get_context_data(**kwargs)
        context['submit_val'] = 'Увійти'
        context['log'] = True
        return context


class PasswordResetView(FormView):
    form_class = PasswordResetForm
    template_name = "form.html"
    success_url = '/users/home'

    def form_valid(self, form):
        self.user = self.request.user
        # form.cleaned_data['my_form_field_name']
        # form.save()

        return super(PasswordResetView, self).form_valid(form)

    def get_context_data(self, **kwargs):
        context = super(PasswordResetView, self).get_context_data(**kwargs)
        context['submit_val'] = 'Підтвердити скидання'
        return context
#
#
# class PasswordChangeView(FormView):
#     form_class = PasswordChangeForm
#     template_name = "form.html"
#     success_url = '/users/home'
#
#     def form_valid(self, form):
#         self.user = self.request.user
#
#         return super(PasswordChangeView, self.request.user).form_valid(form)
#
#     # def get_context_data(self, **kwargs):
#     #     context = super(PasswordChangeView, self).get_context_data(**kwargs)
#     #     context['submit_val'] = 'Підтвердити зміну'
#     #     return context


class LogoutView(View):

    def get(self, request):
        logout(request)

        return HttpResponseRedirect(reverse("index"))


@login_required
def home_view(request):
    from django.contrib.auth.models import User
    User._meta.get_field('email')._unique = True
    profile = request.user.profile
    if request.method == 'POST':
        avatar_form = UserProfileAvatarForm(request.POST, request.FILES)
        if avatar_form.is_valid():
            profile.avatar = request.FILES['avatar']
            profile.save()
            return HttpResponseRedirect('/users/home')
    else:
        avatar_form = UserProfileAvatarForm()
    profile_info_form = UserProfileInfoForm(data=model_to_dict(profile))
    return render(request, 'home_page.html', {'user': request.user, 'profile': request.user.profile, 'anonymous_avatar_src': anonymous_avatar_src,
                                              'avatar_form': avatar_form, 'MEDIA_URL': MEDIA_URL, 'profile_info_form': profile_info_form})


@login_required
def guest_view(request, username):
    user = User.objects.get(username=username)
    if not user:
        raise Http404
    profile = user.profile
    profile_info_form = UserProfileInfoForm(data=model_to_dict(profile))
    return render(request, 'guest_page.html', {'user': user, 'profile': profile, 'anonymous_avatar_src': anonymous_avatar_src,
                                              'MEDIA_URL': MEDIA_URL, 'profile_info_form': profile_info_form})



@login_required
def search_view(request):
    poss_username = request.GET.get('search')
    if poss_username:
        found_users = User.objects.filter(Q(username__startswith=poss_username) |
                                          Q(first_name__startswith=poss_username) |
                                          Q(last_name__startswith=poss_username))
        searchtag = poss_username
    else:
        found_users = User.objects.all()
        searchtag = ''
    return render(request, 'search.html', {'user_list': User.objects.all(), 'MEDIA_URL': MEDIA_URL, 'anonymous_avatar_src': anonymous_avatar_src,
                                           'found_users': found_users, 'searchtag': searchtag})


@login_required
def password_change(request):
    if request.method == 'POST':
        form = PasswordChangeForm(request.user, data=request.POST)
        if form.is_valid():
            form.save()
            update_session_auth_hash(request, form.user)
            # messages.success(request, "Password changed.")
            return redirect("/")
    else:
        form = PasswordChangeForm(request.user)
    data = {
        'form': form
    }
    return render(request, "form.html", data)


@register.filter
def cut(value, arg):
    return value.replace(arg, '')