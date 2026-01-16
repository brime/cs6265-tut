#!/bin/bash

PKG=`basename $(pwd)`
tar -zcvf ../$PKG.tar.gz \
    --exclude=".vagrant/" \
    --exclude="*.tar.gz" \
    --exclude="*.log" \
    --exclude="deploy.py" \
    .
