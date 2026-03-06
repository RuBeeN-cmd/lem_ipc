#!/bin/bash

LOGS_DIR="./logs/"
PROGRAM="./lemipc"

rm -rf ${LOGS_DIR}
mkdir -p ${LOGS_DIR}

if [[ $# -ne 2 ]]; then
    echo invalid use 
    exit 1
fi

nb_player=${1}
nb_team=${2}
for ((i=0; i<${nb_player}; i++));
do
    team_id=$(((i % ${nb_team}) + 1))
    printf "%-25s Team: [   %-4d]\n" "Lauching number |${i}|" "${team_id}"
    # valgrind --log-file=./val_log_player${i}.log 
    # valgrind --log-file=./val_log_player${i}.log ${PROGRAM} ${team_id} >> ${LOGS_DIR}player.log & # >/dev/null 2>&1 &
    ${PROGRAM} ${team_id} 10 10 >> ${LOGS_DIR}player${i}.log 2>&1 & # >/dev/null 2>&1 &
    sleep 0.0001
done

${PROGRAM} visualizer #> viz.log 2>&1 &

