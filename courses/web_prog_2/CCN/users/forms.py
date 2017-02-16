from django import forms
from django.contrib.auth.forms import UsernameField
from django.contrib.auth.models import User
from django.core.files.images import get_image_dimensions
from django.forms import ModelForm
from registration.forms import RegistrationForm

from users.models import UserProfile


class UserProfileAvatarForm(forms.ModelForm):

   class Meta:
      model = UserProfile
      fields = ('avatar',)


class UserProfileInfoForm(forms.ModelForm):

   class Meta:
      model = UserProfile
      fields = ('birthday', 'language')


class RegistrationFormWithName(RegistrationForm):

   class Meta:
      model = User
      fields = ("username", "first_name", "last_name")
      field_classes = {'username': UsernameField}

   def __init__(self, *args, **kwargs):
      super(RegistrationFormWithName, self).__init__(*args, **kwargs)
      self.fields['first_name'].required = True
      self.fields['last_name'].required = True

   def clean(self):
      data = self.cleaned_data['email']
      if not self.cleaned_data['first_name'] or self.cleaned_data['last_name']:
         raise forms.ValidationError("")
      if User.objects.filter(email=self.cleaned_data['email']).exists():
         raise forms.ValidationError("Такий email вже використовується")
      return data

   # def save(self, commit=False):
   #    user = super(RegistrationFormWithName, self).save(commit=False)
   #    user.email = self.cleaned_data['email']
   #    if commit:
   #       user.save()
   #    return user

   # def __init__(self, *args, **kwargs):
   #    super(RegistrationForm, self).__init__(*args, **kwargs)
   #    self.fields.keyOrder = ['first_name', 'username', 'last_name', 'email']


# class UserProfileForm(forms.ModelForm):
#     class Meta:
#         model = UserProfile
#
#     def clean_avatar(self):
#         avatar = self.cleaned_data['avatar']
#
#         try:
#             w, h = get_image_dimensions(avatar)
#
#             #validate dimensions
#             max_width = max_height = 100
#             if w > max_width or h > max_height:
#                 raise forms.ValidationError(
#                     u'Please use an image that is '
#                      '%s x %s pixels or smaller.' % (max_width, max_height))
#
#             #validate content type
#             main, sub = avatar.content_type.split('/')
#             if not (main == 'image' and sub in ['jpeg', 'pjpeg', 'gif', 'png']):
#                 raise forms.ValidationError(u'Please use a JPEG, '
#                     'GIF or PNG image.')
#
#             #validate file size
#             if len(avatar) > (20 * 1024):
#                 raise forms.ValidationError(
#                     u'Avatar file size may not exceed 20k.')
#
#         except AttributeError:
#             """
#             Handles case when we are updating the user profile
#             and do not supply a new avatar
#             """
#             pass
#
#         return avatar