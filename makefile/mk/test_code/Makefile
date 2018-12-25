

TARGET=test
ALL_SOURCES=$(shell find -name "*.c")

SOURCE_TO_OBJECT = ./out/$(subst .c,.o,$(notdir $(1)))
ALL_OBJECTS=$(foreach src,$(ALL_SOURCES),$(call SOURCE_TO_OBJECT,$(src)))

define CREATE_OBJECT_TARGET
$(call SOURCE_TO_OBJECT,$(1)) : $(1)
	gcc -c -o $$@ $$^
endef

all: $(TARGET)

clean:
	-rm $(ALL_OBJECTS)
	-rm $(TARGET)

$(TARGET):$(ALL_OBJECTS)
	gcc -o $@ $^

$(foreach src,$(ALL_SOURCES),$(eval $(call CREATE_OBJECT_TARGET,$(src))))
