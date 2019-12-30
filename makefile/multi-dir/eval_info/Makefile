
###############################################
pointer := pointed_value

define foo 
var := 123
arg := $1
$$($1) := ooooo
endef 

$(info $(call foo,pointer))
#res := $(call foo,pointer)
#$(eval $(call foo,pointer))

target:
	@echo -----------------------------
	@echo var: $(var)
	@echo arg: $(arg)
	@echo res: $(res)
	@echo pointer: $(pointer), pointed_value: $(pointed_value)
	@echo done.
	@echo -----------------------------

###############################################
