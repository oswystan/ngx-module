#!/usr/bin/env bash
###########################################################################
##                     Copyright (C) 2018 wystan
##
##       filename: env_setup.sh
##    description:
##        created: 2018-04-28 11:21:27
##         author: wystan
##
###########################################################################

dir_root=$(pwd)
dir_sbin="${dir_root}/install/sbin"
dir_logs="${dir_root}/install/logs"
log_file="${dir_logs}/restful.log"
pid_file="${dir_logs}/nginx.pid"
ngx_file="${dir_sbin}/nginx"

_help() {
cat <<!EOF

usage: app <command>
    - start
    - stop
    - restart
    - status
    - log
    - help

!EOF
}

app() {
    case $1 in
        "start")
            ${ngx_file}
            ;;
        "stop")
            ${ngx_file} -s quit
            ;;
        "restart")
            ${ngx_file} -s quit
            rm -f ${log_file}
            ${ngx_file}
            ;;
        "status")
            pgrep -lf nginx
            ;;
        "log")
            less -N ${log_file}
            ;;
        "")
            cd ${dir_root}
            ;;
        *)
            _help
            ;;
    esac
}

complete -W "start stop restart status log help" app

###########################################################################
