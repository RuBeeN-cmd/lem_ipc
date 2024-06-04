#!/bin/bash

free_ipcs() {
    IPCS_OPT="$1"
    MSG="$2"

	USER=$(whoami)
	if [ "$(uname)" == "Linux" ]; then
		PERM="644"
	elif [ "$(uname)" == "Darwin" ]; then
		if [ $IPCS_OPT == "-s" ]; then
			PERM="ra\-r\-\-r\-\-"
		else
			PERM="rw\-r\-\-r\-\-"
		fi
	fi
    local ipcs_id=$(ipcs ${IPCS_OPT} | grep -v dest | grep $USER | grep $PERM | awk '{print $2}' | tr '\n' ' ')
    if [ "${ipcs_id}" != "" ]; then
        ID=$(ipcs ${IPCS_OPT} | grep -v dest | grep $USER | grep $PERM | awk '{print $2}' | tr '\n' ' ' | awk '{print $1}')
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