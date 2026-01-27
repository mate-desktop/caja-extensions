#!/usr/bin/bash

# Use grouped output messages
infobegin() {
	echo "::group::${1}"
}
infoend() {
	echo "::endgroup::"
}

# Required packages on Fedora
requires=(
	ccache # Use ccache to speed up build
)

# https://src.fedoraproject.org/cgit/rpms/caja-extensions.git
requires+=(
	autoconf-archive
	caja-devel
	dbus-glib-devel
	gajim
	gcc
	git
	gstreamer1-plugins-base-devel
	gtk3-devel
	gupnp-devel
	make
	mate-common
	mate-desktop-devel
	redhat-rpm-config
)

infobegin "Update system"
dnf update -y
infoend

infobegin "Install dependency packages"
dnf install -y ${requires[@]}
infoend
