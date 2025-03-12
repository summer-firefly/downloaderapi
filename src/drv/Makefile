MODULE_NAME = softathome_downloaderapi
obj-$(CONFIG_SOFTATHOME_DOWNLOADERAPI) += $(MODULE_NAME).o
$(MODULE_NAME)-y += downloaderapi_kernel.o
PR_FMT = $(MODULE_NAME)
PR_FMT_DEFINE="-Dpr_fmt(fmt)= \"[$(PR_FMT)]: \" fmt"
ccflags-y += $(PR_FMT_DEFINE)