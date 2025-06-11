# 获取当前目录名作为模块名
MODULE := $(notdir $(CURDIR))
# 库配置 自动生成库名
TRGLIB := lib$(MODULE).a

# 源码和对象
SRC_DIR := .
OBJ_DIR := .
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
LIBPATH := ../../lib
INCLUDE_DIR = ../include
INCLUDE = -I$(INCLUDE_DIR)

# 编译设置
CC := gcc
CFLAGS := -g $(INCLUDE)

.PHONY: all clean

all:
	@echo "========== [${MODULE}] 开始编译 ${TRGLIB}"
	@$(MAKE) ${TRGLIB} --no-print-directory
	@echo "✅ 成功编译生成 ${TRGLIB}"
	@cp -f ${TRGLIB} ${LIBPATH}/${TRGLIB}
	@echo "📁 已复制 ${TRGLIB} 到目录：${LIBPATH}"
	@echo "========== [${MODULE}] 完成编译 ${TRGLIB}"

# 构建库文件
${TRGLIB}: ${OBJS}
	@rm -f $@
	@ar -rc $@ $^

# 通用编译规则
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "========== [${MODULE}] 开始清理 ${TRGLIB}"
	@echo "🧹正在清理模块 [${MODULE}] ..."
	@rm -f ${OBJS} ${TRGLIB} ${LIBPATH}/${TRGLIB}
	@echo "✅ 模块清理完成 [${MODULE}] "
	@echo "========== [${MODULE}] 完成清理 ${TRGLIB}"
