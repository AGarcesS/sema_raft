from __future__ import print_function

import sys
import time
from functools import partial

sys.path.append("../")
from pysyncobj import SyncObj, replicated


class TestObj(SyncObj):
    def __init__(self, selfNodeAddr, otherNodeAddrs):
        super(TestObj, self).__init__(selfNodeAddr, otherNodeAddrs)
        self.__database = {}

    @replicated
    def setValue(self, key, value):
        self.__database[key] = value
        return self.__database[key], value

    @replicated
    def getValue(self, key, value):
        return self.__database[key], value

    def getDatabase(self):
        return self.__database


def onAdd(res, err, value):
    print("Entry %d:" % value, res)


if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: %s self_port partner1_port partner2_port ..." % sys.argv[0])
        sys.exit(-1)

    port = int(sys.argv[1])
    partners = ["localhost:%d" % int(p) for p in sys.argv[2:]]
    o = TestObj("localhost:%d" % port, partners)
    n = 0
    old_value = -1
    run = 1
    while run:
        time.sleep(0.5)
        if o.getDatabase() != old_value:
            old_value = o.getDatabase()
            print(old_value)
        if o._getLeader() is None:
            continue
        if n < 18:
            o.setValue(n, n * 2 + 7)
            print(o.getDatabase())
        else:
            run = 0
        n += 1
