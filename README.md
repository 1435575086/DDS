# DDS

functest.cpp可能存在的问题
若在运行功能性测试文档时报如下错误：
/usr/bin/ld: /tmp/ccO884SE.o: in function `md5[abi:cxx11](char const*, int)':
functest2.cpp:(.text+0x4e): undefined reference to `MD5'
collect2: error: ld returned 1 exit status
这个错误是因为编译器无法找到 OpenSSL 库。需要在编译命令中链接该库，可以在命令行末尾添加 -lssl -lcrypto 参数进行链接，如下所示：
g++ functest.cpp -o functest -lssl -lcrypto

