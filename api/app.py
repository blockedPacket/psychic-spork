from flask import Flask, jsonify
import psutil
from datetime import date

app = Flask(__name__)

convertbytes = lambda x: round(x / (1024 * 1024 * 1024))

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
        cpu_total_cores=f"{psutil.cpu_count(logical=False)}"
    )


if __name__ == '__main__':
    app.run(debug=True,port=13000)
