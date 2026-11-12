import struct,sys
d=open(sys.argv[1],'rb').read()
off=0
while off<len(d)-16:
    if d[off+3]==0x87 and d[off]==2:
        ver,ln,tag=d[off],struct.unpack('<H',d[off+1:off+3])[0],d[off+3]
        flags,swv,fuse,n=struct.unpack('<IHBB',d[off+4:off+12])
        print(f"container @0x{off:x} flags=0x{flags:x} nimg={n}")
        for i in range(n):
            o=off+16+i*0x80
            io,isz,ld,_,ent,_=struct.unpack('<IIQ' 'Q' 'Q' 'Q',d[o:o+40]) if False else (0,0,0,0,0,0)
            io,isz=struct.unpack('<II',d[o:o+8]); ld,ent=struct.unpack('<QQ',d[o+8:o+24]); fl,=struct.unpack('<I',d[o+24:o+28])
            print(f"   img{i}: off=0x{io:x} size=0x{isz:x} load=0x{ld:x} entry=0x{ent:x} flags=0x{fl:x} type={fl&0xf}")
        off+=0x400
    else: off+=0x400
