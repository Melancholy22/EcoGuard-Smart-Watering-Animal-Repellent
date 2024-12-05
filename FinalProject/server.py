from flask import Flask, Response
from flask import request
import matplotlib.pyplot as plt
import io
from datetime import datetime

app = Flask(__name__)
temps = []
time = []
moists = []


@app.route('/plot')
def plot_graph():
    

    plt.figure(figsize=(12, 6))
    fig, axs = plt.subplots(2, 1, figsize=(12, 8))
    axs[0].plot(time, temps, label="Temp vs Time", color="blue", marker="o")
    axs[0].set_xlabel("Time")
    axs[0].set_ylabel("Temperature")
    axs[0].set_title("Flask Generated Plot")
    axs[0].legend()
    axs[0].set_ylim(0, 30)
    axs[0].tick_params(axis='x', which='both', bottom=False, top=False, labelbottom=False)

    axs[1].plot(time, moists, label="Moist vs Time", color="red", marker="o")
    axs[1].set_xlabel("Time")
    axs[1].set_ylabel("Moisture level")
    axs[1].set_title("Flask Generated Plot")
    axs[1].legend()
    axs[1].set_ylim(0, 1)
    axs[1].tick_params(axis='x', which='both', bottom=False, top=False, labelbottom=False)
    

    plt.tight_layout() 
    
    buffer = io.BytesIO()
    plt.savefig(buffer, format='png')
    buffer.seek(0)
    plt.close()  

    return Response(buffer, mimetype='image/png')
@app.route("/")

def hello():
    print(request.args.get("temp"))
    temp = float(request.args.get("temp"))
    moist = (3000 - float(request.args.get("moist")))/3000
    time.append(datetime.now().strftime("%H:%M:%S"))
    temps.append(temp)
    moists.append(moist)

    
    return "We received data"
