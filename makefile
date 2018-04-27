#######################################################################
##                     Copyright (C) 2018 wystan
##
##       filename: makefile
##    description:
##        created: 2018-04-24 20:34:48
##         author: wystan
##
#######################################################################
.PHONY: all build test install doc

top_dir  := $(PWD)
ngx_dir  := $(top_dir)/nginx
ngx_conf := nginx/objs/ngx_auto_headers.h
ngx_bin  := $(ngx_dir)/objs/nginx
module_src := $(shell find module -name *.c -o -name *.h)

rest_lib := $(top_dir)/module/libngx_rest.a
rest_src := ngx_rest/http/router.cc 	\
			ngx_rest/http/log.cc 		\
			ngx_rest/http/app.cc 		\
			ngx_rest/http/request.cc 	\
			ngx_rest/http/response.cc 	\

rest_obj := $(rest_src:.cc=.o)

CPP_FLAGS := -I nginx/src/core -I nginx/objs -I nginx/src/os/unix -I nginx/src/event -I nginx/src/http -I nginx/src/http/modules

#####################################
## functions
#####################################
define get-ngx
	@ [ -d nginx ] || \
     git clone https://github.com/nginx/nginx.git && \
	 cd nginx && \
	 git checkout -b dev release-1.10.2
endef

define build-ngx-conf
	@ cd nginx && \
	  auto/configure --prefix=$(top_dir)/install --with-http_ssl_module --with-pcre \
	  --add-module=$(top_dir)/module
endef

#####################################
## targets and rules
#####################################
all: $(ngx_bin)

install:
	-killall nginx
	rm -rf install
	make -C nginx install
	cp module/nginx.conf install/conf/
	cp module/nginx.conf install/conf/nginx.conf.default

$(ngx_bin): $(ngx_conf) $(module_src) $(rest_lib)
	make -C nginx -j4

$(ngx_conf): nginx
	$(call build-ngx-conf)

nginx:
	$(call get-ngx)

cleangx:
	make -C nginx clean

clean:
	@ rm -rf $(ngx_dir)/objs/addon/module/*
	@ rm -rf $(ngx_bin) install 
	@ rm -rf $(rest_obj) $(rest_lib)

distclean:
	@ rm -rf nginx

$(rest_lib):$(rest_obj)
	@ ar -r -o $@ $^

%.o:%.cc
	g++ $(CPP_FLAGS) -std=c++11 -Wall -Werror -c $^ -o $@

vars:
	@printf "\n"
	@printf "%10s = %s\n" "top_dir" $(top_dir)
	@printf "%10s = %s\n" "ngx_dir" $(ngx_dir)
	@printf "%10s = %s\n" "ngx_conf" $(ngx_conf)
	@printf "\n"
#######################################################################