from django.shortcuts import render
import json
# Create your views here.
def index_view(request):
    return render(request,'main/search_box.html')

def search_view(request):

    with open('score.json', 'r') as f:
    	score = json.load(f)

    with open('data.json', 'r') as f:
    	data = json.load(f)

    x_step = 500
    x_max = 3000
    y_step = 300
    y_max = 1500
    x_n = int(x_max/x_step)
    y_n = int(y_max/y_step)
    x = score[0]['x']/x_step
    y = score[0]['y']/y_step
    pos = int(x*y_n+y)
    print("Fetching data at position", pos ,"...")
    #print(data[pos])
    data[pos] = score[0]
    #print(data[pos])

    with open('data.json', 'w') as outfile:
    	json.dump(data, outfile)

    return render(request,'main/result.html',{'score_data':score})

def list_view(request):

	with open('data.json', 'r') as f:
		data = json.load(f)

	return render(request,'main/list.html',{'score_data':data})