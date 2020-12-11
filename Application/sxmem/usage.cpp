const char *usage =
"\
Usage: sxmem <IP> [OPTION]...\n\
\n\
To read/write sx chip memory and registers.\n\
\n\
  --version     version info\n\
  --help        this message\n\
  --log         log config\n\
  --address     address\n\
  --operation   operation\n\
  --bytecount   [OPTIONAL]\n\
  --data        [OPTIONAL]\n\
\n\
Examples:\n\
  sxmem --log \"app:5\"\n\
  sxmem 10.86.40.89 --operation=write --address=0x11000000 --data=\"4 : 1 12 0x22, 2 : 2 5 0xFF\"\n\
  sxmem 10.86.40.89 --operation=write --address=0x11000000 --data=\"input.bin\"\n\
  sxmem 10.86.40.89 --operation=write --address=0x11000000 --data=\"11 22 FF C0 C0 C0\"\n\
  sxmem 10.86.40.89 --operation=read --address=0x11000000 --bytecount=1024\n\
  sxmem 10.86.40.89 --operation=read --address=0x11000000 --bytecount=1024 --data=\"output.bin\"\n\
\n\
Copyright @ 2020, Jinfeng Zhuang\n\
";