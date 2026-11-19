SUMMARY = "Turn an LED (libgpiod) on when DHT22 temperature exceeds a threshold"
DESCRIPTION = "Reads the DHT22 through the default kernel dht11 IIO driver \
(sysfs in_temp_input) and drives the LED on GPIO17 with libgpiod."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = " \
    file://dht22.c \
    file://dht22.service \
"

S = "${WORKDIR}"

DEPENDS = "libgpiod"

inherit systemd

SYSTEMD_SERVICE:${PN} = "dht22.service"

do_compile() {
    ${CC} ${CFLAGS} ${LDFLAGS} -Wall -o dht22 ${S}/dht22.c -lgpiod
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${B}/dht22 ${D}${bindir}/dht22

    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${S}/dht22.service ${D}${systemd_system_unitdir}/dht22.service
}
