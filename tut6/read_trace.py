#!/usr/local/bin/python3

import subprocess
from pprint import pprint
import binascii
from collections import defaultdict
import math
import time
from timeit import Timer
import timeit
import cProfile
import profile
import re


def timing(f):
    def wrap(*args):
        time1 = time.time()
        ret = f(*args)
        time2 = time.time()
        print('%s function took %0.3f ms' % (f.__name__,
                                             (time2 - time1) * 1000.0))
        return ret

    return wrap


mdw_encoding = {
    '000': 'Interrupt Acknowledge',
    '001': 'halt/special cycle',
    '010': 'I/O read',
    '011': 'I/O write',
    '100': 'Instruction Read',
    '101': 'Reserved',
    '110': 'data read',
    '111': 'data write',
}


def readlines_bytes(filename, lines, **kwargs):
    if lines is None:
        args = ["xxd", "-g", "4", "-c", "4", "-b", "-s", "+1677715", filename]
    else:
        args = ["xxd", "-g", "4", "-c", "4", "-b", "-l", str(lines), filename]

    data = subprocess.run(
        args=args, stdout=subprocess.PIPE).stdout.split(b'\n')[0:-1:2]

    lines = [int(r.split(b' ')[1], 2) for r in data][0::2]
    return lines


# def readlines_string(filename, lines, **kwargs):
#     if lines is None:
#         args = ["xxd", "-g", "4", "-c", "4", "-b", filename]
#     else:
#         args = ["xxd", "-g", "4", "-c", "4", "-b", "-l", str(lines), filename]

#     data = subprocess.run(
#         args=args, stdout=subprocess.PIPE).stdout.decode().split('\n')[0:-1:2]

#     lines = [int(r.split(' ')[1], 2) for r in data]
#     return lines


def next_address_in_burst(pair):
    global masks
    print('address')
    address = pair[0] & masks['address'][1]
    print(address)
    print(bin(address))
    # burst =
    print('next address')
    next_address = (address & 15) | ((address + 1) & 15)
    print(next_address)
    print(bin(next_address))

    print('SHIFT')
    print('address')
    address = pair[0] & masks['address'][1]
    address = address << 2
    print(address)
    print(bin(address))
    # burst =
    print('next address')
    next_address = (address & 15) | ((address + 1) & 15)
    print(next_address)
    print(bin(next_address))


class cach_tag_set(object):
    """a set from the tag table"""

    def __init__(self, L, K, N):
        self._K = K
        self._L = L
        self._N = N
        # self.dirs = {
        #     -1 - i: {
        #         'last': -1 - i,
        #         'dir': i
        #     }
        #     for i in range(self._K)
        # }
        self.dirs = {-1 - i: -1 - i for i in range(self._K)}
        # self.dirs = [-1 - i for i in range(self._K)]
        self.newest = 1

    def find_oldest(self):
        min_last = self.newest
        min_tag = None
        for tag, entry in self.dirs.items():
            if entry['last'] < min_last:
                min_last = entry['last']
                min_tag = tag

        return min_last, min_tag

    def add_item(self, new_tag):
        # chose oldest
        min_last, min_tag = self.find_oldest()
        old = self.dirs.pop(min_tag)
        self.dirs[new_tag] = {**old, **{'last': self.newest}}
        self.newest += 1
        return old['dir']

    def get_item(self, tag):
        try:
            self.dirs[tag] = self.newest
            # self.dirs[tag]['last'] = self.newest
            self.newest += 1
            # print('cache hit')
            return True  # , self.dirs[tag]['dir']
        except KeyError as e:
            # print('cache miss')
            # find_oldest
            min_last = self.newest
            min_tag = None
            for t, entry in self.dirs.items():
                if entry < min_last:
                    min_last = entry
                    min_tag = t
            # add_item
            self.dirs.pop(min_tag)
            # old = self.dirs.pop(min_tag)
            self.dirs[tag] = self.newest
            # self.dirs[tag] = {**old, **{'last': self.newest}}
            self.newest += 1
            # self.add_item(tag)
            return False  # , self.add_item(tag)


def bprint(num):
    print(num)
    print("{0:032b}".format(num))


