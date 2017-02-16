"""CCN URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/1.10/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  url(r'^$', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  url(r'^$', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.conf.urls import url, include
    2. Add a URL to urlpatterns:  url(r'^blog/', include('blog.urls'))
"""
from django.conf.urls import url, include
from django.conf.urls.static import static
from django.contrib import admin
from django.contrib.admin import register
from django.views.generic import TemplateView
from rest_framework.urlpatterns import format_suffix_patterns

from CCN import settings
from users.views import api_root, UserList
urlpatterns = [
    url(r'^api/$', api_root),
    url(r'^users/$', UserList.as_view(), name='user-list'),
    url(r'^admin/', include(admin.site.urls)),
    url(r'^users/', include("users.urls")),
    url(r'^workspace/', include("workspace.urls")),
    url(r'^password_reset/', include('password_reset.urls'), name="password_reset"),
    url(r'^$', TemplateView.as_view(template_name='index.html'), name='index'),
    url(r'^avatar/', include('avatar.urls')),
    url(r'^api-auth/', include('rest_framework.urls', namespace='rest_framework')),
] + static(settings.MEDIA_URL, document_root=settings.MEDIA_ROOT)


urlpatterns = format_suffix_patterns(urlpatterns, allowed=['json', 'api'])

