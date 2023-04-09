# Realproxy

Realproxy是一款轻量级、高性能的代理服务器，用于http、tcp的一些Fake代理，也可用于内网http代理，Fake代理用于服务器前置，满足过滤条件的可以传递给real
server，否则传递到fake server   By: T00ls.com

``` 
__________              .__                                    
\______   \ ____ _____  |  | _____________  _______  ______.__.
 |       _// __ \\__  \ |  | \____ \_  __ \/  _ \  \/  <   |  |
 |    |   \  ___/ / __ \|  |_|  |_> >  | \(  <_> >    < \___  |
 |____|_  /\___  >____  /____/   __/|__|   \____/__/\_ \/ ____|
        \/     \/     \/     |__|                     \/\/     
```

## 版本

v 0.0.0 

## 说明

-port 端口，默认为8879

-type 选择类型，有http与tcp两种选择

-hflag http模式必选配置，选择过滤httpheader的类型

-pass
http模式为httpheader对应的过滤值，注意过滤值前面如果有空格则需要有空格匹配，大多数会有，如果不匹配则代理到fake
server。tcp模式为对应的字节，注意为十进制

``` cmd
举个例子：
-pass " Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/112.0.0.0 Safari/537.36 Edg/112.0.1722.34"
-pass "11 12 125 87"
```

-tflag
tcp模式必选配置，选择过滤第一个包的字节位置，注意从0开始，可以参考下cs的client-\>teamserver的header位置匹配，可以过滤下试试

-fip fake server服务器地址

-fport fake server服务器服务端口号

-rip real server服务器地址，未配置fip则默认传递到real server

-rport real server服务器服务端口号

## 举个栗子

``` cmd
Realproxy.exe -fport 80 -rport 8080 -port 666 -fip 192.168.159.199 -rip 192.168.159.129 -type http -hflag User-Agent: -pass " Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/112.0.0.0 Safari/537.36 Edg/112.0.1722.34"
```

## 演示

![](./image/image-20230409191117749.png "fig:")

![](./image/image-20230409191210334.png "fig:")

![](./image/image-20230409191254815.png "fig:")

![](./image/image-20230409191455157.png "fig:")

![](./image/image-20230409191532775.png "fig:")
