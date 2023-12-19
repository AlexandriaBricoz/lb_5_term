from flask import Flask
from flask import request
from flask_cors import CORS, cross_origin
import subprocess


app = Flask(__name__)
cors = CORS(app)
app.config['CORS_HEADERS'] = 'Content-Type'

binary_path = '/home/nerdermind/WebstormProjects/lb_5_term/cmake-build-debug/untitled1'


def execute_command(command):
    result = subprocess.run(f"echo '{command}' | {binary_path}", shell=True, stdout=subprocess.PIPE)
    return result.stdout


execute_command('newGraph')


@app.route("/command/", methods=["POST"])
def command():
    print(request.get_json(force=True))
    cmd = request.get_json(force=True)['command']
    result = execute_command(cmd)
    print(result)
    return {"result": result.decode().strip(' \n')}
