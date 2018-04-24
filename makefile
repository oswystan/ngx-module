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

#####################################
## functions
#####################################
define get-ngx
	@ [ -d nginx ] || \
     git clone https://github.com/nginx/nginx.git && \
	 cd nginx && \
	 git checkout -b dev release-1.14.0
endef

define build-ngx-conf
	@ cd nginx && \
	  auto/configure --prefix=/usr/local/nginx --with-http_ssl_module --with-pcre --add-module=$(top_dir)/module
endef

#####################################
## targets and rules
#####################################
all: $(ngx_conf)
	make -C nginx -j4

$(ngx_conf): nginx
	$(call build-ngx-conf)

nginx:
	$(call get-ngx)

clean:
	@ rm -rf nginx/objs/addon/module/*

distclean:
	@ rm -rf nginx

vars:
	@printf "\n"
	@printf "%10s = %s\n" "top_dir" $(top_dir)
	@printf "%10s = %s\n" "ngx_dir" $(ngx_dir)
	@printf "%10s = %s\n" "ngx_conf" $(ngx_conf)
	@printf "\n"
#######################################################################
