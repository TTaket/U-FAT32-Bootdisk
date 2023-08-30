#### 1.2

接下来我们需要实现一批次的库函数

1.2.1  利用c实现库函数 提高编程效率

1. 在指定位置打印字符串
     printInPos(char * msg,uint16_t len,uint8_t row,uint8_t col);

2. 在光标位置打印字符

   extern void  putchar (char*c);
3. 读入一个字符

   extern char  getch();

基于上述的汇编实现库函数 我们可以用c扩展出以下函数

1. 获取字符串长度

   uint16_t strlen(char *s) 

2. 在光标处打印字符串

   void print(char *s)

3. 读入字符串

   void readBuf(char*buffer,uint16_tmaxlen)

1.2.2 实现简单shell 

我们已经有一批次的库函数了 可以开始着手我们的shell界面了 利用库函数 和 c 实现shell界面
