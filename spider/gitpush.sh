echo "输入上传到github的文件"
read filename
echo "输入注释"
read commit
git add $filename
git commit -m"$commit"
git push -u origin master
