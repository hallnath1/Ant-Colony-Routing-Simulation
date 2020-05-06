# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('ara', ['internet', 'wifi'])
    module.includes = '.'
    module.source = [
        'model/ara-id-cache.cc',
        'model/ara-dpd.cc',
        'model/ara-rtable.cc',
        'model/ara-rqueue.cc',
        'model/ara-packet.cc',
        'model/ara-neighbor.cc',
        'model/ara-routing-protocol.cc',
        'helper/ara-helper.cc',
        ]

    module_test = bld.create_ns3_module_test_library('ara')
    module_test.source = [
        'test/ara-test-suite.cc',
        ]
 
    headers = bld(features='ns3header')
    headers.module = 'ara'
    headers.source = [
        'model/ara-id-cache.h',
        'model/ara-dpd.h',
        'model/ara-rtable.h',
        'model/ara-rqueue.h',
        'model/ara-packet.h',
        'model/ara-neighbor.h',
        'model/ara-routing-protocol.h',
        'helper/ara-helper.h',
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    #bld.ns3_python_bindings()

