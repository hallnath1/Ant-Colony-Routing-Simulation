# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('ant', ['internet', 'wifi'])
    module.includes = '.'
    module.source = [
        'model/ant-id-cache.cc',
        'model/ant-dpd.cc',
        'model/ant-rtable.cc',
        'model/ant-rqueue.cc',
        'model/ant-packet.cc',
        'model/ant-neighbor.cc',
        'model/ant-routing-protocol.cc',
        'helper/ant-helper.cc',
        ]

    module_test = bld.create_ns3_module_test_library('ant')
    module_test.source = [
        'test/ant-test-suite.cc',
        ]
 
    headers = bld(features='ns3header')
    headers.module = 'ant'
    headers.source = [
        'model/ant-id-cache.h',
        'model/ant-dpd.h',
        'model/ant-rtable.h',
        'model/ant-rqueue.h',
        'model/ant-packet.h',
        'model/ant-neighbor.h',
        'model/ant-routing-protocol.h',
        'helper/ant-helper.h',
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    #bld.ns3_python_bindings()

