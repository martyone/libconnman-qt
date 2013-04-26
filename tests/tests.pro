include(tests_common.pri)

TEMPLATE = subdirs
SUBDIRS = \
    ut_agent.pro \
    ut_clock.pro \
    ut_basic.pro \
    ut_manager.pro \
    ut_service.pro \
    ut_session.pro \
    ut_technology.pro \

DBUS_INTROSPECTION_FILES = \
    $${PWD}/../libconnman-qt/connman_clock.xml \
    $${PWD}/../libconnman-qt/connman_manager.xml \
    $${PWD}/../libconnman-qt/connman_notification.xml \
    $${PWD}/../libconnman-qt/connman_service.xml \
    $${PWD}/../libconnman-qt/connman_session.xml \
    $${PWD}/../libconnman-qt/connman_technology.xml \

qtPrepareTool(QMAKE_XMLPATTERNS, xmlpatterns)

make_default.CONFIG = phony
QMAKE_EXTRA_TARGETS += make_default

extract_interfaces.target = interfaces.xml
extract_interfaces.depends = $${PWD}/extract-interfaces.xsl $${DBUS_INTROSPECTION_FILES}
extract_interfaces.commands = $${QMAKE_XMLPATTERNS} $${PWD}/extract-interfaces.xsl \
    -initial-template main \
    -param input-files=$$join(DBUS_INTROSPECTION_FILES, ':') \
    -output .interfaces.xml \
    && mv -f .interfaces.xml interfaces.xml || { rm -f .interfaces.xml; false; }
make_default.depends += extract_interfaces
QMAKE_EXTRA_TARGETS += extract_interfaces

extract_interfaces_xsl.path = $${INSTALL_TESTDIR}
extract_interfaces_xsl.files = extract-interfaces.xsl
INSTALLS += extract_interfaces_xsl

interfaces_xml.path = $${INSTALL_TESTDIR}
interfaces_xml.files = $${extract_interfaces.target}
interfaces_xml.CONFIG += no_check_exist
INSTALLS += interfaces_xml

runtest_sh.path = $${INSTALL_TESTDIR}
runtest_sh.files = runtest.sh
INSTALLS += runtest_sh

tests_xml.path = $${INSTALL_TESTDIR}
tests_xml.files = tests.xml
tests_xml.CONFIG = no_check_exist
INSTALLS += tests_xml

configure_tests_xml.target = tests.xml
configure_tests_xml.depends = $${PWD}/tests.xml.in
configure_tests_xml.commands = 'sed -e "s!@INSTALL_TESTDIR@!$${INSTALL_TESTDIR}!g" \
    < $${PWD}/tests.xml.in > tests.xml'
QMAKE_EXTRA_TARGETS += configure_tests_xml
make_default.depends += configure_tests_xml

check.depends = make_default
check.CONFIG = phony recursive
QMAKE_EXTRA_TARGETS += check
