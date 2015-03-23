all: compile

CXX       := g++
CXX_FLAGS := -pipe -std=c++11 -Wall -fPIC -Iinclude -Ipch -Ofast -MMD -flto -fvisibility=hidden
LD_FLAGS  := -lyaml-cpp -lldap -shared
CPP_FILES := $(wildcard src/*.cpp)
HDR_FILES := $(wildcard include/*.h)
OBJ_FILES := $(addprefix .build/,$(notdir $(CPP_FILES:.cpp=.o)))
DEPENDS   := ${OBJ_FILES:.o=.d}

compile: bin/openvpnldapgroups.so

rebuild: clean
	$(MAKE) compile

bin/openvpnldapgroups.so: $(OBJ_FILES)
	$(CXX) $(CXX_FLAGS) $(LD_FLAGS) -o $@ $^

.build/%.o: src/%.cpp pch/pch.h.gch
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

pch/pch.h.gch: pch/pch.h
	$(CXX) $(CXX_FLAGS) -c -o $@ $<

clean:
	rm -rf bin/*
	rm -rf .build/*
	rm -rf pch/pch.h.gch

.PHONY: clean rebuild

-include $(DEPENDS)
