cd cmp0 && sh compile.sh
cd ..
cd cmp1 && sh compile.sh
cd ..
mv cmp0/a.out ./cmp0.out
mv cmp1/a.out ./cmp1.out
ls -l cmp0.out
ls -l cmp1.out
