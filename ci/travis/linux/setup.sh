#!/usr/bin/env bash

sudo apt-add-repository --yes ppa:beineri/opt-qt-5.10.1-trusty
sudo apt-get -qq update
sudo apt-get --yes install qt510-meta-minimal qt510tools qt510location
sudo /opt/qt510/bin/qt510-env.sh