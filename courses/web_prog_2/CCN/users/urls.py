from django.conf.urls import include, url
from django.contrib.admin import register

from users import views

urlpatterns = [
    url(r'^login$', views.LoginFormView.as_view(), name="login"),
    url(r'^register$', views.RegisterFormView.as_view(), name="register"),
    url(r'^logout$', views.LogoutView.as_view(),  name="logout"),
    url(r'^password_change$', views.password_change,  name="password_change"),
    url(r'^guest/(?P<username>\w+)$', views.guest_view,  name="guest"),
#    url(r'^password_reset$', views.password_reset,  name="password_reset"),
    url(r'^home$', views.home_view,  name="home"),
    url(r'^search$', views.search_view,  name="search"),
]