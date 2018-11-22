@echo off
set /p project_name= <project.cfg
python3 api-utils-common/scripts/make.py --tool %project_name% %*