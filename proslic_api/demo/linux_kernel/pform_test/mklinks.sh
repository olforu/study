#!/bin/bash
#
# Simple shell script to create the needed symlinks for first time setup.
# ASSUMES PROSLIC_API_DIR is set to point to proslic_api install directory.
#

PROSLIC_TEST_DIR="$PROSLIC_API_DIR/demo/pform_test"

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

  if [ -h "api_src" ]; then
    echo "Deleting existing symbolic link for 'api_src'"
    rm api_src
  fi

  if [ -h "custom" ]; then
    echo "Deleting existing symbolic link for 'custom'"
    rm custom
  fi

  if [ -h "inc" ]; then
    echo "Deleting existing symbolic link for 'inc'"
    rm inc
  fi

  if [ -h "platform" ]; then
    echo "Deleting existing symbolic link for 'platform'"
    rm platform 
  fi

  if [ -d "$PROSLIC_API_DIR/src" ]; then
    ln -s "$PROSLIC_API_DIR/src" api_src
  else
    echo "$PROSLIC_API_DIR/src is not a directory"
  fi

  if [ -d "$PROSLIC_TEST_DIR/src" ]; then
    ln -s "$PROSLIC_TEST_DIR/src" .
  else
    echo "$PROSLIC_API_DIR/src is not a directory"
  fi

  if [ -d "$PROSLIC_API_DIR/demo/platform" ]; then
    ln -s "$PROSLIC_API_DIR/demo/platform" .
  else
    echo "$PROSLIC_API_DIR/demo/platform is not a directory"
  fi

  if [ -d "$PROSLIC_TEST_DIR/custom" ]; then
    ln -s "$PROSLIC_TEST_DIR/custom" .
  else
    echo "$PROSLIC_TEST_DIR/custom"
  fi

 if [ -d "$PROSLIC_TEST_DIR/inc" ]; then
    ln -s "$PROSLIC_TEST_DIR/inc" .
  else
    echo "$PROSLIC_TEST_DIR/inc"
  fi


fi


