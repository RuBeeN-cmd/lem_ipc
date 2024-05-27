#!/bin/bash

# source $PWD/rsc/sh/color.sh

all_check() {
    ipcs | grep -e "0x" | grep "--r.-r--r--" | cut -d ' ' -f 2 > /dev/null
    if [ $? -ne 0 ]; then
        printf ${RED} "Leak found after clean.\n"
        free_ipcs ${IPCS_OPT} ${MSG}
    else
        printf ${GREEN} "No leak after clear.\n"
    fi

}

free_ipcs() {
    IPCS_OPT="$1"
    MSG="$2"
    local ipcs_id=$(ipcs ${IPCS_OPT} | grep -v dest | grep "\-\-r.\-r\-\-r\-\-" | awk '{print $2}' | tr '\n' ' ')
    if [ "${ipcs_id}" != "" ]; then
        ID=$(ipcs ${IPCS_OPT} | grep -v dest | grep "\-\-r.\-r\-\-r\-\-" | awk '{print $2}' | tr '\n' ' ' | awk '{print $1}')
        printf ${YELLOW} "Clear ${ipcs_id} ${MSG} leak found.\n"
        ipcrm ${IPCS_OPT} ${ID}
        free_ipcs ${IPCS_OPT} ${MSG}
    else
        printf ${GREEN} "Nothing to clean no ${MSG} leak found.\n"
    fi
}

sleep 2
free_ipcs -m "Shared memory"
free_ipcs -s "Semaphore"
free_ipcs -q "Message queue"
# all_check