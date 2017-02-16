from django.conf.urls import include, url

from users import views
from workspace.views import sandbox

urlpatterns = [
    url(r'^sandbox$', sandbox, name="sandbox"),
]