# my-jvmti-demo
用JVMTI接口获取方法进入的事件，打印类的加载信息

## 编译方法
```shell script
cmake
make
```

## 使用方法
编译出来的动态链接库，通过-agentpath:/xxxx的方式加载到Java应用中