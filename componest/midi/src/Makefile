 
# $^    	代表所有的依赖文件
# $@  		代表目标文件  
# $<   		代表第一个依赖文件
# = 		引用此变量时是最后一次赋值
# :=		引用变量时使用当前变量值
# ?=		引用变量时，如果变量已被初始化，则不重新赋值，否则重新赋值
# .PHONY	
 
SUB_DIR := $(filter $(shell ls),$(shell ls -l | grep ^d))
CUR_C_SRC := $(wildcard *.c)
CUR_CPP_SRC := $(wildcard *.cpp)
OBJS := $(patsubst %.c,%.o,$(CUR_C_SRC)) $(patsubst %.cpp,%.o,$(CUR_CPP_SRC))
CC := g++
CFLAGS += -g
CFLAGS += $(INCLUDE_PATH)

all:$(SUB_DIR) $(OBJS)
 
$(SUB_DIR):print
	make -C $@
 
ifneq ($(CUR_C_SRC),$(OBJS))
%.o:%.c
	$(CC) -c $<  -o $(ROOT_DIR)/$(OBJDIR)/$@ $(CFLAGS)
%.o:%.cpp
	$(CC) -c $<  -o $(ROOT_DIR)/$(OBJDIR)/$@ $(CFLAGS)
endif

.PHONY:clean
clean:
	@rm -rf $(OBJS) $(TARGET)
 
.PHONY:print
print:
	@echo subdir = $(SUB_DIR)
	@echo curcfile = $(CUR_C_SRC)
 
 
