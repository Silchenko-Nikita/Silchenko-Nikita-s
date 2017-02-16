from django.contrib.auth.decorators import login_required
from django.http import HttpResponseRedirect
from django.shortcuts import render

from workspace.forms import LiteraryCompositionForm
# Create your views here.


@login_required
def sandbox(request):
    if request.method == 'POST':
        comp_form = LiteraryCompositionForm(request.POST)
        if comp_form.is_valid():
            literary_compos = comp_form.save()
            literary_compos.beginner = request.user;
    else:
        comp_form = LiteraryCompositionForm()

    if request.user.literary_compositions:
        print('yes')
    return render(request, 'sandbox.html', {'comp_form': comp_form, 'user': request.user})