class lnk_cache(object):
    def __init__(self, L, K, N):
        self._L = L
        self._K = K
        self._N = N

        self._L_bits = int(math.log(L, 2))
        self._K_bits = int(math.log(K, 2))
        self._N_bits = int(math.log(N, 2))

        # print('L')
        # print(self._L)
        # print(self._L_bits)

        # print('K')
        # print(self._K)
        # print(self._K_bits)

        # print('N')
        # print(self._N)
        # print(self._N_bits)

        self.make_masks()

        self.offset_mask = '0' * (23 - self._L_bits) + '1' * self._L_bits
        self.offset_mask_int = int(self.offset_mask, 2)

        self.set_mask = '0' * (23 - self._L_bits - self._N_bits) + \
            '1' * self._N_bits + '0' * self._L_bits
        self.set_mask_int = int(self.set_mask, 2)

        self.tag_mask = '1' * \
            (23 - (self._L_bits + self._N_bits)) + \
            '0' * (self._L_bits + self._N_bits)
        self.tag_mask_int = int(self.tag_mask, 2)

        self.newest = [1 for set_num in range(self._N)]
        self.tags = {
            set_num: {-1 - i: -1 - i
                      for i in range(self._K)}
            for set_num in range(self._N)
        }

    def get_(self, set_num, tag, offset):
        _set = self.tags[set_num >> self._L_bits]
        hit, dir_num = _set.get_item(tag)
        return hit

    def access(self, word0, tag_mask_int, set_mask_int):
        address = word0 & address_mask
        _set = address & set_mask_int >> 16
        # _set = address & set_mask_int >> self._L_bits
        _tag = address & tag_mask_int
        try:
            self.tags[_set][_tag] = self.newest[_set]
            self.newest[_set] += 1
            return ((word0 & burst_mask) >> 27) + 1, 0
        except KeyError as e:
            newest = self.newest[_set]
            min_last = newest
            # min_tag = None
            for t, entry in self.tags[_set].items():
                if entry < min_last:
                    min_last = entry
                    min_tag = t
            del self.tags[_set][min_tag]
            self.tags[_set][_tag] = newest
            self.newest[_set] += 1
            return ((word0 & burst_mask) >> 27), 1

    def print_mask(self, m):
        print(m)
        print(len(m))
        print(int(m, 2))
        print()

    def store_mask(self, m):
        # self.print_mask(m)
        return m, int(m, 2)

    def make_masks(self):
        self.masks = {}
        # self.masks = {}
        # print()
        # print('making self.masks')

        # print('address')
        address = '0' * 3 + '0' * 2 + '0' * 4 + '1' * 23
        self.masks['address'] = self.store_mask(address)

        # print('BE')
        BE = '0' * 3 + '0' * 2 + '1' * 4 + '0' * 23
        self.masks['BE'] = self.store_mask(BE)

        # print('Burst')
        burst = '0' * 3 + '1' * 2 + '0' * 4 + '0' * 23
        self.masks['burst'] = self.store_mask(burst)

        # print('mdw')
        mdw = '1' * 3 + '0' * 29
        self.masks['mdw'] = self.store_mask(mdw)

        # print('interval')
        interval = '0' * 24 + '1' * 8
        self.masks['interval'] = self.store_mask(interval)

        return self.masks


def setup_cache():
    result = readlines_bytes('gcc1.trace', None)

    ci = lnk_cache(16, 1, 1024)
    cd = lnk_cache(16, 8, 256)
    masks = ci.masks
    return result, ci, cd, masks


def test_cache(result, ci, cd, masks):

    hits = 0
    misses = 0
    skipped = 0

    for i, r in enumerate(result):
        mdw = (r & mdw_mask) >> 30
        if mdw == 2:
            # pi.enable()
            hit, miss = ci.access(r, ci_tag_mask, ci_set_mask)
            # pi.disable()
        elif mdw == 3:
            # pd.enable()
            hit, miss = cd.access(r, cd_tag_mask, cd_set_mask)
            # pd.disable()
        else:
            # ps.enable()
            skipped += 1
            # ps.disable()
            continue
        # print('HIT:', hit)
        hits += hit
        misses += miss
    # print('INSTRUCTIONS')
    # pi.print_stats(sort='time')
    # print('DATA')
    # pd.print_stats(sort='time')
    # print('SKIPPED')
    # ps.print_stats(sort='time')

    return hits, misses, skipped  # , first_non_skip_4, first_non_skip_67
    # print('TOTAL HITS')
    # print(hits)
    # print('TOTAL MISSES')
    # print(misses)
    # print('PERCENTAGE')
    # if hits + misses > 0:
    #     print(hits / (hits + misses))
    # else:
    #     print('not run')

    # print('Non hits')
    # print('4:', first_non_skip_4)
    # print('67:', first_non_skip_67)


