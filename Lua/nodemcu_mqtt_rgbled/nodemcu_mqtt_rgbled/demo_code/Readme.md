# Demo Code

## nodemcu
Contains the lua files to be uploaded to NodeMCU using luatool. The following
command should work:

python <path/to/luatool/>luatool.py -f <filename>.lua -v

## python
Contains two python MQTT clients used to test and interact with the MQTT
network. Modify the code to work with your particualr configuration. I wrote
them in Python 3 so run them with:

python3 <path/to/file/><filename>.py