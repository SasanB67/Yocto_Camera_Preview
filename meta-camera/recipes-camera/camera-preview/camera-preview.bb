SUMMARY = "Start a full-screen camera preview on HDMI at boot"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI = "file://camera-preview.service"

S = "${WORKDIR}"

inherit systemd

SYSTEMD_SERVICE:${PN} = "camera-preview.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"

do_install() {
    install -d ${D}${systemd_system_unitdir}
    install -m 0644 ${WORKDIR}/camera-preview.service ${D}${systemd_system_unitdir}/
}

RDEPENDS:${PN} = "libcamera libcamera-apps v4l-utils"
