#!/bin/bash

TEST_NAME="${1}"

cd "$(dirname "$0")" || exit 1

if ! [[ -n ${TEST_NAME} && !(${TEST_NAME} =~ /) && -x ${TEST_NAME}.bin ]]
then
    echo "Usage: $0 <test-name> [test-args...]" >&2
    exit 1
fi

if [[ -e ${TEST_NAME}.machine ]]
then
    BB_ARGS="${BB_ARGS} --run-iters=1 --pass-through-environment"

    # There must be bug in bendy-bus: with the switch '--pass-through-environment'
    # DBUS_SESSION_BUS_ADDRESS instead of being replaced appears **twice** in process environment.
    unset DBUS_SESSION_BUS_ADDRESS

    shift
    bendy-bus ${BB_ARGS} ${TEST_NAME}.machine interfaces.xml -- $(pwd)/${TEST_NAME}.bin "${@}"
else
    eval `dbus-launch --sh-syntax`
    trap "kill ${DBUS_SESSION_BUS_PID}" EXIT
    ./${TEST_NAME}.bin "${@}"
fi
