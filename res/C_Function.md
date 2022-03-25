##### 1.strpbrk

```cpp
//C库函数  头文件<string.h>
1.
	char *strpbrk(char *str1, char *str2)
  /*
  strpbrk( szURL, " \t" )
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

*/
```



##### 8.iovec



##### 9.writev