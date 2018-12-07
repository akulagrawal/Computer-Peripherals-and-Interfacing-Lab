from django.http import HttpResponse
from django.shortcuts import render
import json
from .forms import NameForm
import paho.mqtt.publish as publish
# Create your views here.
def index_view(request):
    return render(request,'main/search_box.html')

def search_view(request):

    try:
        with open('sensordata.json', 'r') as f:
            score = json.load(f)
    except:
        with open('score.json', 'r') as f:
           score = json.load(f)

    with open('data.json', 'r') as f:
    	data = json.load(f)

    with open('score.json', 'w') as outfile:
        json.dump(score, outfile)

    x_step = 500
    x_max = 2500
    y_step = 300
    y_max = 1200
    x_n = int(x_max/x_step)
    y_n = int(y_max/y_step)
    x = score['x']/x_step
    y = score['y']/y_step
    pos = int(x*y_n+y)
    print("Fetching data at position", pos ,"...")
    #print(data[pos])
    data[pos] = score
    #print(data[pos])

    with open('data.json', 'w') as outfile:
    	json.dump(data, outfile)

    return render(request,'main/result.html',{'score_data':score})

def list_view(request):

    with open('data.json', 'r') as f:
        data = json.load(f)

    return render(request,'main/list.html',{'score_data':data})

def search_form(request):
    return render(request, 'main/search_form.html')

def search(request):
    mode = request.GET['mode']
    x = request.GET['x']
    y = request.GET['y']
    z = request.GET['z']
    while len(x) < 4:
        x = "0" + x
    while len(y) < 4:
        y = "0" + y
    while len(z) < 4:
        z = "0" + z

    with open('coords.txt', 'w') as f:
        f.write(mode + "," + x + "," + y + "," + z)

    try:
        with open('sensordata.json', 'r') as f:
           score = json.load(f)
    except:
        with open('score.json', 'r') as f:
           score = json.load(f)

    with open('data.json', 'r') as f:
        data = json.load(f)

    with open('score.json', 'w') as outfile:
        json.dump(score, outfile)

    x_step = 500
    x_max = 2500
    y_step = 300
    y_max = 1200
    x_n = int(x_max/x_step)
    y_n = int(y_max/y_step)
    x = score['x']/x_step
    y = score['y']/y_step
    pos = int(x*y_n+y)
    print("Fetching data at position", pos ,"...")
    #print(data[pos])
    data[pos] = score
    #print(data[pos])

    with open('data.json', 'w') as outfile:
        json.dump(data, outfile)

    MQTT_SERVER = "10.42.0.1"
    MQTT_PATH = "test"

    with open ("coords.txt", "r") as f:
        inputstr = f.read()

    publish.single(MQTT_PATH, inputstr, hostname=MQTT_SERVER)

    return render(request,'main/result.html',{'score_data':score})