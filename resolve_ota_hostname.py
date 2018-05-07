Import('env')
import socket
from base64 import b64decode

try:
    hostname = b64decode(ARGUMENTS.get("UPLOAD_HOSTNAME"))
    ip = socket.gethostbyname(hostname)

    env.Replace(UPLOAD_PORT=ip)

    print("Changed upload port to " + ip + " (resolved from '" + hostname + "')")
except:
    pass
