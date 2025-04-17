#!/bin/bash

PROGRAM="./lemipc"

rm player_log viz_log

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
    ${PROGRAM} ${team_id} >> player_log 2>&1 & # >/dev/null 2>&1 &
    sleep 0.01
done

${PROGRAM} visualizer >> viz_log 2>&1 &