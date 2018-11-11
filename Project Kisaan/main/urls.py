from django.conf.urls import url,include
from . import views
from django.conf.urls.static import static

urlpatterns = [
    url(r'^$',views.index_view,name='index_main'),
    url(r'search/',views.search_view,name='search_view'),
    url(r'list/',views.list_view,name='list_view')
]