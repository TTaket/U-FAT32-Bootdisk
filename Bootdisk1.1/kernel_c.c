// c 文件是提供的一个结构化文件 

//这里的MYPRINT() 类似一个系统调用所以需要汇编实现他
//不过c中如果想要调用这个函数 需要先有其声明 这里正是一个声明
extern void MYPRINT();

//这里的DISPLAY() 类似一个用户态函数  内部和我们正常的c函数一样
void DISPLAY(){
    MYPRINT();
    while(1){};
}
