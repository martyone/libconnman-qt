include(tests_common.pri)

pro_file_basename = $$basename(_PRO_FILE_)
pro_file_basename = $$replace(pro_file_basename, '.pro', '')

TEMPLATE = app
QT += dbus testlib

equals(QT_MAJOR_VERSION, 4): {
    LIBS += -l$$qtLibraryTarget(connman-qt4) -L$${OUT_PWD}/../libconnman-qt
}

equals(QT_MAJOR_VERSION, 5):  {
    LIBS += -l$$qtLibraryTarget(connman-qt5) -L$${OUT_PWD}/../libconnman-qt5
}

TARGET = $${pro_file_basename}.bin

SOURCES = $${pro_file_basename}.cpp


target.path = $${INSTALL_TESTDIR}
INSTALLS += target

defineTest(usesBendyBus) {
    exists($${pro_file_basename}.machine) {
        return(true)
    } else {
        return(false)
    }
}

usesBendyBus() {
    machine_file.path = $${INSTALL_TESTDIR}
    machine_file.files = $${pro_file_basename}.machine
    INSTALLS += machine_file
}

check.depends = all
usesBendyBus() {
    check.commands = \
        env --unset=DBUS_SESSION_BUS_ADDRESS \
            LD_LIBRARY_PATH="$${OUT_PWD}/../libconnman-qt:${LD_LIBRARY_PATH}" \
            bendy-bus --run-iters=1 --pass-through-environment \
                $${PWD}/$${pro_file_basename}.machine $${OUT_PWD}/interfaces.xml \
                -- $${OUT_PWD}/$${TARGET}
} else {
    check.commands = '\
        bash -c \'eval `dbus-launch --sh-syntax`; \
            trap "kill \$\${DBUS_SESSION_BUS_PID}" EXIT; \
            export LD_LIBRARY_PATH="$${OUT_PWD}/../libconnman-qt:\$\${LD_LIBRARY_PATH}"; \
            $${OUT_PWD}/$${TARGET};\''
}
check.CONFIG = phony
QMAKE_EXTRA_TARGETS += check
