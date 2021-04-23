#!/usr/bin lua

print("Downloading update.tar.gz...")
local http = require("socket.http")
local socket = require("socket")
local ltn12 = require("ltn12")
local host, port = "10.0.2.2", 8000

local file = ltn12.sink.file(io.open('/etc/update.d/update.tar.gz', 'w+b'))
local body, code = http.request{
    url = "http://10.0.2.2:8000/update.tar.gz",
    sink = file,
}
if not body then 
    print("No update found")
else 
    print(code)
    print("Update found")

    os.execute("tar --overwrite -xvf /etc/update.d/update.tar.gz -C /etc/update.d")
    local f = io.open('/etc/update.d/update.txt', 'rb')
    local content = f:read("*all")
    print(content)
    f:close()
end
