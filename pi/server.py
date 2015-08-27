import logging
from flask import Flask, request, render_template, jsonify
from flask.json import dumps
import tx
import settings
from commands import commands, create_command_array, command_parameters
from addresses import receiving_addresses


app = Flask(__name__)
master = tx.Master()


logging.basicConfig(filename=settings.LOG_FILE_PATH,
                    format='%(asctime)s %(levelname)s: %(message)s',
                    level=logging.DEBUG)
logging.getLogger().addHandler(logging.StreamHandler())


@app.route('/', methods=['GET', 'POST'])
def index():
    if request.method == 'GET':
        return render_template('index.html', commands=commands, addresses=receiving_addresses, command_parameters=command_parameters)

    if request.method == 'POST':
        command = request.get_json()
        command_name = command.get('command_name')
        command_addresses = command.get('addresses')
        logging.debug("Received: {}".format(str(command)))
        results = {}

        if command_name == "custom":
            custom = command.get('custom_values')
            if custom is not None:
                custom = [int(i) for i in custom]
                results = send_all(command_addresses, create_command_array(custom))

        elif command_name in commands.keys():
            results = send_all(command_addresses, create_command_array(commands[command_name]))

        return jsonify(**results)


@app.route('/api/parameters/<command_name>', methods=['GET', ])
def get_params(command_name):
    if request.method == 'GET':
        print(command_name)
        params = command_parameters.get(command_name)
        if params is not None:
            return dumps(params)
    return "Command not found"


def send_all(command_addresses, command):
    results = {}
    for addr in command_addresses:
        if addr in receiving_addresses.keys():
            result = master.send_command(receiving_addresses[addr], command)
            results[addr] = result
    return results

if __name__ == '__main__':
    app.run(*settings.HOST,debug=True)