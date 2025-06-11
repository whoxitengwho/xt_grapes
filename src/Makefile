.PHONY: all clean libs apps


LIBDIRS := ./shmem ./conf
APPDIRS := ./grape ./cluster
SUBDIRS := $(LIBDIRS) $(APPDIRS)

LIBPATH = ../lib
APPPATH = ../bin

MAKEFLAGS += --no-print-directory


all:
	@echo "📦 开始构建全部模块..."
	@$(MAKE) $(LIBPATH)
	@$(MAKE) $(APPPATH)
	@$(MAKE) libs
	@$(MAKE) apps
	@echo "✅ 所有模块构建完成"



apps: shared
	@echo "🛠️ 正在构建app所有模块..."
	@echo
	@for dir in $(APPDIRS); do \
		$(MAKE) -C $$dir || exit 1; \
	done
	@echo
	@echo "🎉 所有app模块构建完成"

libs:
	@echo "🛠️ 正在构建lib所有模块..."
	@echo
	@for dir in $(LIBDIRS); do \
		$(MAKE) -C $$dir || exit 1; \
	done
	@echo
	@echo "🎉 所有lib模块构建完成"

# 自动创建目录规则
$(LIBPATH):
	@mkdir -p $(LIBPATH)

$(APPPATH):
	@mkdir -p $(APPPATH)

shared:
	@rm -f ${APPPATH}/shared

clean:
	@echo "🧹 清理所有模块..."
	@echo
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean || exit 1; \
	done
	@rm -f ${LIBPATH}/*
	@rm -f ${APPPATH}/*
	@echo
	@echo "✅ 全部清理完成"