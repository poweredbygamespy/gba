#!/bin/python3

filename = "../unscii-8-alt.hex"
out = "tmp.txt"

f = open(filename, "r");
hex = f.read()
f.close()

dict = {}
for l in hex.split("\n"):
    if len(l.split(":")) >= 2:
        dict[l.split(":")[0]] = l.split(":")[1]

res = ""
for v in dict.values():
    for byte in [v[i:i+2] for i in range(0, len(v), 2)]:
        res += "\t.word 0x" + "{t:08b}".format(t = int("0x" + byte, 16))[::-1] + "\n"

f = open(out, "w")
f.write(res)
f.close()
