#!/usr/bin/env bash

{
	read -r; project_name=${REPLY//[$'\r\n']}
} < project.cfg

python3 api-utils-common/scripts/make.py --tool "${project_name}" $@

