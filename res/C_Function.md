##### 1.strpbrk

```cpp
//C库函数  头文件<string.h>
1.
	char *strpbrk(char *str1, char *str2)
  /*
  char *text = "POST / HTTP1.1";
  char *url = strpbrk(text, " \t"); //url = " / HTTP1.1"
	参数说明：str1待比较的字符串，str2为指定被搜索的字符串。
	函数功能：比较字符串str1和str2中是否有相同的字符，如果有，则返回该字符在str1中的位置的指针。
	*/

```

##### 2.strcasecmp

```cpp
2.
  int strcasecmp (const char *s1, const char *s2);
	/*
	strcasecmp("aa", "AA")
	函数说明：strcasecmp()用来比较参数s1 和s2 字符串，比较时会自动忽略大小写的差异。
	*/
```

##### 3.strspn

```cpp
size_t strspn(const char *str1, const char *str2)
  /*
  检索字符串 str1 中第一个不在字符串 str2 中出现的字符下标。
  const char str1[] = "ABCDEFG019874";
  const char str2[] = "AB0CD";
  len = strspn(str1, str2);//4 -->E
  */
```

##### 4.strchr

```cpp
char *strchr(const char *str, int c)
  /*
  在参数 str 所指向的字符串中搜索 第一次 出现字符 c（一个无符号字符）的位置
  返回值：该函数返回在字符串 str 中第一次出现字符 c 的位置，如果未找到该字符则返回 NULL。
  const char str[] = "http://www.runoob.com";
  const char ch = '.';
  char *ret;
  ret = strchr(str, ch);// ret:.runoob.com

	char *strrchr(const char *s, int c);
	在参数 str 所指向的字符串中搜索 最后一次 出现字符 c（一个无符号字符）的位置
  上述例子中 ret-->.com
  */
```

5.strcat

```cpp
char *strcat(char *dest, const char *src)
  /*
  把 src 所指向的字符串追加到 dest 所指向的字符串的结尾。
  该函数返回一个指向最终的目标字符串 dest 的指针。
  */
```

##### 6.stat

```cpp
//获取文件属性，存储在statbuf中
    int stat(const char *pathname, struct stat *statbuf);
		struct stat 
		{
      //部分成员
	  	 mode_t    st_mode;        /* 文件类型和权限 */
	   	 off_t     st_size;        /* 文件大小，字节数*/
		};
```

##### 7.mmap

```cpp
void* mmap(void* start,size_t length,int prot,int flags,int fd,off_t offset);
int munmap(void* start,size_t length);
/*
用于将一个文件或其他对象映射到内存，提高文件的访问速度。
start：映射区的开始地址，设置为0时表示由系统决定映射区的起始地址

length：映射区的长度

prot：期望的内存保护标志，不能与文件的打开模式冲突

PROT_READ 表示页内容可以被读取

flags：指定映射对象的类型，映射选项和映射页是否可以共享

MAP_PRIVATE 建立一个写入时拷贝的私有映射，内存区域的写入不会影响到原文件

fd：有效的文件描述符，一般是由open()函数返回

mmap用于申请一段内存空间，munmap则是释放由mmap申请的内存空间。
*/
```

##### 8.iovec

```cpp
struct iovec {
2    void      *iov_base;      /* starting address of buffer */
3    size_t    iov_len;        /* size of buffer */
4};
/*
定义了一个向量元素，通常，这个结构用作一个多元素的数组。
iov_base指向数据的地址

iov_len表示数据的长度
*/
```

##### 9.writev

```cpp
ssize_t writev(int filedes, const struct iovec *iov, int iovcnt);
/*
filedes表示文件描述符

iov为前述io向量机制结构体iovec

iovcnt为结构体的个数
若成功则返回已写的字节数，若出错则返回-1。writev以顺序iov[0]，iov[1]至iov[iovcnt-1]从缓冲区中聚集输出数据。writev返回输出的字节总数，通常，它应等于所有缓冲区长度之和。
*/
```

##### 10.printf系列

```cpp
int printf(const char *format, ...); //输出到标准输出
int fprintf(FILE *stream, const char *format, ...); //输出到文件
int sprintf(char *str, const char *format, ...); //输出到字符串str中
int snprintf(char *str, size_t size, const char *format, ...); //按size大小输出到字符串str中
//-------------------
//以下函数功能与上面的一一对应相同，只是在函数调用时，把上面的...对应的一个个变量用va_list调用所替代。在函数调用前ap要通过va_start()宏来动态获取。
int vprintf(const char *format, va_list ap);
int vfprintf(FILE *stream, const char *format, va_list ap);
int vsprintf(char *str, const char *format, va_list ap);
int vsnprintf(char *str, size_t size, const char *format, va_li
/*
可变参数列表va_list宏说明:
void va_start(va_list ap, last);
void va_end(va_list ap);
1.va_start与va_end是成对被调用的
2.开始的时候被调用va_start，获得各输出变量地址
3.结束的时候被调用va_end，释放相应的资源
*/

```

