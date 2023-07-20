import time
from robproj1 import CosSignal

PERIOD = 1.0
SLEEP = 0.5


def run():
    signal = CosSignal(PERIOD)

    while True:
        try:
            print(signal.get())
            time.sleep(SLEEP)
        except KeyboardInterrupt:
            break


if __name__ == "__main__":
    run()
