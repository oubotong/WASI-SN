with open('test.wasm', 'r') as fp:
    hex_list = ["0x{:02x}".format(ord(c)) for c in fp.read()]
    print hex_list
