#!/usr/local/bin/python3

import subprocess
from pprint import pprint
import binascii


def readlines(filename, **kwargs):
    data = subprocess.run(
        args=["xxd", "-l", "160", filename],
        stdout=subprocess.PIPE).stdout.decode().split('\n')
    lines = [r for r in data if r != '']
    rows = parse_lines(lines)
    pairs = extract_pairs(rows)
    return pairs


def parse_lines(data):
    print('in parse')
    rows = [r.split(' ')[:9] for r in data]
    return rows


def extract_pairs(rows):
    print('in extract pairs')
    pairs_list = []
    for r in rows:
        for g in r[1:]:
            pairs_list.append((g[0], g[1]))
            pairs_list.append((g[2], g[3]))

    return pairs_list


# def pairs_to_words(pairs):
#     for p in pairs:

if __name__ == "__main__":
    pairs = readlines('gcc1.trace')
    # pprint(pairs)
    print('main')
    p = pairs[0]
    print(type(p[0]))
    ps = p[0] + p[1]
    pb = ps.encode()
    pi = int(ps)
    print(p)
    print(ps)
    print(pb)
    print(pi)
    # print('hexlify')
    # # print(binascii.hexlify(ps))
    # print(binascii.hexlify(pb))
    # # print(binascii.hexlify(pi))
    # print('unhexlify')
    # print(binascii.unhexlify(ps))
    # print(binascii.unhexlify(pb))
    # # print(binascii.unhexlify(pi))
    # print('b2a_hex')
    # # print(binascii.b2a_hex(ps))
    # print(binascii.b2a_hex(pb))
    # # print(binascii.b2a_hex(pi))
    # print('a2b_hex')
    # print(binascii.a2b_hex(ps))
    # print(binascii.a2b_hex(pb))
    # # print(binascii.a2b_hex(pi))
    # print('a2b_uu')
    # print(binascii.a2b_uu(ps))
    # print(binascii.a2b_uu(pb))
    # # print(binascii.a2b_uu(pi))
    # print('b2a_uu')
    # # print(binascii.b2a_uu(ps))
    # print(binascii.b2a_uu(pb))
    # # print(binascii.b2a_uu(pi))
    # print('bin(int())')
    # print(bin(int(ps)))
    # print(bin(int(pb)))
    # print(bin(int(pi)))
    print('bin(int())[2:]')
    print(bin(int(ps))[2:])
    print(bin(int(pb))[2:])
    print(bin(int(pi))[2:])
    print('bin(int(, 16))[2:]')
    print(bin(int(ps, 16))[2:])
    print(bin(int(pb, 16))[2:])
    # print(bin(int(pi, 16))[2:])
    print('bin(int(p[i], 16))[2:]')
    print(bin(int(p[0], 16))[2:])
    print(bin(int(p[1], 16))[2:])
    # print(bin(int(p[0]))[2:])
    # print(bin(int(p[0]))[2:])