if __name__ == "__main__":
    # pr = cProfile.Profile()
    # pr.enable()
    # pr.disable()
    # pr.print_stats(sort='time')
    # for i in range(2):
    # print('run:', i)
    # test_timer = Timer("test_cache()", "from __main__ import test_cache")
    # test_time = test_timer.timeit(number=1)
    # print('time', test_time)

    # test_cache()
    # time cache
    result, ci, cd, masks = setup_cache()
    address_mask = masks['address'][1]
    burst_mask = masks['burst'][1]
    mdw_mask = masks['mdw'][1]
    ci_tag_mask = int(ci.tag_mask_int)
    ci_set_mask = int(ci.set_mask_int)
    cd_tag_mask = int(cd.tag_mask_int)
    cd_set_mask = int(cd.set_mask_int)
    print('starting timer')
    t = timeit.timeit(
        'test_cache(result, ci, cd, masks)', number=1, globals=globals())
    print('done')
    print(t)
    # pr = cProfile.Profile()
    # pi = cProfile.Profile()
    # pd = cProfile.Profile()
    # ps = cProfile.Profile()
    # pa = cProfile.Profile()
    # pr.enable()
    # hits, misses, skipped = test_cache(result, ci, cd, masks)
    # pr.disable()
    # print('ACESS')
    # pa.print_stats(sort='time')
    # print('TOTALS')
    # pr.print_stats(sort='time')

    # # time cache setup
    # t = timeit.timeit(setup_cache, number=1)
    # print(t)

    # time lnk init
    # t = timeit.timeit(
    #    'lnk_cache(Li, Ki, Ni)',
    #    number=1,
    #    setup='Li = 16; Ki = 1; Ni = 1024',
    #    globals=globals())
    # print(t)

    # time readline
    # t = timeit.timeit(
    #    'readlines_bytes(filename, limit)',
    #    number=1,
    #    setup='filename="gcc1.trace"; limit=None',
    #    globals=globals())
    # print(t)

    # profile.run('re.compile("test_cache()")')
    # print(ts_time)
    # times[0].append(ts_time)
    # sums[0] += ts_time

    # tb = Timer("readlines_string('gcc1.trace', 16)",
    # "from __main__ import readlines_string")
    # tb_time = tb.timeit(number=1)
    # print(tb_time)
    # times[1].append(tb_time)
    # sums[1] += tb_time

    # print('BEST')
    # print('string:', min(times[0]))
    # print('byte:', min(times[0]))
    # print('WORST')
    # print('string:', max(times[0]))
    # print('byte:', max(times[0]))
    # print('AVERAGE')
    # print('string:', sums[0] / 100)
    # print('byte:', sums[0] / 2)

    # ci = lnk_cache(16, 1, 1024)
    # cd = lnk_cache(16, 8, 256)
    # masks = ci.masks
    # # print(result[0])
    # # print(*result[0])
    # # print(type(result[0][0]))

    # # hit = ci.access(*result[0])
    # # hit = ci.access(*result[0])
    # hits = 0
    # misses = 0
    # for r in result:
    #     print('word0')
    #     bprint(r[0])
    #     print('mdw')
    #     mdw = (r[0] & masks['mdw'][1]) >> 29
    #     bprint(mdw)
    #     if mdw == 4:
    #         hit, miss = ci.access(*r)
    #     elif mdw == 6 or mdw == 7:
    #         hit, miss = cd.access(*r)
    #     else:
    #         print('skipping')
    #         continue
    #     print('HIT:', hit)
    #     hits += hit
    #     misses += miss
    # print('TOTAL HITS')
    # print(hits)
    # print('TOTAL MISSES')
    # print(misses)
    # print('PERCENTAGE')
    # if hits + misses > 0:
    #     print(hits / (hits + misses))
    # else:
    #     print('not run')

    # _set, directory = ca.check_tags(2, 2)
    # print('set')
    # pprint(_set)
    # hit = ca.get_(1, 1, 1)
    # print(hit)
    # hit = ca.get_(1, 1, 1)
    # print(hit)

    # _set, directory = ca.check_tags(1, 1)
    # data = ca.get_data(directory, 8)

    # result = readlines('gcc1.trace')
    # pprint(result)

    # print('masks')
    # masks = make_masks()
    # pprint(masks)

    # next_address_in_burst(result[0])
