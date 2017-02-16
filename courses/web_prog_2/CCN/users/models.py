import os

from django.conf.global_settings import LANGUAGES, MEDIA_URL
from django.contrib.auth.models import User
import django.db.models as models
from django.db.models.signals import post_save
from django.dispatch import receiver
from rest_framework import serializers

def get_avatar_path(instance, filename):
    return os.path.join('users', str(instance.id), filename)



class UserProfile(models.Model):
    user = models.OneToOneField(User, related_name="profile", unique=True)
    birthday = models.DateField(blank=True, null=True)
    language = models.CharField(max_length=2, choices=LANGUAGES, default='uk')
    avatar = models.ImageField(upload_to=get_avatar_path, default=MEDIA_URL+'users/anonymous.png', blank=True, null=True)
    # created_at = models.DateTimeField(auto_now_add=True)
    # updated_at = models.DateTimeField(auto_now=True)

    class Meta:
        db_table = "user_profile"


class UserSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = User
        fields = ('username', 'email')



@receiver(post_save, sender=User)
def create_user_profile(sender, instance, created, **kwargs):
    if created:
        UserProfile.objects.create(user=instance)


@receiver(post_save, sender=User)
def save_user_profile(sender, instance, **kwargs):
    instance.profile.save()