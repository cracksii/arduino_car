import serial
import threading
import time
from dualsense_controller import *

global s

def debug():
    global s
    while True:
        ln = s.readline().decode(encoding="utf-8", errors="ignore")[:-1]
        if(ln != ""):
            print(ln, flush=True)

def send(id, msg=""):
    # print(f"--> {id}{msg}")
    s.write(f"{id}{msg}\n".encode())


class Controller:
    def __init__(self) -> None:
        self.is_running = True
        self.last_x = 0
        self.last_y = 0

        device_infos: list[DeviceInfo] = DualSenseController.enumerate_devices()
        if len(device_infos) < 1:
            raise Exception('No DualSense Controller available.')
        first_device_info: DeviceInfo = device_infos[0]
        self.controller: DualSenseController = DualSenseController(

            device_index_or_device_info=first_device_info,

            # mapping=Mapping.NORMALIZED,
            left_joystick_deadzone=0.1,
            right_joystick_deadzone=0.1,
            # left_trigger_deadzone=0.05,
            # right_trigger_deadzone=0.05,

            mapping=Mapping.NORMALIZED,

            gyroscope_threshold=0,
            accelerometer_threshold=0,
            orientation_threshold=0,

            # update_level=UpdateLevel.PAINSTAKING,
            # update_level=UpdateLevel.HAENGBLIEM,
            update_level=UpdateLevel.DEFAULT,

            microphone_initially_muted=True,
            microphone_invert_led=True,
        )
        self.controller.left_stick_y.on_change(self.left_y)
        self.controller.right_stick_x.on_change(self.right_x)
        self.controller.btn_ps.on_down(self.ps_btn)
    
    def left_y(self, y):
        if self.last_y == y:
            return
        
        if y > 0:
            send("f", round(y * 255))
        elif y < 0:
            send("b", abs(round(y * 255)))
        else:
            send("s")
        self.last_y = y
            

    def right_x(self, x):
        if self.last_x == x:
            return

        if x > 0:
            send("r", round(x * 255))
        elif x < 0:
            send("l", abs(round(x * 255)))
        else:
            send("c")

        self.last_x = x


    def ps_btn(self):
        send("s")
        send("c")
        self.is_running = False


def main():
    global s
    s = serial.Serial("COM3", 115200, timeout=1)
    thr = threading.Thread(target=debug)
    thr.daemon = True
    thr.start()

    c = Controller()

    c.controller.activate()
    while c.is_running:
        time.sleep(1)
    c.controller.deactivate()
    s.close()

if __name__ == '__main__':
    main()
