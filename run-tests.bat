@echo off
< project.cfg (
	set /p project_name=
	set /p test_args=
)

python3 api-utils-common/scripts/test.py --tool %project_name% %test_args% %*