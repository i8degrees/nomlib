#!/bin/sh
LIBSNDFILE=libsndfile-1.0.25
LIBSNDFILE_TAR=${LIBSNDFILE}.tar.gz

wget http://www.mega-nerd.com/libsndfile/files/${LIBSNDFILE_TAR}
tar xzvf ${LIBSNDFILE_TAR}
mv -v ${LIBSNDFILE} libsndfile-src
rm -fv libsndfile-1.0.25.tar.gz

