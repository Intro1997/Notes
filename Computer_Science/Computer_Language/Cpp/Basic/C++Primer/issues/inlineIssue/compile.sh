cd inlineDemo && sh compile.sh && cd ..
cd withoutInlineDemo && sh compile.sh && cd ..
mv inlineDemo/a.out inlineDemo.out
mv withoutInlineDemo/a.out withoutInlineDemo.out
ls -l *.out