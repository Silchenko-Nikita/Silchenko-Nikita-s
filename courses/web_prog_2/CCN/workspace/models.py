from django.db import models

# Create your models here.
from django.contrib.auth.models import User


class LiteraryComposition(models.Model):
    DETECTIVE = 'DV'
    FANTASY = 'FS'
    FANTASTIC = 'FC'
    HISTORICAL = 'HL'
    TREND = (
        (DETECTIVE, 'Detective'),
        (FANTASY, 'Fantasy'),
        (FANTASTIC, 'Fantastic'),
        (HISTORICAL, 'Historical'),
    )
    beginner = models.ManyToOneRel(User, related_name="literary_compositions", field_name='literary_compositions', to='User')
    title = models.CharField("Назва твору:", max_length=100)
    composition = models.TextField("Твір:", blank=True)
    started_at = models.DateTimeField(auto_now_add=True)
    finished_at = models.DateTimeField(auto_now_add=True)
    trend = models.CharField(max_length=2, choices=TREND)

    class Meta:
        db_table = "literary_composition"