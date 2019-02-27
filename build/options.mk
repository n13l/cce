default: all
s=.
o=obj
MAKEFLAGS += -rR --include-dir=$(CURDIR)
# To put more focus on warnings, be less verbose as default
# Use 'make V=1' to see the full commands
ifeq ("$(origin V)", "command line")
  BUILD_VERBOSE = $(V)
endif
ifndef BUILD_VERBOSE
  BUILD_VERBOSE = 0
endif
ifeq ($(BUILD_VERBOSE),1)
  M=@\#
  Q =
else
  M=@echo #
  Q = @
endif


