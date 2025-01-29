@ECHO OFF
call del *~
pushd controller-template
call del *~
popd
pushd controller-far-left
call update.bat
call del *~
popd
pushd controller-far-right
call update.bat
call del *~
popd
pushd controller-center-left
call update.bat
call del *~
popd
pushd controller-center-right
call update.bat
call del *~
popd
pushd controller-center-left-sewer
call update.bat
call del *~
popd
pushd controller-floor
call update.bat
call del *~
popd
