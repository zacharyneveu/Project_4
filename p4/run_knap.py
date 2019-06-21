#! /usr/bin/env python
# -*- coding: utf-8 -*-
# vim:fenc=utf-8
#
# Copyright © 2019 zach <zach@zach-blade>
#
# Distributed under terms of the MIT license.

"""
Runs graph coloring on all input files
"""

import sys, os
import pexpect

infiles = [x for x in os.listdir('instances') if (x.startswith('knapsack') and x.endswith('.input'))]
infiles = sorted(infiles, key=lambda x: int(x[8:-6]))
infiles = [x for x in infiles if int(x[8:-6]) in range(int(sys.argv[1]), int(sys.argv[2]))]

for f in infiles:
    print(f)
    p = pexpect.spawnu('./p4.out')
    p.expect('Enter filename')
    p.sendline('instances/'+f)

    p.expect('Output file created.', timeout=610)
    with open('outputs/'+f[:-6]+'.output', 'w+') as of:
        of.write(p.before)
