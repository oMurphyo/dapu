
 
# $^    	代表所有的依赖文件
# $@  		代表目标文件  
# $<   		代表第一个依赖文件
# = 		引用此变量时是最后一次赋值
# :=		引用变量时使用当前变量值
# ?=		引用变量时，如果变量已被初始化，则不重新赋值，否则重新赋值
# .PHONY	
 
 
TARGET := main
OBJDIR := objects
CC := g++
 
ROOT_DIR := $(shell pwd)
INCLUDE_PATH := -I $(shell pwd)
export TARGET CC OBJDIR ROOT_DIR INCLUDE_PATH
 
SUB_DIR := $(filter-out $(OBJDIR),$(filter $(shell ls),$(shell ls -l | grep ^d)))  
SUB_DIR := $(filter-out include,$(SUB_DIR))  
SUB_DIR := $(filter-out samples,$(SUB_DIR))  
SUB_DIR := $(filter-out test,$(SUB_DIR))  
SUB_DIR := $(filter-out output,$(SUB_DIR))  
CUR_C_SRC := $(wildcard *.c)
CUR_CPP_SRC := $(wildcard *.cpp)
OBJS := $(patsubst %.c,$(OBJDIR)/%.o,$(CUR_C_SRC)) $(patsubst %.cpp,$(OBJDIR)/%.o,$(CUR_CPP_SRC))
CFLAGS := -g 

all:$(SUB_DIR) $(OBJS) $(TARGET) $(ROOT_DIR)
 
$(SUB_DIR):print
	make -C $@
	
ifneq ($(CUR_C_SRC),$(OBJS))
$(OBJDIR)/%.o:%.c
	$(CC) -c $<  -o $@	$(CFLAGS)
$(OBJDIR)/%.o:%.cpp
	$(CC) -c $<  -o $@	$(CFLAGS)
endif
 
 
$(TARGET):
	make -C $(OBJDIR)
 
.PHONY:clean
clean:
	@rm -rf $(OBJDIR)/*.o $(TARGET)
 
.PHONY:print
print:
	@echo subdir = $(SUB_DIR)
	@echo curcfile = $(CUR_C_SRC)
	@echo objs = $(OBJS)
 
 
 
 
 
 
