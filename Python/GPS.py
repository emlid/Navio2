import navio.util
from navio.ublox import Ublox


if __name__ == "__main__":
    ubl = Ublox()
    navio.util.check_apm()

    for ind in range(0, 10):
        ubl.enable_posllh()

    while(1):
        buffer = ubl.bus.xfer2([100])
        for byt in buffer:
            ubl.scan_ubx(byt)
            if not ubl.mess_queue.empty():
                msg = ubl.parse_ubx()
                if (msg != None):
                    print(msg)
