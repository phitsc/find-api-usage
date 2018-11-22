@echo off
set /p project_name= <project.cfg
python3 api-utils-common/scripts/run.py --tool %project_name% %*