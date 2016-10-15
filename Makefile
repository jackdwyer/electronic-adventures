tempsensor:
	arduino --upload TempDisplay/TempDisplay.ino --board esp8266:esp8266:nodemcuv2 --port /dev/ttyUSB0

# --board should reflect generic espt
wsled:
	arduino --upload WSLed/WSLed.ino --board esp8266:esp8266:generic --port /dev/ttyUSB0

console:
	miniterm.py /dev/ttyUSB0 115200

console9600:
	miniterm.py /dev/ttyUSB0 9600

boot:
	miniterm.py /dev/ttyUSB0 74880
