#!/bin/bash

LOGS_DIR="./logs/"
PROGRAM="./lemipc"

usage() {
	echo "Usage: ${0} <player-nb> <team-nb> [<board-width>] [<board-height>]" >&2
	exit 1
}

[[ "$#" -lt 2 || "$#" -gt 4 ]] && usage

rm -rf ${LOGS_DIR}
mkdir -p ${LOGS_DIR}

nb_player=${1}
nb_team=${2}
board_width=${3:-$(bc <<< "sqrt(${nb_player}) * 2")}
board_height=${4:-${board_width}}

for ((i=0; i<${nb_player}; i++));
do
    team_id=$(((i % ${nb_team}) + 1))
    printf "%-25s Team: [   %-4d]\n" "Lauching number |${i}|" "${team_id}"
    # valgrind --log-file=./val_log_player${i}.log 
    # valgrind --log-file=./val_log_player${i}.log ${PROGRAM} ${team_id} >> ${LOGS_DIR}player.log & # >/dev/null 2>&1 &
    ${PROGRAM} ${team_id} ${board_width} ${board_height} >> ${LOGS_DIR}player${i}.log 2>&1 & # >/dev/null 2>&1 &
    sleep 0.0001
done

${PROGRAM} visualizer #> viz.log 2>&1 &

