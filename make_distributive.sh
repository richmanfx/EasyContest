#!/bin/sh

# Сборка дистрибутива EasyContest

version=$1
current_date=`/bin/date +%Y%m%d`

binarnic=bin/release/EasyContest
docs=documentation/EasyContest.pdf
resources="resource/ec.png resource/EasyContest.desktop.example"
translations=translations/*.qm
configs=".easycontest/ec.cnf.example .easycontest/contests/*.ec.example"

/bin/tar cjvf EasyContest-${current_date}-${version}.tar.bz2 ${binarnic} ${docs} ${resources} ${translations} ${configs}

