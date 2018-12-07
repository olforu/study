#!/bin/bash
#
# Simple shell script to create the needed symlinks for first time setup.
# ASSUMES PROSLIC_API_DIR is set to point to proslic_api install directory.
#

if [ "$PROSLIC_API_DIR" == "" ]; then
 	echo "Fatal: PROSLIC_API_DIR undefined"
else
  echo "PROSLIC_API_DIR defined, continuing."
  if [ -h "src" ]; then
    echo "Deleting existing symbolic link for 'src'"
    rm src
  fi

  if [ -h "patch_files" ]; then
    echo "Deleting existing symbolic link for 'patch_files'"
    rm patch_files
  fi

  if [ -d "$PROSLIC_API_DIR/src" ]; then
    ln -s "$PROSLIC_API_DIR/src" .
  else
    echo "$PROSLIC_API_DIR/src is not a directory"
  fi

  if [ -d "$PROSLIC_API_DIR/patch_files" ]; then
    ln -s "$PROSLIC_API_DIR/patch_files" .
  else
    echo "$PROSLIC_API_DIR/patch_files is not a directory"
  fi
fi

