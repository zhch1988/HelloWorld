# HelloWorld
1.dependency_module是依赖模块，需要先加载才能加载module_test模块(已在module_test中的ins.sh和rm.sh脚本中按顺序做了加载和卸载操作)<br>
2.application_test中是对应用于测试的用户程序<br>
3.device_test是用于测试module_test中platform_driver的内核模块(platform_device)<br>
4.include文件夹是公用的头文件夹<br>
