SHELL := /bin/bash

RM := rm -rf
MKDIR := mkdir -p
LESS := less
DIFF := diff --text --color
VDIFF := vimdiff
LLDB := lldb
GDB := gdb
VALGRIND := valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
VALGRIND_LOG := valgrind.log

CXX := clang++
CXXFLAGS := -O0 -gdwarf-4 -std=c++98 -stdlib=libc++ 
ifdef SANITIZE
CXXFLAGS += -fno-omit-frame-pointer -fno-optimize-sibling-calls -fsanitize=address
endif

WARNING := -Wall -Wextra
ifndef NOERROR
WARNING += -Werror
endif

FT := ft_container
STD := std_container
ifdef SANITIZE
FT := $(FT)_asan
STD := $(STD)_asan
endif

OUT_DIR := out
ifdef SANITIZE
OUT_DIR := $(OUT_DIR)/sanitize
else
OUT_DIR := $(OUT_DIR)/release
endif

FT_OUT_DIR := $(OUT_DIR)/ft
STD_OUT_DIR := $(OUT_DIR)/std

SRCS := main.cpp  \
ft/tree.cpp \
tests/vector.tests.cpp \
tests/stack.tests.cpp \
tests/set.tests.cpp \
tests/map.tests.cpp \
tests/utils/chrono.cpp \
tests/utils/logger.cpp \
tests/utils/testing_struct.cpp

OBJS := $(SRCS:.cpp=.o)
FT_OBJS := $(addprefix $(FT_OUT_DIR)/, $(OBJS))
STD_OBJS := $(addprefix $(STD_OUT_DIR)/, $(OBJS))

all: $(FT) $(STD)

-include $(FT_OBJS:.o=.d)
-include $(STD_OBJS:.o=.d)

$(FT_OUT_DIR)/%.o: %.cpp Makefile
	$(MKDIR) $(@D)
	$(COMPILE.cpp) $< $(WARNING) -MMD -MP -o $@
$(STD_OUT_DIR)/%.o: %.cpp Makefile
	$(MKDIR) $(@D)
	$(COMPILE.cpp) $< $(WARNING) -MMD -MP -o $@ -DSTD

$(FT): $(FT_OBJS)
	$(CXX) $(CXXFLAGS) $(FT_OBJS) -o $@
$(STD): $(STD_OBJS)
	$(CXX) $(CXXFLAGS) $(STD_OBJS) -o $@

asan-%: export SANITIZE = true
asan-%: export NOERROR = true
asan-%:
	$(MAKE) --no-print-directory $*

noerr-%: export NOERROR = true
noerr-%:
	$(MAKE) --no-print-directory $*

diff: $(FT) $(STD)
	-@$(DIFF) <(./$(FT) $(ARGS)) <(./$(STD) $(ARGS)) 
vdiff: $(FT) $(STD)
	-@$(VDIFF) <(./$(FT) $(ARGS)) <(./$(STD) $(ARGS)) 

lldb: $(FT)
	-@$(LLDB) ./$(FT)
gdb: $(FT)
	-@$(GDB) ./$(FT)

valgrind: $(FT)
	-@$(VALGRIND) --log-file=$(VALGRIND_LOG) ./$(FT) $(ARGS)
	@$(LESS) $(VALGRIND_LOG)

clean: 
	$(RM) $(OUT_DIR)
fclean: clean
	$(RM) $(FT) $(STD)
re: fclean all

compile_commands.json:
	compiledb make re

pvs: compile_commands.json
	$(RM) pvs_report
	pvs-studio-analyzer analyze -f ./compile_commands.json
	plog-converter -t fullhtml PVS-Studio.log -o ./pvs_report

cc: compile_commands.json
	codechecker analyze --ctu ./compile_commands.json --output ./cc_report
	codechecker parse ./cc_report

.PHONY: all clean fclean re valgrind lldb gdb diff vdiff pvs cc