1. #### 1.4

   实现shell 命令 以及文件系统

   1. 设计文件系统的数据结构

      1. DBR ： 位于分区的第一个扇区

         ![屏幕截图 2023-09-01 110402](https://gitee.com/TTaket/typora-image/raw/master/屏幕截图 2023-09-01 110402.png)

   2. 提供文件系统的基本api
      ![image-20230908105248239](https://gitee.com/TTaket/typora-image/raw/master/image-20230908105248239.png)
      ![image-20230908105256235](https://gitee.com/TTaket/typora-image/raw/master/image-20230908105256235.png)

   3. 提供上层命令 

      利用文件系统提供的api 实现四个基本命令

   4. 完成shell 和 对应命令的绑定关系

      绑定过程![image-20230908105410894](https://gitee.com/TTaket/typora-image/raw/master/image-20230908105410894.png)

      ls：![ls](https://gitee.com/TTaket/typora-image/raw/master/ls.png)cd:
      
      ![cd](https://gitee.com/TTaket/typora-image/raw/master/cd.png)
      
      touch:![touch](https://gitee.com/TTaket/typora-image/raw/master/touch.png)
