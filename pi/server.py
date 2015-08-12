import logging
from flask import Flask, request, render_template
import tx
import settings
from commands import commands, create_command_array
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
        return render_template('index.html', commands=commands, addresses=receiving_addresses)

    if request.method == 'POST':
        command = request.get_json()
        command_name = command.get('command_name')
        command_addresses = command.get('addresses')
        logging.debug("Received: {}".format(str(command)))

        if command_name == "custom":
            custom = command.get('custom_values')
            if custom is not None:
                custom = [int(i) for i in custom]
                send_all(command_addresses, create_command_array(custom))

        elif command_name in commands.keys():
            send_all(command_addresses, create_command_array(commands[command_name]))

        return "Success"


def send_all(command_addresses, command):
    for addr in command_addresses:
        if addr in receiving_addresses.keys():
            master.send_command(receiving_addresses[addr], command)


if __name__ == '__main__':
    app.run(*settings.HOST)