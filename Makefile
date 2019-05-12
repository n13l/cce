include build/options.mk
include build/rules.mk

LDFLAGS=-lstdc++ -lurcu -lpthread -ldl -fno-exceptions
CFLAGS=-I$(s) -std=c11
CXXFLAGS=-I$(s) -std=c++14

CC  := $(or ${X},${X},gcc)
CXX := $(or ${X},${X},g++)
AR  := $(or ${X},${X},ar)

srcs := tools/tester.c
objs := $(patsubst %.c,$(o)/%.o,$(sort $(srcs)))

$(o)/tools/tester: $(o)/tools/tester.o

all: $(o)/tools/tester

test: 
	$(Q)$(s)/tests/run-tests.sh --tap

clean:
	$(Q)rm -rf obj

.PHONY: all test clean
