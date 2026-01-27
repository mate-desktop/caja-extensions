#!/usr/bin/bash

# Use grouped output messages
infobegin() {
	echo "::group::${1}"
}
infoend() {
	echo "::endgroup::"
}

# Required packages on Debian
requires=(
	ccache # Use ccache to speed up build
)

# https://salsa.debian.org/debian-mate-team/caja-extensions
requires+=(
	autoconf-archive
	autopoint
	gcc
	git
	libcaja-extension-dev
	libdbus-1-dev
	libdbus-glib-1-dev
	libdconf-dev
	libgtk-3-dev
	libgupnp-1.6-dev
	libmate-desktop-dev
	libstartup-notification0-dev
	libxml2-utils
	make
	mate-common
	pkg-config
	libgstreamer-plugins-base1.0-dev
)

infobegin "Update system"
apt-get update -qq
infoend

infobegin "Install dependency packages"
env DEBIAN_FRONTEND=noninteractive \
	apt-get install --assume-yes \
	${requires[@]}
infoend
