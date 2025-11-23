#!/bin/bash

HEADER=$'// Copyright 2025 Atalante Studio.\n// Distributed under the MIT License.'

files=$(git diff --diff-filter=d --name-only origin/$GITHUB_BASE_REF origin/$GITHUB_HEAD_REF -- *.{cpp,hpp,ipp})
unlicensed_files=()

for file in $files; do
	line=$(head -n 2 $file | sed "s/^\xef\xbb\xbf//")

	if [ "$line" != "$HEADER" ]; then
		unlicensed_files+=($file)
	fi
done

if [[ ${#unlicensed_files[@]} -gt 0 ]]; then
	echo "The following file(s) don't have a valid copyright license header." >> $GITHUB_STEP_SUMMARY

	for file in "${unlicensed_files[@]}"; do
		echo "- [$file]($GITHUB_HEAD_URL/$file)" >> $GITHUB_STEP_SUMMARY
	done

	exit 1
fi