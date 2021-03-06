
# Environment
EXEC_DIR   = bin
OBJECT_DIR = build

# Compilation Flags
COMPILE_FLAGS := -ggdb3 -pipe -pedantic -Wall -Wextra -Werror -O2\
                 -Iinclude

# Callable variables (i.e. functions)
find_in_dir_with_suffix = $(foreach dir,$(1),$(shell find $(dir) -name "*.$(2)"))
dependencies = -MMD -MF $(1:.o=.d) -MT $(1)
compile  = $(CC) $(COMPILE_FLAGS) $(call dependencies,$(OBJECT_DIR)/$(2)) \
           -c $(1) -o $(OBJECT_DIR)/$(2)
echo_and_execute = echo $(1); $(1)

# Main Source Files
MAIN_INCLUDE_ROOT_DIR := include
MAIN_SOURCE_ROOT_DIR := src
MAIN_HEADER_FILES := $(call find_in_dir_with_suffix,$(MAIN_INCLUDE_ROOT_DIR),h)
MAIN_IMPL_FILES   := $(call find_in_dir_with_suffix,$(MAIN_SOURCE_ROOT_DIR),c)

# Main Build Targets
MAIN_OBJECTS = $(addprefix $(OBJECT_DIR)/main/,$(subst src/,,$(MAIN_IMPL_FILES:.c=.o)))
MAIN_OBJECT_DIRS = $(sort $(dir $(MAIN_OBJECTS)))
MAIN_TARGET_LIBRARY := $(EXEC_DIR)/libattest.a

# Test
TEST_IMPL_FILES = $(call find_in_dir_with_suffix,test/,c)
TEST_OBJECTS = $(addprefix $(OBJECT_DIR)/test/,$(subst test/,,$(TEST_IMPL_FILES:.c=.o)))
TEST_EXE = test/bin/test-all

all: $(TEST_EXE) $(MAIN_TARGET_LIBRARY) main_init 

clean:
	rm -rf $(EXEC_DIR)/* $(OBJECT_DIR)/*

main_init:
	@mkdir -p $(sort $(dir $(MAIN_OBJECTS))) $(dir MAIN_TARGET_LIBRARY) $(sort $(dir $(TEST_OBJECTS)))

$(MAIN_TARGET_LIBRARY): $(MAIN_OBJECTS)
	@mkdir -p $(dir $(MAIN_TARGET_LIBRARY))
	ar rcs $(MAIN_TARGET_LIBRARY) $(MAIN_OBJECTS)

$(OBJECT_DIR)/main/%.o: Makefile | main_init
	@$(call echo_and_execute,$(call compile,src/$*.c,main/$*.o))

$(OBJECT_DIR)/test/%.o: Makefile | main_init
	@$(call echo_and_execute,$(call compile,test/$*.c,test/$*.o))

$(TEST_EXE): $(TEST_OBJECTS) $(MAIN_TARGET_LIBRARY)
	@mkdir -p $(dir $(TEST_EXE))
	$(CC) $(COMPILE_FLAGS) $(MAIN_TARGET_LIBRARY) $(TEST_OBJECTS) -o $(TEST_EXE)

%.d:

.PHONY: all clean main_init

define depends_include_template
-include $(1)

endef

$(eval $(foreach dep,$(wildcard $(MAIN_OBJECTS:.o=.d)),$(call depends_include_template,$(dep))))

