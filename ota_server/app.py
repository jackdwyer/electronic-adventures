from flask import Flask, send_from_directory
# app = Flask(__name__, static_url_path='./firmware')
app = Flask(__name__, static_folder='firmware')


@app.route('/status')
def status():
    return 'yah status'


@app.route('/file.bin')
def index():
    return app.send_static_file('lights.bin')


if __name__ == '__main__':
    app.run(host='0.0.0.0', debug=True)
