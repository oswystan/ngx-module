#!/usr/bin/env bash
###########################################################################
##                     Copyright (C) 2018 wystan
##
##       filename: test.sh
##    description:
##        created: 2018-04-28 11:34:26
##         author: wystan
##
###########################################################################
_start() {
    echo "==> start $1"
    echo "-------------------------------------------"
}

_end() {
    if [ $1 -eq 0 ]; then
        echo "---------------- SUCCESS ------------------"
    else
        echo "**************** FAILED *******************"
        exit $1
    fi
}


run() {
    _start "test restful api url" && \
        curl http://localhost:8080/api/servers
    _end $?
}

###########################################################################
run
