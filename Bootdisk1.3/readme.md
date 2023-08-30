
#### 1.3

接下来的任务是围绕实模式和保护模式 要做到切换到保护模式

1.3.1 首先 我们应该设计更加结构化的逻辑

![内存结构](https://gitee.com/TTaket/typora-image/raw/master/内存结构.png)

1.3.2 切换到保护模式
因为进入到保护模式的时候 我们原有的BIOS就会因为寻址方式的问题发生改变 这时候需要写出新的idt表 但是工程量过于巨大 所以我们的保护模式目前只是进去一下之后再出来

GDTR：

![1693376423303](https://gitee.com/TTaket/typora-image/raw/master/1693376423303.png)

GDT表详解：

![1693375180885](https://gitee.com/TTaket/typora-image/raw/master/1693375180885.png)
