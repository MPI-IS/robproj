"""
Python wrappers for the C++ code are in the 'robproj2_wrp' python package
(see CMakeLists.txt and src/wrappers.cpp).
This 'robproj2' package extends it with native python code.
Because it is not fun for the user to import both 'robproj2_wrp' and 'robproj2', 
'robproj2' import the full content of 'robproj2_wrp'. 
"""

import shared_memory
import time
import contextlib
from typing import Generator
import robproj2_wrp
from robproj2_wrp import *  # robproj2 extends robproj2_wrp


DEFAULT_SEGMENT_ID = "cos_signal"
DEFAULT_OBJECT_ID = DEFAULT_SEGMENT_ID
DEFAULT_SEGMENT_ID_PERIODS = f"{DEFAULT_SEGMENT_ID}_PERIODS"


@contextlib.contextmanager
def _get_run_cos_signal(
    cos_period: float,
    run_period: float,
    segment_id: str,
    object_id: str,
    segment_id_periods: str,
) -> Generator[robproj2_wrp.RunCosSignal, None, None]:
    signal = robproj2_wrp.RunCosSignal(
        cos_period, run_period, segment_id, object_id, segment_id_periods
    )
    signal.start()
    yield signal
    signal.stop()


def run_cos_signal(
    cos_period: float,
    run_period: float,
    segment_id: str = DEFAULT_SEGMENT_ID,
    object_id: str = DEFAULT_OBJECT_ID,
    segment_id_periods: str = DEFAULT_SEGMENT_ID_PERIODS,
) -> None:
    """
    Spawn a thread running an instance of RunCosSignal, i.e. something
    that will write in the shared memory values at a period of 'run_period'.
    If your kernel is realtime safe: this thread will be realtime, as
    this method spaws a (python wrapped) c++ thread.
    """
    with _get_run_cos_signal(
        cos_period, run_period, segment_id, object_id, segment_id_periods
    ):
        while True:
            try:
                time.sleep(0.01)
            except KeyboardInterrupt:
                break


def print_period_report(
    period: float,
    tolerance: float,
    segment_id: str = DEFAULT_SEGMENT_ID_PERIODS,
) -> None:
    """
    Convenience wrapper of the c++ function print_report, which
    allows to use a default value for "segment_id" (this could have
    been also implemented in c++)
    """
    robproj2_wrp.print_report(segment_id, period, tolerance)


def get_cos_value(
    segment_id: str = DEFAULT_SEGMENT_ID, object_id: str = DEFAULT_SEGMENT_ID
):
    """
    Read the shared memory for the value written by a thread spawned by
    the "run_cos_signal" function.
    """
    return shared_memory.get_double(segment_id, object_id)
