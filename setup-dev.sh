#!/bin/bash

log() {
    echo "[$(date)] [$1]: $2"
}

checkForBrew() {
    if ! hash brew 2>/dev/null; then
        log "ERROR" "Expected brew to be installed"
        exit 1
    fi
}

installOpenCV() {
    if ! brew ls --versions opencv > /dev/null; then
        log "INFO" "Installing OpenCV"
        brew install opencv
        log "INFO" "Installed OpenCV"
    else
        log "INFO" "OpenCV already installed"
    fi
}

installPkgConfig() {
    if ! brew ls --versions pkg-config > /dev/null; then
        log "INFO" "Installing pkg-config"
        brew install pkg-config
        log "INFO" "Installed pkg-config"
    else
        log "INFO" "pkg-config already installed"
    fi
}

showOpenCVLinkerFlags() {
    OPENCV_VERSION=$(basename "$(find /usr/local/Cellar/opencv -maxdepth 1  | sort | tail -n1)")
    OPENCV_PACKAGE_PATH="/usr/local/Cellar/opencv/$OPENCV_VERSION/lib/pkgconfig/opencv4.pc"
    log "INFO" "OpenCV Linker Flags:"
    log "INFO" "$(pkg-config --cflags --libs $OPENCV_PACKAGE_PATH)"
}


############################################################
###################### Execute Script ######################
############################################################
checkForBrew
installOpenCV
installPkgConfig
showOpenCVLinkerFlags

