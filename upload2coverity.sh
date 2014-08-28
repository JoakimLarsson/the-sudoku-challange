rm -rf cov-int
make clean
cov-build --dir cov-int make -f Makefile.reference
cov-build --dir cov-int make pef
tar czvf tsc.tgz cov-int
curl --form project=JoakimLarsson%2Fthe-sudoku-challange --form token=nVCkhbcM94Ach6a7sci6kw --form email=joakim@bildrulle.nu --form file=@./tsc.tgz --form version="Version" --form description="Description"  https://scan.coverity.com/builds?project=JoakimLarsson%2Fthe-sudoku-challange
