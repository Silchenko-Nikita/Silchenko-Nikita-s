from django.forms import ChoiceField

from users import forms
from workspace.models import LiteraryComposition


class LiteraryCompositionForm(forms.ModelForm):

   class Meta:
      model = LiteraryComposition
      fields = ('trend', 'title', 'composition')