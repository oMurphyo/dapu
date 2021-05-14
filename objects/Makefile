

# $^    	代表所有的依赖文件
# $@  		代表目标文件  
# $<   		代表第一个依赖文件
# = 		引用此变量时是最后一次赋值
# :=		引用变量时使用当前变量值
# ?=		引用变量时，如果变量已被初始化，则不重新赋值，否则重新赋值
# .PHONY	

#TARGET := main
OBJS := $(wildcard *.o)
CFLAGS := -g -lasound


$(TARGET):$(OBJS)
	$(CC) $^  -o $(ROOT_DIR)/$@ $(CFLAGS) $(INCLUDE_PATH)
 
