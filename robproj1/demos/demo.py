import time
from robproj1 import CosSignal

# The class CosSignal is a C++ class
# (see robproj1/include/robproj1/cos_signal.hpp)
# which has been binded for python. See
# robproj1/srcpy/wrappers.cpp and robproj1/CMakeLists.txt
# (but robproj2 does something similar and its CMakeLists.txt
# file has more comments)

# Here you may see these bindings in action.
# To run this demo, simply run:
# 'python ./demo.py'

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
