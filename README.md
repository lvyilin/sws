 sws
============
**sws** 是一个简单的web服务器。支持`GET`, `POST`, `HEAD`, `PUT`, `DELETE`请求。

更多信息参见sws.1.pdf。

## 项目规划
### 开发环境和工具
+ 开发环境：Ubuntu
+ 开发语言：C 99
+ 开发工具：CLion
+ 构建工具：CMake 3.10+
### 开发规范
+ 代码风格：[Google C++风格指南](http://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/)
## 功能说明
### GET
#### 文件 & 文件夹
若URL地址为文件路径，返回文件内容。若为目录，返回执行`ls -lhF <path>`的输出内容。
#### CGI
当URL地址格式为`/cgi-bin/foo`，执行`-c`指定目录下的`foo`程序，并返回输出结果。
### POST
返回一个HTML文件，包括请求URL地址与POST请求体中的内容。
### HEAD
同GET方法，但只返回请求头。
### PUT
将POST请求体中的data字段键值写入URL指定的文件。
### DELETE
将URL指定的文件删除。
## 设计说明
### 编译说明
```shell
mkdir build
cd build
cmake ..
make
```
### 运行说明
```shell
sws [ −dh] [ −c dir] [ −i address] [ −l file] [ −p port] dir
```
### 模块说明

| 模块   | 用途               |
| -------- | ------------------ |
| main     | 主函数、命令行解析 |
| utility  | 常量、通用函数定义 |
| network  | 网络函数           |
| parser   | 解析请求           |
| response | 构造响应           |
| logger   | 记录日志           |
| fileio   | 文件IO             |

### 函数说明
+ main
```c
argp_parse(&argp, argc, argv, 0, 0, &arguments);
解析命令行参数，库函数
```
+ utility
```c
int starts_with(const char *pre, const char *str, int str_len);
工具函数，判断字符串是否为指定开头
```
+ network
```c
in_addr_t get_binary_addr(const char *src);
获得二进制ip地址

void start_listener(int port, char *bind_addr, FILE *logger, char *index_path, char *cgi_path, int debug);
开始监听
```
+ parser
```c
void request_parse(char *request, int len, struct RequestInfo *info);
解析请求，将请求头与请求体存放于RequestInfo

void parameter_parse(char *body, char *key, char *value);
解析请求体键值对
```
+ response
```c
void exec_command(const char *command, char *output);
执行命令，输出结果返回到output，用于实现CGI与输出目录
int is_regular_file(const char *path);
int is_valid_dir(const char *path);
判断是否为有效文件 & 文件夹
void get_response(struct RequestInfo request,struct ResponseInfo *response_info, char *response, char *index_path, char *cgi_path);
从RequestInfo获得请求信息，返回响应字符串与ResponseInfo(用于日志)
void get_date_info(struct Response *header);
获得服务器日期时间
void get_url_path(char *url, char *index_path, char *dest);
获得GET请求中的绝对地址
int get_file_info(char *filepath, struct Response *header);
获得文件信息
void build_response(struct Response *response, char *dest);
将Response结构体转化为char字符串
void get_status_msg(enum HttpStatus status, char *dest);
将HTTP状态码转换为字符串用户拼接输出结果
```
+ logger
```c
FILE *make_logger(char *filepath);
int close_logger(FILE *logger);
打开 & 关闭日志文件
int log_to(struct RequestInfo *req,struct ResponseInfo *resp, FILE *logger) ;
记录信息
```
+ fileio
```c
void read_file(char *filepath, char *dest, int max_len);
int write_file(char *filepath, char *src);
int delete_file(char *filepath);
读 & 写 & 删文件
int is_absolute_path(char *filepath);
void get_absolute_path(char *relative_path, char *absolute_path);
判断 & 拼接获得绝对路径
```
### 程序流程
![Flowchart](flowchart.png)
## 运行截图
查看Screenshot文件夹。
## License
2018.8前版权归作者所有，之后使用MIT协议。
