LIBDIR = ../../lib
APPDIR = ../../bin
# 获取当前目录名作为模块名
MODULE := $(notdir $(CURDIR))
# 库配置 自动生成库名
TARGET = $(MODULE)

MAIN_SRC = main.c
OTHER_SRCS = $(filter-out main.c, $(wildcard *.c))
SRCS = $(MAIN_SRC) $(OTHER_SRCS)

OBJS = $(SRCS:.c=.o)

LIBS = $(wildcard $(LIBDIR)/*.a)
LDFLAGS = $(patsubst $(LIBDIR)/lib%.a, -l%, $(LIBS))
LDFLAGS += -pthread
INCLUDE = -I../include

CC = gcc
CFLAGS = -g $(INCLUDE)

.PHONY: all clean


all: $(APPDIR) $(APPDIR)/$(TARGET)

$(APPDIR)/$(TARGET): $(OBJS) | $(APPDIR)
	@echo "========== [${MODULE}] 开始编译 ${MODULE}"
	@$(CC) $(CFLAGS) $(OBJS) -L$(LIBDIR) $(LDFLAGS) -o $@
	@echo "✅ 成功生成 $(TARGET)，复制到 $(APPDIR)"
	@echo "========== [${MODULE}] 完成编译 ${TRGLIB}"

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(APPDIR):
	@mkdir -p $(APPDIR)

clean:
	@echo "========== [${MODULE}] 开始清理 ${TRGLIB}"
	@echo "🧹正在清理模块 [${MODULE}] ..."
	@rm -f ${OBJS} ${TARGET}
	@echo "🧹 清理 $(APPDIR) 下可执行文件 $(TARGET)"
	@rm -f ${APPDIR}/${TARGET}
	@echo "✅ 模块清理完成 [${MODULE}] "
	@echo "========== [${MODULE}] 完成清理 ${TRGLIB}"