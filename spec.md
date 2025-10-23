
# 一些设计

## 命令

每条命令之间使用";"或者换行区分
例如

在两条命令出现在同一行的时候必须使用";"区分
例如:
`loop command arg; processing data # 同一行上的两条命令`
`git push; git push github # 同一行上的两条命令`

```
# 这里两条命令之间有一个换行时, 不需要用";"
loop command arg
processing data
```

花括号可以包裹多条命令, 语法上等同于一条命令(这样的形式: `{ <命令> <命令> ... }`).
执行到花括号的时候, 将会顺序执行包裹的命令.

## 注释

一行中由 `#` 开始的部分将会被忽略
例如 `proc args # comment`
我们也可以用这样的形式写多行注释
`#( comment )`

注释的会在解析时全部删去
也就是说 `echo "he#( piggod )llo"` 在运行时等同于 `echo hello`
```
cmd1 # comment
cmd2
```
等同于
```
cmd1
cmd2
```

## 命令替换

被方括号的包裹的命令将被执行(这样的形式: `[ <命令> ]`),
其输出将被展开为命令行.
例如 `[ echo "git push" ]` 将执行 `git push`,
同时没有 echo 输出终端(因为被捕获).
又比如 `git push [ echo "origin" ]` 将执行 `git push origin`

被以 `%[ <命令> ]` 的形式包裹的命令也会被执行,
其返回值将被作为命令行参数的一部分.
例如 `echo %[ ls ]` 将执行 ls 列出文件之后,
执行 `echo 0`.

使用 `&[ <命令> ]` 的形式后台运行命令,
然后将其 pid 作为命令行参数的一部分.
我们可以写出这样的命令
```
define %get &[ http-get example.com ]
sleep 5s
when %[ kill -0 %get ] {
	echo "请求超时, 杀死进程"
	kill -9 %get
}; null
```

注意: 被中括号包裹的命令执行顺序会先于外面的命令
命令替换可以嵌套, 嵌套最深的命令将被最先执行
所有被中括号包裹的命令会优先解析并执行

## 变量

Que SHell 有一个专门的关键字用于定义变量: `define`
以 `define <变量名> <变量值 ...>` 的形式使用
例如:
`define foo      i love    you # %foo 就是 "i love    you"`
`define foo "    i love    you" # %foo 就是 "    i love    you"`
`echo %foo # 将被替换为 "echo     i love    you"`
因为空格会被处理成分隔符, 所以输出 "i love you", 如果需要处理空格可以使用:
`echo "%foo"`

所有由"%"开头的参数于解析时展开

## 条件判断 (when)

我不打算给我的 Que SHell 提供 if-else 功能, 而是简单的 when 命令.
when 命令是一条包含两条命令的复合命令.
语法: `when <条件> <命令> <命令>`

<条件> 是一个参数, 如果这个参数是0, 将执行第一个 <命令>(称为 then 命令), 否则执行第二个 <命令>(称为 otherwise 命令).
因为程序正确退出时一般使用 0, 所以我将其作为条件真值.
<条件> 可以由任意数量字母或数字组成, 不允许留空, 没有留空的状态.

在 when then 命令结束后必须跟一条 otherwise 命令

when 在执行了 <命令> 的时候返回 0, 否则返回 1.
我们可以写出这样的代码:
```
define %condition %[ ls ]; # 这个是 Que SHell 定义变量的方式

when %condition echo "成功列出文件";
echo "不成功列出文件"; 
```

当我们不需要 otherwise 命令时可以使用 null(Que SHell 内置命令, 什么都不做, 始终返回0, 不读取任何参数).
例如`when %condition echo "yes"; null`

我们可以以这样的形式编写类似 if-elif-else 的语句:
例如:
```
when %condition1 {
	echo "1"
}
when %condition2 {
	echo "2"
}
when %condition3 {
	echo "3"
}
echo "either not"
```

## 循环 (loop)

我只提供一种类似 while 的循环
语法类似这样: `loop <命令> <命令>`
loop 命令是包括两个命令的复合命令
当第一条命令返回值为0时将执行第二条命令
否则结束循环

我可以写出这样的代码:
`loop processing arg arg2; echo "processing 已处理";`

我们也可以在 loop 中使用花括号语法
这是一个每隔 5s 确认某进程存活的 qsh 脚本
```
loop kill -0 %my_proc
{
	echo "进程 %my_proc 确认存活."
	sleep 2s
}
```

## 管道

使用一般的管道 `command | process` 进行 stdout 和 stdin 的对接

## 文件重定向

Que SHell 的文件重定向类似于传统的 SHell,
例如:
`grep "i love you" < input.txt` 文本文件替换 stdin.
`cat file1.txt file2.txt > file_s.txt` stdout 直接写入文件
`server ?> errlog.txt` stderr 直接写入文件
`echo "new content" >> log.txt` 追加模式

```
< 用于 stdin
> 用于 stdout
>> 用于追加
?> 用于 stderr
```
