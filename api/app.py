from flask import Flask, jsonify
import psutil
from datetime import date
from pynvml import *
from pynvml.smi import nvidia_smi
import os


app = Flask(__name__)

nvmlInit()
nvsmi = nvidia_smi.getInstance()

handle = nvmlDeviceGetHandleByIndex(0)
nvidiamemoryinfo = nvmlDeviceGetMemoryInfo(handle)
nvidiatemperature = nvmlDeviceGetTemperature(handle,NVML_TEMPERATURE_GPU)
nvidiarates = nvmlDeviceGetUtilizationRates(handle)
nvidiacudaversion = nvmlSystemGetCudaDriverVersion()
nvidiafanspeed = nvmlDeviceGetFanSpeed(handle)

cpufreq = psutil.cpu_freq()

convertbytes = lambda x: round(x / (1024 * 1024 * 1024))

def nvidiainfo() -> str():
    return None

def memoryinfo() -> str():
    return f"{convertbytes(psutil.virtual_memory().total)}GB",\
           f"{convertbytes(psutil.virtual_memory().used)}GB", \
           f"{psutil.virtual_memory().percent}%"
        
        
@app.route('/')
def index(): 
    
    mtotal,mused,mpercent = memoryinfo()
       
    return jsonify(
        date=date.today(),
        memory_total=mtotal,
        memory_used=mused,
        memory_percent=mpercent,
        cpu_percent=f"{psutil.cpu_percent()}%",
        cpu_total_threads=f"{psutil.cpu_count()}",
        cpu_total_cores=f"{psutil.cpu_count(logical=False)}",
        nvidia_driver_version=nvmlSystemGetDriverVersion(),
        nvidia_memory_total=f"{convertbytes(nvidiamemoryinfo.total)}GB",
        nvidia_memory_free=f"{convertbytes(nvidiamemoryinfo.free)}GB",
        nvidia_memory_used=f"{convertbytes(nvidiamemoryinfo.used)}GB",
        nvidia_memory_rate=f"{nvidiarates.memory}%",
        nvidia_gpu_rate=f"{nvidiarates.gpu}%",
        nvidia_fan_speed=f"{nvidiafanspeed}%",
        nvidia_temperature=f"{nvidiatemperature}C",
        nvidia_cuda_version=f"{nvidiacudaversion}",
        cpu_freq_min=f"{cpufreq.min} GHZ",
        cpu_freq_max=f"{cpufreq.max} GHZ",
        cpu_freq_current=f"{cpufreq.current} GHZ",
        system_info=f"{os.popen('uname -a').read()}"
    )


if __name__ == '__main__':
    app.run(debug=True,port=13001)
