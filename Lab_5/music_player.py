from mpd import MPDClient
import gpiod
import time

def eventtype_wo_bounce(line, timeout):
	event = 0
	new = False
	ev_line = line.event_wait(sec=timeout)
	while ev_line:
		new = True
		event = line.event_read()
		ev_line = line.event_wait(0,150000000)
	if new == False :
		return event
	else :
		return event.type

def bulk_eventtype_wo_bounce(lines, timeout):
	event = 0
	result = lines.event_wait(sec=timeout)
	if result != None :
		line = result[0]
		read_val = line.event_read()
		olevent = read_val.type
		event = eventtype_wo_bounce(line, timeout)
		if event == 0 :
			event = olevent
		if event == 2 and str(line) == str(lines.to_list()[0]) :
			event = 12
		elif event == 2 and str(line) == str(lines.to_list()[1]) :
			event = 13
		elif event == 2 and str(line) == str(lines.to_list()[2]) :
			event = 14
		elif event == 2 and str(line) == str(lines.to_list()[3]) :
			event = 15
		elif event == 2 and str(line) == str(lines.to_list()[4]) :
			event = 16
	return event

client = MPDClient()
client.idletimeout = None
client.connect("localhost", 6600)
client.play()

def reconnect():
    try:
        client.ping()
    except Exception:
        client.connect("localhost", 6600)
        client.update()

stopped = False

chip = gpiod.Chip('gpiochip1')
led24 = chip.get_line(24)
led24.request(consumer="its_me", type=gpiod.LINE_REQ_DIR_OUT)
led24.set_value(0)

offsets = [12, 13, 14, 15, 16]
buttons = chip.get_lines(offsets)
buttons.request(consumer='its_me', type=gpiod.LINE_REQ_EV_BOTH_EDGES)
while True:
	eventtt = bulk_eventtype_wo_bounce(buttons, 2)
	if eventtt == 12 :
		reconnect()
		client.previous()
	elif eventtt == 13 :
		if stopped == False :
			led24.set_value(1)
			stopped = True
			reconnect()
			client.pause()
		else :
			led24.set_value(0)
			stopped = False
			reconnect()
			client.play()
	elif eventtt == 14 :
		reconnect()
		client.next()
	elif eventtt == 15 :
		reconnect()
		volume1 = (int)(client.status()['volume'])
		if volume1 >= 20 :
			reconnect()
			client.setvol(volume1 - 20)
	elif eventtt == 16 :
		reconnect()
		volume2 = (int)(client.status()['volume'])
		if volume2 <= 80 :
			reconnect()
			client.setvol(volume2 + 20)
